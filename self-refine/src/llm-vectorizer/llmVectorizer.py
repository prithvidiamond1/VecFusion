#!/usr/bin/env python3
"""
Minimal LLM vectorizer prototype.
Scope:
- C kernels only
- single scalar function in the input file
- the vectorized function must keep the same signature and be renamed to
  `vectorized_<scalar_function>`
- best effort for simple array-processing kernels
"""

from __future__ import annotations

import argparse
import hashlib
import os
import re
import shlex
import subprocess
import sys
import tempfile
import textwrap
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

import requests


@dataclass
class RunConfig:
    source_path: Path
    scalar_function: str
    model: str
    max_rounds: int
    num_trials: int
    array_len: int
    random_seed: int
    compiler: str
    compiler_flags: list[str]
    work_dir: Optional[Path]
    target_hint: str
    dry_run: bool
    api_base_url: str
    api_timeout: int


@dataclass
class CandidateResult:
    round_idx: int
    response_text: str
    candidate_code: str
    compile_ok: bool
    tests_ok: bool
    report: str
    binary_path: Optional[Path] = None
    candidate_path: Optional[Path] = None


@dataclass
class AliveResult:
    # "PASS"         — alive-tv confirmed the transformation is correct
    # "FAIL"         — alive-tv found a concrete counterexample
    # "INCONCLUSIVE" — timeout, IR compile error, or alive-tv parse/internal error
    verdict: str
    message: str


def parse_args() -> RunConfig:
    parser = argparse.ArgumentParser(description="Simple LLM vectorizer prototype.")
    parser.add_argument("source", type=Path, help="Path to a C file containing the scalar function.")
    parser.add_argument(
        "--scalar-function",
        required=True,
        help="Name of the scalar function to vectorize.",
    )
    parser.add_argument(
        "--model",
        default=os.environ.get("LLM_VECTORIZER_MODEL", "claude-sonnet-4-6"),
        help="Anthropic model name.",
    )
    parser.add_argument("--max-rounds", type=int, default=4, help="Maximum vectorization attempts.")
    parser.add_argument("--num-trials", type=int, default=64, help="Number of randomized tests.")
    parser.add_argument("--array-len", type=int, default=128, help="Array length used in tests.")
    parser.add_argument("--random-seed", type=int, default=7, help="Seed used by the test harness.")
    parser.add_argument("--compiler", default="clang", help="C compiler.")
    parser.add_argument(
        "--compiler-flag",
        action="append",
        default=[],
        help="Extra compiler flag. Repeat to add more.",
    )
    parser.add_argument(
        "--work-dir",
        type=Path,
        default=None,
        help="Optional directory for intermediate files. Defaults to a temp directory.",
    )
    parser.add_argument(
        "--target-hint",
        default="portable SIMD-style C using compiler vector types or unrolled/vector-friendly code",
        help="Short hint describing the target vectorization style.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print prompts and exit without calling the LLM.",
    )
    parser.add_argument(
        "--api-base-url",
        default=os.environ.get("ANTHROPIC_BASE_URL", "https://api.anthropic.com"),
        help="Anthropic API base URL.",
    )
    parser.add_argument(
        "--api-timeout",
        type=int,
        default=120,
        help="HTTP timeout in seconds for model calls.",
    )
    args = parser.parse_args()
    return RunConfig(
        source_path=args.source.resolve(),
        scalar_function=args.scalar_function,
        model=args.model,
        max_rounds=args.max_rounds,
        num_trials=args.num_trials,
        array_len=args.array_len,
        random_seed=args.random_seed,
        compiler=args.compiler,
        compiler_flags=args.compiler_flag,
        work_dir=args.work_dir.resolve() if args.work_dir else None,
        target_hint=args.target_hint,
        dry_run=args.dry_run,
        api_base_url=args.api_base_url.rstrip("/"),
        api_timeout=args.api_timeout,
    )


def read_source(path: Path) -> str:
    try:
        return path.read_text()
    except FileNotFoundError as exc:
        raise SystemExit(f"Source file not found: {path}") from exc


def load_dotenv(dotenv_path: Path) -> None:
    if not dotenv_path.exists():
        return

    for raw_line in dotenv_path.read_text().splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip().strip("'").strip('"')
        if key and key not in os.environ:
            os.environ[key] = value


