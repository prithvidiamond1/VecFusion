from __future__ import annotations

import re
from pathlib import Path

from .adapters import CompilerBaselineAdapter, LLMVectorizerAdapter, VecTransAdapter
from .types import PipelineResult, TransformResult


class PipelineOrchestrator:
    def __init__(self, vectrans_root: Path, llmvec_root: Path) -> None:
        self.vectrans = VecTransAdapter(vectrans_root)
        self.llmvec = LLMVectorizerAdapter(llmvec_root)
        self.compiler = CompilerBaselineAdapter()

    def run(self, pipeline: str, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        if pipeline == "pipeline1":
            return self.run_pipeline1(source_file, scalar_function, outdir)
        if pipeline == "pipeline2":
            return self.run_pipeline2(source_file, scalar_function, outdir)
        raise ValueError(f"Unknown pipeline: {pipeline}")

    def run_pipeline1(self, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        outdir.mkdir(parents=True, exist_ok=True)
        steps: list[TransformResult] = []

        llm_result = self.llmvec.run(
            source_file,
            scalar_function,
            outdir / "llmvec",
            pipeline_context="raw_source",
        )
        steps.append(llm_result)
        if llm_result.ok:
            return self._finalize("pipeline1", llm_result, steps, outdir)

        vectrans_result = self.vectrans.run(source_file.read_text(), outdir / "vectrans")
        steps.append(vectrans_result)
        if vectrans_result.ok:
            return self._finalize("pipeline1", vectrans_result, steps, outdir)

        compiler_result = self.compiler.run(source_file, outdir / "compiler")
        steps.append(compiler_result)
        return self._finalize("pipeline1", compiler_result, steps, outdir)

    def run_pipeline2(self, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        outdir.mkdir(parents=True, exist_ok=True)
        steps: list[TransformResult] = []

        vectrans_result = self.vectrans.run(source_file.read_text(), outdir / "vectrans")
        steps.append(vectrans_result)

        if vectrans_result.ok and vectrans_result.candidate_code:
            vectrans_code_path = outdir / "vectrans_verified_input.c"
            normalized_code = self._restore_original_function_name(
                vectrans_result.candidate_code,
                scalar_function,
            )
            vectrans_code_path.write_text(normalized_code)

            llm_result = self.llmvec.run(
                vectrans_code_path,
                scalar_function,
                outdir / "llmvec",
                pipeline_context="vectrans_preprocessed",
            )
            steps.append(llm_result)
            if llm_result.ok:
                return self._finalize("pipeline2", llm_result, steps, outdir)

        compiler_result = self.compiler.run(source_file, outdir / "compiler")
        steps.append(compiler_result)
        return self._finalize("pipeline2", compiler_result, steps, outdir)

    def _restore_original_function_name(self, code: str, scalar_function: str) -> str:
        opt_name = f"{scalar_function}_opt"
        return re.sub(rf"\b{re.escape(opt_name)}\b", scalar_function, code)

    def _finalize(
        self,
        pipeline: str,
        final_result: TransformResult,
        steps: list[TransformResult],
        outdir: Path,
    ) -> PipelineResult:
        result = PipelineResult(
            pipeline=pipeline,
            success=final_result.ok,
            final_stage=final_result.stage,
            final_code_path=final_result.artifact_path,
            summary_path="",
            steps=steps,
        )
        summary_path = result.write_summary(outdir)
        result.summary_path = str(summary_path)
        return result