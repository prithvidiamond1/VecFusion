#!/usr/bin/env bash
set -u

ROOT="/Users/torence/VecTrans/self-refine"
PERF_REPEATS="${PERF_REPEATS:-5}"
PIPELINE="pipeline2"

cd "$ROOT" || exit 1
source .venv/bin/activate
source set_env.sh

OUT_ROOT="results/all_tests_${PIPELINE}_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$OUT_ROOT"

TESTS=()
while IFS= read -r test; do
  TESTS+=("$test")
done < <(find test_case -maxdepth 1 -name "*.c" -print | sed 's#.*/##' | sed 's#\.c$##' | sort)

echo "Running ${#TESTS[@]} tests for $PIPELINE into: $OUT_ROOT"
echo

for test in "${TESTS[@]}"; do
  echo "=================================================="
  echo "Running $test on $PIPELINE"
  echo "Output -> $OUT_ROOT/${test}_${PIPELINE}"
  echo "=================================================="

  if ! PYTHONPATH=. python3 -m src.integration.cli "test_case/${test}.c" \
    --scalar-function "$test" \
    --pipeline "$PIPELINE" \
    --vectrans-root . \
    --llmvec-root ./src/llm-vectorizer \
    --outdir "$OUT_ROOT/${test}_${PIPELINE}"; then
    echo "Run command failed for $test / $PIPELINE"
  fi

  if [ -f "$OUT_ROOT/${test}_${PIPELINE}/summary.json" ]; then
    python3 - <<PY
import json
from pathlib import Path
p = Path("$OUT_ROOT/${test}_${PIPELINE}/summary.json")
data = json.loads(p.read_text())
print({
    "test": "$test",
    "pipeline": "$PIPELINE",
    "success": data.get("success"),
    "final_stage": data.get("final_stage"),
    "final_code_path": data.get("final_code_path"),
})
PY
  else
    echo "No summary.json found for $test / $PIPELINE"
  fi

  echo
done

python scripts/compare_perf_all.py \
  --root "$ROOT" \
  --result-root "$OUT_ROOT" \
  --repeats "$PERF_REPEATS"

echo
echo "All runs finished."
echo "Results saved under: $OUT_ROOT"
echo "Performance CSV: $OUT_ROOT/performance_vs_o3.csv"