def get_api_key() -> str:
    api_key = os.environ.get("ANTHROPIC_API_KEY") or os.environ.get("CLAUDE_API_KEY")
    if not api_key:
        raise SystemExit("ANTHROPIC_API_KEY or CLAUDE_API_KEY is required unless --dry-run is used.")
    return api_key


class ClaudeAgent:
    def __init__(self, name: str, system_message: str, config: RunConfig, api_key: str):
        self.name = name
        self.system_message = system_message
        self.config = config
        self.api_key = api_key

    def generate_reply(self, prompt: str) -> str:
        try:
            response = requests.post(
                f"{self.config.api_base_url}/v1/messages",
                headers={
                    "content-type": "application/json",
                    "x-api-key": self.api_key,
                    "anthropic-version": "2023-06-01",
                },
                json={
                    "model": self.config.model,
                    "max_tokens": 1600,
                    "temperature": 0,
                    "system": self.system_message,
                    "messages": [{"role": "user", "content": prompt}],
                },
                timeout=self.config.api_timeout,
            )
        except requests.RequestException as exc:
            raise SystemExit(f"Anthropic API request failed: {exc}") from exc
        if response.status_code >= 400:
            raise SystemExit(f"Anthropic API error {response.status_code}: {response.text}")

        payload = response.json()
        parts = payload.get("content", [])
        text_parts = [part.get("text", "") for part in parts if part.get("type") == "text"]
        reply = "\n".join(part for part in text_parts if part).strip()
        if not reply:
            raise SystemExit(f"Anthropic API returned no text content: {payload}")
        return reply


def make_vectorizer_prompt(config: RunConfig) -> str:
    return textwrap.dedent(
        f"""
        You are a C loop vectorization assistant.

        Your job is to rewrite one scalar function into a vectorized candidate that is still valid C.

        Hard requirements:
        - Emit exactly one C code block and nothing else outside the code block.
        - Preserve the original function signature exactly, except rename the function to
          `vectorized_{config.scalar_function}`.
        - Keep semantics identical to the scalar function.
        - Do not emit a full file. Emit only the candidate function and any typedefs required by
          that function. Do NOT emit any helper functions (static inline or otherwise) — inline
          all logic directly into the candidate function body.
        - Keep the code portable and compilable with clang on macOS.
        - Prefer {config.target_hint}.
        - Include a scalar cleanup tail when needed.
        - Do not call external libraries beyond standard headers that would already exist.

        Optimization goal:
        - Produce a simple first working vectorized version, not a perfect one.
        - Favor correctness and compilability over aggressive tricks.

        Reply with only the code block.
        """
    ).strip()


def make_tester_prompt() -> str:
    return textwrap.dedent(
        """
        You are a tester agent for a loop vectorization pipeline.

        You will receive:
        - the vectorizer candidate code
        - an external compile/test report

        Your job:
        - identify the smallest concrete fixes the vectorizer should make next
        - focus on correctness and compilability
        - if the candidate already passes, reply with only: PASS

        Constraints:
        - keep the response under 8 bullet points
        - be specific about what failed and how to fix it
        - do not restate the whole candidate
        """
    ).strip()


def make_initial_task(config: RunConfig, scalar_source: str) -> str:
    return textwrap.dedent(
        f"""
        Vectorize the scalar C function `{config.scalar_function}` below.

        Scalar source:
        ```c
        {scalar_source}
        ```

        Produce a candidate function named `vectorized_{config.scalar_function}`.
        """
    ).strip()


def make_revision_task(previous_code: str, test_feedback: str, scalar_source: str, config: RunConfig) -> str:
    return textwrap.dedent(
        f"""
        Revise your candidate for `{config.scalar_function}` based on tester feedback.

        Original scalar source:
        ```c
        {scalar_source}
        ```

        Previous candidate:
        ```c
        {previous_code}
        ```

        Tester feedback:
        {test_feedback}

        Return a corrected candidate function named `vectorized_{config.scalar_function}`.
        """
    ).strip()


