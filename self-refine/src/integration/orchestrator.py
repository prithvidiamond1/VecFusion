from pathlib import Path

from .adapters import CompilerBaselineAdapter, LLMVectorizerAdapter, VecTransAdapter
from .types import PipelineResult
from .vectrans_only_adapter import VecTransOnlyAdapter


class PipelineOrchestrator:
    """VecTrans-owned orchestration entry point for all combined pipelines."""

    def __init__(self, vectrans_root: Path, llmvec_root: Path) -> None:
        self.vectrans = VecTransAdapter(vectrans_root)
        self.vectrans_only = VecTransOnlyAdapter(vectrans_root)
        self.llmvec = LLMVectorizerAdapter(llmvec_root)
        self.compiler = CompilerBaselineAdapter()

    def run(self, pipeline: str, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        if pipeline == "pipeline1":
            return self.run_pipeline1(source_file, scalar_function, outdir)
        if pipeline == "pipeline2":
            return self.run_pipeline2(source_file, scalar_function, outdir)
        if pipeline == "vectrans_only":
            return self.run_vectrans_only(source_file, scalar_function, outdir)
        raise ValueError(f"Unknown pipeline: {pipeline}")

    def run_vectrans_only(self, source_file: Path, scalar_function: str, outdir: Path) -> PipelineResult:
        import json

        outdir.mkdir(parents=True, exist_ok=True)

        vectrans_result = self.vectrans_only.run(
            source_file=source_file,
            outdir=outdir / "vectrans_only",
        )

        steps = [
            {
                "stage": "vectrans_only",
                "ok": vectrans_result.ok,
                "candidate_code": vectrans_result.candidate_code,
                "summary": getattr(vectrans_result, "summary", ""),
                "artifact_path": getattr(vectrans_result, "artifact_path", ""),
                "error": getattr(vectrans_result, "error", None),
            }
        ]

        if vectrans_result.ok and vectrans_result.candidate_code:
            final_transform = self.compiler.evaluate_candidate(
                source_file=source_file,
                scalar_function=scalar_function,
                candidate_code=vectrans_result.candidate_code,
                outdir=outdir,
                pipeline_name="vectrans_only",
                steps=steps,
                final_stage="vectrans_only",
            )
        else:
            final_transform = self.compiler.run(
                source_file=source_file,
                outdir=outdir,
            )
            steps.append(
                {
                    "stage": final_transform.stage,
                    "ok": final_transform.ok,
                    "candidate_code": final_transform.candidate_code,
                    "summary": getattr(final_transform, "summary", ""),
                    "artifact_path": getattr(final_transform, "artifact_path", ""),
                    "error": None,
                }
            )

        summary_path = outdir / "summary.json"
        summary_payload = {
            "pipeline": "vectrans_only",
            "success": final_transform.ok,
            "final_stage": final_transform.stage,
            "final_code_path": final_transform.artifact_path,
            "steps": steps,
        }
        summary_path.write_text(json.dumps(summary_payload, indent=2))

        return PipelineResult(
            pipeline="vectrans_only",
            success=final_transform.ok,
            final_stage=final_transform.stage,
            final_code_path=final_transform.artifact_path,
            summary_path=str(summary_path),
            steps=steps,
        )