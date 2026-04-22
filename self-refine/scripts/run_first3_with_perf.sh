#!/usr/bin/env bash
set -euo pipefail

ROOT="/Users/torence/VecTrans/self-refine"
TESTS=(AllPointsEq s1113 s112)

cd "$ROOT"
source .venv/bin/activate
source set_env.sh

OUT_ROOT="results/batch_first3_rerun_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$OUT_ROOT"

for test in "${TESTS[@]}"; do
  for pipeline in pipeline1 pipeline2; do
    echo "=================================================="
    echo "Running $test on $pipeline"
    echo "Output -> $OUT_ROOT/${test}_${pipeline}"
    echo "=================================================="

    PYTHONPATH=. python3 -m src.integration.cli "test_case/${test}.c" \
      --scalar-function "$test" \
      --pipeline "$pipeline" \
      --vectrans-root . \
      --llmvec-root ./src/llm-vectorizer \
      --outdir "$OUT_ROOT/${test}_${pipeline}"
  done
done

python scripts/compare_perf_first3.py \
  --root "$ROOT" \
  --result-root "$OUT_ROOT" \
  --tests "${TESTS[@]}"

echo
echo "All runs finished."
echo "Results saved under: $OUT_ROOT"
echo "Performance CSV: $OUT_ROOT/performance_vs_o3.csv"