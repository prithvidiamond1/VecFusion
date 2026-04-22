from __future__ import annotations

import argparse
import json
from pathlib import Path

from .orchestrator import PipelineOrchestrator


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run VecTrans-owned combined vectorization pipelines.")
    parser.add_argument("source", type=Path, help="Path to the C source file.")
    parser.add_argument("--scalar-function", required=True, help="Scalar function name for LLM-Vectorizer stages.")
    parser.add_argument("--pipeline", choices=["pipeline1", "pipeline2", "vectrans_only"], required=True)
    parser.add_argument("--vectrans-root", type=Path, required=True)
    parser.add_argument("--llmvec-root", type=Path, required=True)
    parser.add_argument("--outdir", type=Path, required=True)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    orchestrator = PipelineOrchestrator(args.vectrans_root, args.llmvec_root)
    result = orchestrator.run(
        pipeline=args.pipeline,
        source_file=args.source,
        scalar_function=args.scalar_function,
        outdir=args.outdir,
    )
    print(json.dumps(
        {
            "pipeline": result.pipeline,
            "success": result.success,
            "final_stage": result.final_stage,
            "final_code_path": result.final_code_path,
            "summary_path": result.summary_path,
        },
        indent=2,
    ))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
