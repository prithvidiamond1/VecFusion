from __future__ import annotations

import os
from pathlib import Path

from .adapters import CompilerBaselineAdapter, LLMVectorizerAdapter, VecTransAdapter
from .types import PipelineResult, TransformResult


class PipelineOrchestrator:
    """VecTrans-owned orchestration entry point for all combined pipelines."""

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

    def _finalize(
        self,
        pipeline: str,
        outdir: Path,
        final_transform: TransformResult,
        steps: list[TransformResult],
    ) -> PipelineResult:
        result = PipelineResult(
            pipeline=pipeline,
            success=final_transform.ok,
            final_stage=final_transform.stage,
            final_code_path=final_transform.artifact_path,
            summary_path="",
            steps=steps,
        )
        summary_path = result.write_summary(outdir)
        result.summary_path = str(summary_path)
        return result

    def run_pipeline1(self, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        outdir.mkdir(parents=True, exist_ok=True)

        steps: list[TransformResult] = []

        llmvec_result = self.llmvec.run(
            source_file=source_file,
            scalar_function=scalar_function,
            outdir=outdir / "llmvec",
            max_rounds=int(os.environ.get("LLM_VECTORIZER_MAX_ROUNDS", "6")),
            pipeline_context="raw_source",
        )
        steps.append(llmvec_result)

        if llmvec_result.ok and llmvec_result.candidate_code:
            return self._finalize("pipeline1", outdir, llmvec_result, steps)

        source_code = source_file.read_text()
        vectrans_result = self.vectrans.run(
            source_code=source_code,
            outdir=outdir / "vectrans",
            max_rounds=int(os.environ.get("VECTRANS_MAX_ROUNDS", "6")),
        )
        steps.append(vectrans_result)

        if vectrans_result.ok and vectrans_result.candidate_code:
            return self._finalize("pipeline1", outdir, vectrans_result, steps)

        compiler_result = self.compiler.run(source_file=source_file, outdir=outdir / "compiler")
        steps.append(compiler_result)
        return self._finalize("pipeline1", outdir, compiler_result, steps)

    def run_pipeline2(self, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        outdir.mkdir(parents=True, exist_ok=True)

        source_code = source_file.read_text()
        steps: list[TransformResult] = []

        vectrans_result = self.vectrans.run(
            source_code=source_code,
            outdir=outdir / "vectrans",
            max_rounds=int(os.environ.get("VECTRANS_MAX_ROUNDS", "6")),
        )
        steps.append(vectrans_result)

        if not vectrans_result.ok or not vectrans_result.candidate_code:
            compiler_result = self.compiler.run(source_file=source_file, outdir=outdir / "compiler")
            steps.append(compiler_result)
            return self._finalize("pipeline2", outdir, compiler_result, steps)

        llmvec_input = outdir / "llmvec" / "vectrans_preprocessed_input.c"
        llmvec_input.parent.mkdir(parents=True, exist_ok=True)
        llmvec_input.write_text(vectrans_result.candidate_code + "\n")

        llmvec_result = self.llmvec.run(
            source_file=llmvec_input,
            scalar_function=scalar_function,
            outdir=outdir / "llmvec",
            max_rounds=int(os.environ.get("LLM_VECTORIZER_MAX_ROUNDS", "6")),
            pipeline_context="vectrans_preprocessed",
        )
        steps.append(llmvec_result)

        if llmvec_result.ok and llmvec_result.candidate_code:
            return self._finalize("pipeline2", outdir, llmvec_result, steps)

        return self._finalize("pipeline2", outdir, vectrans_result, steps)