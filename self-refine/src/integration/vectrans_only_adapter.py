import os
from pathlib import Path
from types import SimpleNamespace

from src.vectorizer.run_split import run_file


class VecTransOnlyAdapter:
    def __init__(self, vectrans_root: Path) -> None:
        self.vectrans_root = vectrans_root.resolve()

    def run(self, source_file: Path, outdir: Path):
        outdir.mkdir(parents=True, exist_ok=True)
        log_dir = outdir / "vectrans_logs"
        log_dir.mkdir(parents=True, exist_ok=True)

        prev_cwd = Path.cwd()
        try:
            os.chdir(self.vectrans_root)

            candidate_code = run_file(
                source_file=source_file.resolve(),
                max_attempts=int(os.getenv("VECTRANS_MAX_ATTEMPTS", "20")),
                log_dir=log_dir,
            )
        except Exception as e:
            return SimpleNamespace(
                ok=False,
                candidate_code=None,
                error=f"VecTrans standalone failed: {e}",
            )
        finally:
            os.chdir(prev_cwd)

        if candidate_code is None:
            return SimpleNamespace(
                ok=False,
                candidate_code=None,
                error="VecTrans produced no verified final candidate",
            )

        candidate_path = outdir / "vectrans_final.c"
        candidate_path.write_text(candidate_code)

        return SimpleNamespace(
            ok=True,
            candidate_code=candidate_code,
            error=None,
        )