def extract_code_block(text: str) -> str:
    fenced = re.findall(r"```(?:c|cpp)?\s*(.*?)```", text, flags=re.DOTALL | re.IGNORECASE)
    if fenced:
        return fenced[0].strip()
    return text.strip()


def build_harness(scalar_source: str, scalar_function: str, candidate_code: str, config: RunConfig) -> str:
    vectorized_function = f"vectorized_{scalar_function}"
    return textwrap.dedent(
        f"""
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        {scalar_source}

        {candidate_code}

        static uint32_t next_u32(uint32_t *state) {{
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }}

        static void fill_i32(int *buf, int n, uint32_t *state) {{
            for (int i = 0; i < n; ++i) {{
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }}
        }}

        static uint64_t checksum_i32(const int *buf, int n) {{
            uint64_t acc = 1469598103934665603ull;
            for (int i = 0; i < n; ++i) {{
                acc ^= (uint32_t)buf[i];
                acc *= 1099511628211ull;
            }}
            return acc;
        }}

        int main(void) {{
            const int n = {config.array_len};
            uint32_t seed = {config.random_seed}u;
            int a[{config.array_len}];
            int b[{config.array_len}];
            int out_scalar[{config.array_len}];
            int out_vector[{config.array_len}];

            for (int trial = 0; trial < {config.num_trials}; ++trial) {{
                fill_i32(a, n, &seed);
                fill_i32(b, n, &seed);
                memset(out_scalar, 0, sizeof(out_scalar));
                memset(out_vector, 0, sizeof(out_vector));

                {scalar_function}(a, b, out_scalar, n);
                {vectorized_function}(a, b, out_vector, n);

                if (memcmp(out_scalar, out_vector, sizeof(out_scalar)) != 0) {{
                    fprintf(stderr, "Mismatch on trial %d\\n", trial);
                    fprintf(stderr, "scalar_checksum=%llu\\n", (unsigned long long)checksum_i32(out_scalar, n));
                    fprintf(stderr, "vector_checksum=%llu\\n", (unsigned long long)checksum_i32(out_vector, n));
                    for (int i = 0; i < n; ++i) {{
                        if (out_scalar[i] != out_vector[i]) {{
                            fprintf(stderr, "first_diff_index=%d scalar=%d vector=%d\\n", i, out_scalar[i], out_vector[i]);
                            break;
                        }}
                    }}
                    return 2;
                }}
            }}

            printf("PASS trials=%d checksum=%llu\\n", {config.num_trials}, (unsigned long long)checksum_i32(out_vector, n));
            return 0;
        }}
        """
    ).strip() + "\n"


def run_compile_and_tests(
    scalar_source: str,
    candidate_code: str,
    config: RunConfig,
    round_idx: int,
) -> CandidateResult:
    work_parent = config.work_dir if config.work_dir else Path(tempfile.mkdtemp(prefix="llm_vectorizer_"))
    work_parent.mkdir(parents=True, exist_ok=True)
    candidate_path = work_parent / f"candidate_round_{round_idx}.c"
    binary_path = work_parent / f"candidate_round_{round_idx}"

    candidate_path.write_text(build_harness(scalar_source, config.scalar_function, candidate_code, config))

    compile_cmd = [
        config.compiler,
        str(candidate_path),
        "-O3",
        "-std=c11",
        "-Wall",
        "-Wextra",
        "-o",
        str(binary_path),
        *config.compiler_flags,
    ]
    compile_proc = subprocess.run(compile_cmd, capture_output=True, text=True)
    if compile_proc.returncode != 0:
        report = textwrap.dedent(
            f"""
            Compile failed.
            Command: {' '.join(compile_cmd)}
            stderr:
            {compile_proc.stderr.strip()}
            """
        ).strip()
        return CandidateResult(
            round_idx=round_idx,
            response_text="",
            candidate_code=candidate_code,
            compile_ok=False,
            tests_ok=False,
            report=report,
            binary_path=None,
            candidate_path=candidate_path,
        )

    run_proc = subprocess.run([str(binary_path)], capture_output=True, text=True)
    tests_ok = run_proc.returncode == 0
    report_parts = [
        f"Compile succeeded for round {round_idx}.",
        f"Exit code: {run_proc.returncode}",
    ]
    if run_proc.stdout.strip():
        report_parts.append("stdout:\n" + run_proc.stdout.strip())
    if run_proc.stderr.strip():
        report_parts.append("stderr:\n" + run_proc.stderr.strip())

    return CandidateResult(
        round_idx=round_idx,
        response_text="",
        candidate_code=candidate_code,
        compile_ok=True,
        tests_ok=tests_ok,
        report="\n".join(report_parts),
        binary_path=binary_path,
        candidate_path=candidate_path,
    )


