from __future__ import annotations

import importlib.util
import os
import re
import sys
from pathlib import Path
from types import ModuleType

from .types import TransformResult


_FUNC_DEF_RE = re.compile(
    r"""
    (?P<ret>
        \b[A-Za-z_][\w\s\*\[\]]*?
    )
    \s+
    (?P<name>[A-Za-z_]\w*)
    \s*
    \(
        (?P<params>[^{};]*?)
    \)
    \s*
    \{
    """,
    re.DOTALL | re.VERBOSE,
)


class VecTransAdapter:
    """Call VecTrans's iterative_vectorize flow and recover final code from its log output."""

    def __init__(self, vectrans_root: Path) -> None:
        self.vectrans_root = vectrans_root
        self.module_path = vectrans_root / "src" / "vectorizer" / "run_split.py"

    def _load_module(self) -> ModuleType:
        if not self.module_path.exists():
            raise FileNotFoundError(f"Could not find run_split.py under {self.vectrans_root}")

        extra_paths = [
            str(self.vectrans_root),
            str(self.vectrans_root / "prompt-lib"),
        ]
        for p in reversed(extra_paths):
            if p not in sys.path:
                sys.path.insert(0, p)

        spec = importlib.util.spec_from_file_location("vendored_vectrans_run_split", self.module_path)
        if spec is None or spec.loader is None:
            raise ImportError(f"Unable to load module spec from {self.module_path}")

        module = importlib.util.module_from_spec(spec)
        module.__dict__["os"] = os
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        return module

    def _extract_final_code(self, text: str) -> str:
        # Prefer explicit FINAL CODE blocks first.
        matches = re.findall(r"# FINAL CODE\s*```c\s*(.*?)```", text, flags=re.DOTALL | re.IGNORECASE)
        if matches:
            return matches[-1].strip()

        # Fallback: last fenced C block.
        fenced = re.findall(r"```c\s*(.*?)```", text, flags=re.DOTALL | re.IGNORECASE)
        if fenced:
            return fenced[-1].strip()

        return ""

    def _looks_like_full_c_function(self, code: str) -> bool:
        code = code.strip()
        if not code:
            return False

        # Must contain at least one apparent function definition.
        if not _FUNC_DEF_RE.search(code):
            return False

        # Quick brace sanity check.
        return code.count("{") >= 1 and code.count("}") >= 1

    def run(self, source_code: str, outdir: Path, max_rounds: int = 6) -> TransformResult:
        outdir.mkdir(parents=True, exist_ok=True)

        input_snapshot = outdir / "vectrans_input_snapshot.c"
        input_snapshot.write_text(source_code)

        log_path = outdir / "vectrans_output.md"
        log_path.write_text("")

        old_clang_path = os.environ.get("CLANG_PATH")
        old_sdkroot = os.environ.get("SDKROOT")

        try:
            if not os.environ.get("CLANG_PATH"):
                try:
                    import subprocess
                    clang_path = subprocess.check_output(["xcrun", "-f", "clang"], text=True).strip()
                    os.environ["CLANG_PATH"] = clang_path
                except Exception:
                    pass

            if not os.environ.get("SDKROOT"):
                try:
                    import subprocess
                    sdkroot = subprocess.check_output(["xcrun", "--show-sdk-path"], text=True).strip()
                    os.environ["SDKROOT"] = sdkroot
                except Exception:
                    pass

            module = self._load_module()

            old_cwd = Path.cwd()
            os.chdir(self.vectrans_root)
            try:
                if hasattr(module, "reset_cache"):
                    try:
                        module.reset_cache()
                    except Exception:
                        pass

                module.iterative_vectorize(
                    source_code=source_code,
                    max_attempts=max_rounds,
                    outputFileName=str(log_path),
                )
            finally:
                os.chdir(old_cwd)

            log_text = log_path.read_text() if log_path.exists() else ""
            candidate_code = self._extract_final_code(log_text)

            if not candidate_code:
                no_candidate_path = outdir / "vectrans_no_final_code.txt"
                no_candidate_path.write_text(
                    "VecTrans ran but no '# FINAL CODE' fenced block was found in the output log.\n\n"
                    + log_text
                )
                return TransformResult(
                    stage="vectrans",
                    ok=False,
                    candidate_code="",
                    summary="VecTrans ran but did not emit a final candidate block.",
                    rounds_used=max_rounds,
                    artifact_path=str(no_candidate_path),
                    metadata={
                        "input_snapshot": str(input_snapshot),
                        "log_path": str(log_path),
                    },
                )

            if not self._looks_like_full_c_function(candidate_code):
                invalid_path = outdir / "vectrans_non_function_candidate.c"
                invalid_path.write_text(candidate_code + "\n")
                return TransformResult(
                    stage="vectrans",
                    ok=False,
                    candidate_code=candidate_code,
                    summary=(
                        "VecTrans emitted a C snippet, not a full function. "
                        "Rejecting it as a pipeline handoff candidate."
                    ),
                    rounds_used=max_rounds,
                    artifact_path=str(invalid_path),
                    metadata={
                        "input_snapshot": str(input_snapshot),
                        "log_path": str(log_path),
                    },
                )

            final_candidate_path = outdir / "final_candidate.c"
            final_candidate_path.write_text(candidate_code + "\n")
            return TransformResult(
                stage="vectrans",
                ok=True,
                candidate_code=candidate_code,
                summary="VecTrans produced a full-function final candidate and wrote it to the output log.",
                rounds_used=max_rounds,
                artifact_path=str(final_candidate_path),
                metadata={
                    "input_snapshot": str(input_snapshot),
                    "log_path": str(log_path),
                },
            )

        except Exception as exc:
            failure_path = outdir / "vectrans_adapter_failure.txt"
            failure_path.write_text(f"{type(exc).__name__}: {exc}\n")
            return TransformResult(
                stage="vectrans",
                ok=False,
                candidate_code="",
                summary=f"VecTrans adapter failed before producing a candidate: {type(exc).__name__}: {exc}",
                rounds_used=max_rounds,
                artifact_path=str(failure_path),
                metadata={"input_snapshot": str(input_snapshot)},
            )
        finally:
            if old_clang_path is None:
                os.environ.pop("CLANG_PATH", None)
            else:
                os.environ["CLANG_PATH"] = old_clang_path

            if old_sdkroot is None:
                os.environ.pop("SDKROOT", None)
            else:
                os.environ["SDKROOT"] = old_sdkroot


