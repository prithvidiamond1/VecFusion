#!/usr/bin/env bash
set -euo pipefail

ROOT="/Users/torence/VecTrans/self-refine"
TEST_DIR="$ROOT/test_case"
OUT_ROOT="$ROOT/results/batch_first3"

# Change this to the real path of your env file
ENV_FILE="/FULL/PATH/TO/set.env.sh"

cd "$ROOT"

source "$ROOT/.venv/bin/activate"
source "$ENV_FILE"

mkdir -p "$OUT_ROOT"

echo "Using test files from: $TEST_DIR"
echo "Writing results to:    $OUT_ROOT"
echo

# First 3 tests alphabetically
mapfile -t TESTS < <(find "$TEST_DIR" -maxdepth 1 -name "*.c" | sort | head -n 3)

if [ "${#TESTS[@]}" -eq 0 ]; then
  echo "No .c test files found in $TEST_DIR"
  exit 1
fi

for test_file in "${TESTS[@]}"; do
  base="$(basename "$test_file" .c)"
  scalar_function="$base"

  echo "=================================================="
  echo "Running test: $base"
  echo "File: $test_file"
  echo "Function: $scalar_function"
  echo "=================================================="

  for pipeline in pipeline1 pipeline2; do
    outdir="$OUT_ROOT/${base}_${pipeline}"

    echo
    echo ">>> Running $pipeline on $base"
    PYTHONPATH=. python3 -m src.integration.cli "$test_file" \
      --scalar-function "$scalar_function" \
      --pipeline "$pipeline" \
      --vectrans-root . \
      --llmvec-root ./src/llm-vectorizer \
      --outdir "$outdir" || true

    if [ -f "$outdir/summary.json" ]; then
      echo "--- summary for $base / $pipeline ---"
      python3 - <<PY
import json, pathlib
p = pathlib.Path("$outdir/summary.json")
data = json.loads(p.read_text())
print("pipeline   :", data.get("pipeline"))
print("success    :", data.get("success"))
print("final_stage:", data.get("final_stage"))
print("final_code :", data.get("final_code_path"))
PY
    else
      echo "No summary.json found for $base / $pipeline"
    fi
  done

  echo
done

echo "Done."