def source_set_env(set_env_path: Path) -> dict:
    """Source set_env.sh in a bash subshell and return the resulting environment as a dict.

    Uses null-delimited `env -0` output so values containing newlines are handled correctly.
    Falls back to the current process environment if the file is missing or the subshell fails.
    """
    if not set_env_path.exists():
        return dict(os.environ)
    proc = subprocess.run(
        ["bash", "-c", f"source {shlex.quote(str(set_env_path))} && env -0"],
        capture_output=True,
        text=True,
    )
    env: dict = {}
    for entry in proc.stdout.split("\0"):
        if "=" in entry:
            k, v = entry.split("=", 1)
            env[k] = v
    return env if env else dict(os.environ)


def run_alive2_check(
    scalar_source: str,
    candidate_code: str,
    config: RunConfig,
    round_idx: int,
    alive_env: dict,
) -> AliveResult:
    """Phase 6: compile both functions to LLVM IR and run alive-tv translation validation.

    Strategy:
    - Scalar IR  : compile the original scalar source with -O0.
    - Target IR  : rename `vectorized_<fn>` → `<fn>` in the candidate, then compile with -O0.
      This lets alive-tv match the two functions by name without any extra flags.
    - alive-tv verdict parsing:
        "Transformation seems to be correct!" → PASS
        "Transformation doesn't verify!"      → FAIL
        SMT timeout / Python timeout           → INCONCLUSIVE
        IR compile error / unknown output      → INCONCLUSIVE
    """
    work_parent = config.work_dir if config.work_dir else Path(tempfile.mkdtemp(prefix="llm_vectorizer_"))
    work_parent.mkdir(parents=True, exist_ok=True)

    clang = alive_env.get("CLANG_PATH", config.compiler)
    base_ir_flags = ["-O0", "-emit-llvm", "-S", "-std=c11"]

    # --- compile scalar to IR ---
    scalar_c = work_parent / f"alive_scalar_r{round_idx}.c"
    scalar_ll = work_parent / f"alive_scalar_r{round_idx}.ll"
    scalar_c.write_text(scalar_source)

    r = subprocess.run(
        [clang, str(scalar_c), *base_ir_flags, "-o", str(scalar_ll)],
        capture_output=True, text=True, env=alive_env,
    )
    if r.returncode != 0:
        return AliveResult("INCONCLUSIVE", f"Scalar IR compilation failed:\n{r.stderr.strip()}")

    # --- compile vectorized to IR (rename so alive-tv can match by name) ---
    # Simple text replace is safe: the vectorizer is instructed to emit only the
    # candidate function, so `vectorized_<fn>` appears only as the function name.
    vec_source = candidate_code.replace(
        f"vectorized_{config.scalar_function}", config.scalar_function
    )
    vec_c = work_parent / f"alive_vec_r{round_idx}.c"
    vec_ll = work_parent / f"alive_vec_r{round_idx}.ll"
    vec_c.write_text(vec_source)

    r = subprocess.run(
        [clang, str(vec_c), *base_ir_flags, "-o", str(vec_ll)],
        capture_output=True, text=True, env=alive_env,
    )
    if r.returncode != 0:
        return AliveResult("INCONCLUSIVE", f"Vectorized IR compilation failed:\n{r.stderr.strip()}")

    # --- run alive-tv ---
    alive_cmd = [
        "alive-tv",
        f"--func={config.scalar_function}",
        str(scalar_ll),
        str(vec_ll),
    ]
    try:
        r = subprocess.run(
            alive_cmd,
            capture_output=True, text=True, env=alive_env, timeout=180,
        )
    except subprocess.TimeoutExpired:
        return AliveResult("INCONCLUSIVE", "alive-tv exceeded the 180 s Python-level timeout.")

    combined = (r.stdout + r.stderr).strip()

    if "Transformation seems to be correct" in combined:
        return AliveResult("PASS", combined)
    if "Transformation doesn't verify" in combined:
        return AliveResult("FAIL", combined)
    if "timed out" in combined.lower() or "timeout" in combined.lower():
        return AliveResult("INCONCLUSIVE", f"alive-tv SMT timeout:\n{combined}")
    return AliveResult(
        "INCONCLUSIVE",
        f"alive-tv returned an unrecognised result (exit {r.returncode}):\n{combined}",
    )


