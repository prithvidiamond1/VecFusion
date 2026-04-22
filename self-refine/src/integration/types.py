from __future__ import annotations

from dataclasses import asdict, dataclass, field
from pathlib import Path
import json
from typing import Any


@dataclass
class TransformResult:
    stage: str
    ok: bool
    candidate_code: str = ""
    summary: str = ""
    compile_feedback: str = ""
    unit_test_output: str = ""
    unit_test_error: str = ""
    formal_verification: str = ""
    rounds_used: int = 0
    artifact_path: str = ""
    metadata: dict[str, Any] = field(default_factory=dict)

    def to_dict(self) -> dict:
        return asdict(self)


@dataclass
class PipelineResult:
    pipeline: str
    success: bool
    final_stage: str
    final_code_path: str
    summary_path: str
    steps: list[TransformResult]

    def write_summary(self, outdir: Path) -> Path:
        outdir.mkdir(parents=True, exist_ok=True)
        summary_path = outdir / "summary.json"
        payload = {
            "pipeline": self.pipeline,
            "success": self.success,
            "final_stage": self.final_stage,
            "final_code_path": self.final_code_path,
            "steps": [step.to_dict() for step in self.steps],
        }
        summary_path.write_text(json.dumps(payload, indent=2))
        return summary_path