class LLMVectorizerAdapter:
    """Call vendored LLM-Vectorizer primitives from inside VecTrans."""

    def __init__(self, llmvec_root: Path) -> None:
        self.llmvec_root = llmvec_root

        preferred = llmvec_root / "llm_vectorizer.py"
        legacy = llmvec_root / "llmVectorizer.py"

        if preferred.exists():
            self.module_path = preferred
        else:
            self.module_path = legacy

    def _load_module(self) -> ModuleType:
        if not self.module_path.exists():
            raise FileNotFoundError(
                f"Could not find llm_vectorizer.py or llmVectorizer.py under {self.llmvec_root}"
            )
        spec = importlib.util.spec_from_file_location("vendored_llm_vectorizer", self.module_path)
        if spec is None or spec.loader is None:
            raise ImportError(f"Unable to load module spec from {self.module_path}")
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        return module

    def _build_config(self, module: ModuleType, source_file: Path, scalar_function: str, outdir: Path, max_rounds: int):
        compiler_flags_env = os.environ.get("LLM_VECTORIZER_COMPILER_FLAGS", "").strip()
        compiler_flags = compiler_flags_env.split() if compiler_flags_env else []
        return module.RunConfig(
            source_path=source_file.resolve(),
            scalar_function=scalar_function,
            model=os.environ.get("LLM_VECTORIZER_MODEL", "deepseek-chat"),
            max_rounds=max_rounds,
            num_trials=int(os.environ.get("LLM_VECTORIZER_NUM_TRIALS", "64")),
            array_len=int(os.environ.get("LLM_VECTORIZER_ARRAY_LEN", "128")),
            random_seed=int(os.environ.get("LLM_VECTORIZER_RANDOM_SEED", "7")),
            compiler=os.environ.get("LLM_VECTORIZER_COMPILER", "clang"),
            compiler_flags=compiler_flags,
            work_dir=outdir,
            target_hint=os.environ.get(
                "LLM_VECTORIZER_TARGET_HINT",
                "portable SIMD-style C using compiler vector types or unrolled/vector-friendly code",
            ),
            dry_run=False,
            api_base_url=os.environ.get("ANTHROPIC_BASE_URL", "https://api.deepseek.com/anthropic").rstrip("/"),
            api_timeout=int(os.environ.get("LLM_VECTORIZER_API_TIMEOUT", "120")),
        )

    def _pipeline_context_prefix(self, pipeline_context: str) -> str:
        if pipeline_context == "vectrans_preprocessed":
            return (
                "This input code has already been processed by an earlier VecTrans stage.\n"
                "Use that code as a starting point, not a constraint.\n"
                "You may simplify awkward structure if needed, but you must preserve the original iteration space and touched indices unless equivalence is obvious.\n"
                "Do not change which array elements are updated, do not densify sparse/block updates, and do not replace block-stride loops with full-range elementwise loops unless that is provably equivalent.\n"
                "Prefer the simplest correct loop structure that preserves behavior.\n"
                "Prioritize semantic correctness first, then vectorizability and performance.\n"
            )
        return ""

    def _contains_function_named(self, code: str, scalar_function: str) -> bool:
        return bool(re.search(rf"\b{re.escape(scalar_function)}\s*\(", code))

    def _rename_single_function(self, code: str, scalar_function: str) -> str:
        matches = list(_FUNC_DEF_RE.finditer(code))
        if len(matches) != 1:
            raise ValueError(
                "VecTrans-preprocessed input is not a single full function; cannot safely normalize handoff."
            )

        match = matches[0]
        old_name = match.group("name")
        if old_name == scalar_function:
            return code

        start, end = match.span("name")
        return code[:start] + scalar_function + code[end:]

    def _prepare_source_text(self, raw_text: str, scalar_function: str, pipeline_context: str) -> str:
        if pipeline_context != "vectrans_preprocessed":
            return raw_text

        if not _FUNC_DEF_RE.search(raw_text):
            raise ValueError(
                "VecTrans-preprocessed input is not a full function body; cannot hand off to LLM-Vectorizer."
            )

        if self._contains_function_named(raw_text, scalar_function):
            return raw_text

        return self._rename_single_function(raw_text, scalar_function)

    def run(
        self,
        source_file: Path,
        scalar_function: str,
        outdir: Path,
        max_rounds: int = 4,
        pipeline_context: str = "raw_source",
    ) -> TransformResult:
        outdir.mkdir(parents=True, exist_ok=True)

        raw_text = source_file.read_text()
        input_snapshot = outdir / "llm_vectorizer_input_snapshot.c"
        input_snapshot.write_text(raw_text)

        try:
            module = self._load_module()
            module.load_dotenv(self.llmvec_root / ".env")

            deepseek_key = os.environ.get("DEEPSEEK_API_KEY") or os.environ.get("API_KEY")
            if deepseek_key:
                os.environ["ANTHROPIC_API_KEY"] = deepseek_key

            os.environ["ANTHROPIC_BASE_URL"] = "https://api.deepseek.com/anthropic"
            os.environ["LLM_VECTORIZER_MODEL"] = "deepseek-chat"
            os.environ.pop("CLAUDE_API_KEY", None)
            os.environ.pop("ANTHROPIC_AUTH_TOKEN", None)

            prepared_text = self._prepare_source_text(raw_text, scalar_function, pipeline_context)

            prepared_text = self._prepare_source_text(raw_text, scalar_function, pipeline_context)
            prepared_source = outdir / "llm_vectorizer_prepared_input.c"
            prepared_source.write_text(prepared_text)

            config = self._build_config(module, prepared_source, scalar_function, outdir, max_rounds)
            scalar_source = module.read_source(config.source_path).strip()

            context_prefix = self._pipeline_context_prefix(pipeline_context)
            vectorizer_prompt = context_prefix + "\n" + module.make_vectorizer_prompt(config)

            try:
                api_key = module.get_api_key()
            except SystemExit as exc:
                prompts_path = outdir / "debug_prompts.txt"
                initial_task = context_prefix + "\n" + module.make_initial_task(config, scalar_source)
                prompts_path.write_text(
                    "== Vectorizer system prompt ==\n"
                    + vectorizer_prompt
                    + "\n\n== Tester system prompt ==\n"
                    + module.make_tester_prompt()
                    + "\n\n== Initial task ==\n"
                    + initial_task
                    + "\n"
                )
                return TransformResult(
                    stage="llm_vectorizer",
                    ok=False,
                    candidate_code="",
                    summary=(
                        "LLM-Vectorizer adapter is wired in, but no API key was found. "
                        f"{exc}. Debug prompts were saved for inspection."
                    ),
                    rounds_used=0,
                    artifact_path=str(prompts_path),
                    metadata={
                        "scalar_function": scalar_function,
                        "input_snapshot": str(input_snapshot),
                        "prepared_input": str(prepared_source),
                        "pipeline_context": pipeline_context,
                    },
                )

            vectorizer = module.new_agent("Vectorizer", vectorizer_prompt, config, api_key)
            tester = module.new_agent("Tester", module.make_tester_prompt(), config, api_key)

            last_feedback = ""
            last_candidate = ""
            final_result = None

            for round_idx in range(1, config.max_rounds + 1):
                round_dir = outdir / f"round_{round_idx}"
                round_dir.mkdir(parents=True, exist_ok=True)

                if round_idx == 1:
                    task = context_prefix + "\n" + module.make_initial_task(config, scalar_source)
                else:
                    task = context_prefix + "\n" + module.make_revision_task(
                        last_candidate,
                        last_feedback,
                        scalar_source,
                        config,
                    )
                (round_dir / "task_prompt.txt").write_text(task)

                response_text = module.ask_agent(vectorizer, task)
                (round_dir / "vectorizer_response.txt").write_text(response_text)
                candidate_code = module.extract_code_block(response_text)
                (round_dir / "candidate_only.c").write_text(candidate_code + "\n")

                result = module.run_compile_and_tests(scalar_source, candidate_code, config, round_idx)
                result.response_text = response_text
                final_result = result
                (round_dir / "external_report.txt").write_text(result.report + "\n")

                if result.tests_ok:
                    final_candidate_path = outdir / "final_candidate.c"
                    final_candidate_path.write_text(candidate_code + "\n")
                    return TransformResult(
                        stage="llm_vectorizer",
                        ok=True,
                        candidate_code=candidate_code,
                        summary=f"LLM-Vectorizer produced a candidate that compiled and passed randomized tests in round {round_idx}.",
                        compile_feedback=result.report,
                        unit_test_output=result.report,
                        rounds_used=round_idx,
                        artifact_path=str(final_candidate_path),
                        metadata={
                            "scalar_function": scalar_function,
                            "input_snapshot": str(input_snapshot),
                            "prepared_input": str(prepared_source),
                            "pipeline_context": pipeline_context,
                            "harness_path": str(result.candidate_path) if result.candidate_path else "",
                            "binary_path": str(result.binary_path) if result.binary_path else "",
                        },
                    )

                tester_prompt = (
                    "Candidate code:\n```c\n"
                    + candidate_code
                    + "\n```\n\nExternal compile/test report:\n"
                    + result.report
                )
                (round_dir / "tester_prompt.txt").write_text(tester_prompt)
                last_feedback = module.ask_agent(tester, tester_prompt)
                (round_dir / "tester_feedback.txt").write_text(last_feedback + "\n")
                last_candidate = candidate_code

            last_candidate_path = outdir / "last_candidate.c"
            if last_candidate:
                last_candidate_path.write_text(last_candidate + "\n")
            else:
                last_candidate_path.write_text("")

            last_report = final_result.report if final_result is not None else "No candidate was produced."
            return TransformResult(
                stage="llm_vectorizer",
                ok=False,
                candidate_code=last_candidate,
                summary="LLM-Vectorizer did not converge within the configured round budget.",
                compile_feedback=last_report,
                unit_test_output=last_report,
                rounds_used=max_rounds,
                artifact_path=str(last_candidate_path),
                metadata={
                    "scalar_function": scalar_function,
                    "input_snapshot": str(input_snapshot),
                    "prepared_input": str(prepared_source),
                    "pipeline_context": pipeline_context,
                },
            )
        except Exception as exc:
            failure_path = outdir / "adapter_failure.txt"
            failure_path.write_text(f"{type(exc).__name__}: {exc}\n")
            return TransformResult(
                stage="llm_vectorizer",
                ok=False,
                candidate_code="",
                summary=f"LLM-Vectorizer adapter failed before producing a candidate: {type(exc).__name__}: {exc}",
                artifact_path=str(failure_path),
                metadata={
                    "scalar_function": scalar_function,
                    "input_snapshot": str(input_snapshot),
                    "pipeline_context": pipeline_context,
                },
            )


class CompilerBaselineAdapter:
    """Fallback stage that leaves the source unchanged and lets the compiler do the work."""

    def run(self, source_file: Path, outdir: Path) -> TransformResult:
        outdir.mkdir(parents=True, exist_ok=True)
        baseline_path = outdir / "compiler_baseline_input.c"
        baseline_path.write_text(source_file.read_text())
        return TransformResult(
            stage="compiler_baseline",
            ok=True,
            candidate_code=baseline_path.read_text(),
            summary="Fallback selected: keep the original scalar code and rely on compiler autovectorization.",
            artifact_path=str(baseline_path),
        )