def new_agent(name: str, system_message: str, config: RunConfig, api_key: str) -> ClaudeAgent:
    return ClaudeAgent(name=name, system_message=system_message, config=config, api_key=api_key)


def ask_agent(agent: ClaudeAgent, prompt: str) -> str:
    return agent.generate_reply(prompt).strip()


def candidate_digest(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()[:12]


def main() -> int:
    load_dotenv(Path(".env"))
    config = parse_args()
    scalar_source = read_source(config.source_path).strip()

    # Phase 6 environment — load once so it's available after a successful test.
    set_env_path = Path("set_env.sh")
    alive_env = source_set_env(set_env_path)
    if set_env_path.exists():
        print(f"[alive2] Loaded environment from {set_env_path}")
    else:
        print("[alive2] set_env.sh not found — Phase 6 will be skipped.", file=sys.stderr)

    if config.dry_run:
        print("== Vectorizer system prompt ==")
        print(make_vectorizer_prompt(config))
        print("\n== Tester system prompt ==")
        print(make_tester_prompt())
        print("\n== Initial task ==")
        print(make_initial_task(config, scalar_source))
        return 0

    api_key = get_api_key()
    vectorizer = new_agent("Vectorizer", make_vectorizer_prompt(config), config, api_key)
    tester = new_agent("Tester", make_tester_prompt(), config, api_key)

    last_feedback = ""
    last_candidate = ""
    final_result: Optional[CandidateResult] = None

    for round_idx in range(1, config.max_rounds + 1):
        if round_idx == 1:
            task = make_initial_task(config, scalar_source)
        else:
            task = make_revision_task(last_candidate, last_feedback, scalar_source, config)

        response_text = ask_agent(vectorizer, task)
        candidate_code = extract_code_block(response_text)
        result = run_compile_and_tests(scalar_source, candidate_code, config, round_idx)
        result.response_text = response_text
        final_result = result

        print(f"[round {round_idx}] candidate={candidate_digest(candidate_code)}")
        print(result.report)
        print()

        if result.tests_ok:
            print("=== SUCCESSFUL CANDIDATE ===")
            print(candidate_code)
            if result.candidate_path:
                print(f"\nSaved harness: {result.candidate_path}")

            # Phase 6: alive2 translation validation
            print("\n=== PHASE 6: ALIVE2 TRANSLATION VALIDATION ===")
            if not set_env_path.exists():
                print("Skipped (set_env.sh not found).")
                return 0
            alive_result = run_alive2_check(scalar_source, candidate_code, config, round_idx, alive_env)
            print(f"Verdict: {alive_result.verdict}")
            print(alive_result.message)
            if alive_result.verdict == "PASS":
                return 0
            if alive_result.verdict == "FAIL":
                return 1
            # INCONCLUSIVE
            return 2

        tester_prompt = textwrap.dedent(
            f"""
            Candidate code:
            ```c
            {candidate_code}
            ```

            External compile/test report:
            {result.report}
            """
        ).strip()
        last_feedback = ask_agent(tester, tester_prompt)
        last_candidate = candidate_code

        print("=== TESTER FEEDBACK ===")
        print(last_feedback)
        print()

        if last_feedback.strip() == "PASS":
            print("Tester reported PASS, but external tests did not. Continuing with external result.")

    print("Vectorization did not converge within the round budget.", file=sys.stderr)
    if final_result is not None:
        print("\n=== LAST CANDIDATE ===", file=sys.stderr)
        print(final_result.candidate_code, file=sys.stderr)
        print("\n=== LAST REPORT ===", file=sys.stderr)
        print(final_result.report, file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
