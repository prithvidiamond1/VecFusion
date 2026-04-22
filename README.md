# VecTrans + LLM-Vectorizer Integration

A research prototype that combines **VecTrans** (LLM-guided source transformation for improved vectorization) with **LLM-Vectorizer** (LLM-generated loop vectorization with verification) to improve compiler auto-vectorization coverage and performance on benchmark kernels.

## Overview

Modern compilers often miss vectorization opportunities due to aliasing, loop-carried dependencies, control flow complexity, or source patterns that obscure SIMD-friendly structure. This project builds combined execution pipelines that use LLM-based transformations and verification-driven vectorization before falling back to standard compiler optimization.

## Goals

- Increase the number of kernels that can be successfully vectorized.
- Preserve correctness through validation / verification.
- Improve runtime performance relative to compiler `-O3` baselines.
- Evaluate behavior on benchmark suites such as TSVC.
- Compare orchestration strategies across multiple pipelines.

## Core Components

### VecTrans
Transforms scalar source code into forms that are more amenable to vectorization.

Examples:
- Loop distribution
- Dependency-breaking rewrites
- Simplified memory access patterns
- Structure-preserving source refactors

### LLM-Vectorizer
Attempts direct loop vectorization using an LLM-guided generation/refinement loop.

Examples:
- SIMD-aware code generation
- Multi-round refinement
- Compiler feedback integration
- Correctness checking

### Compiler Baseline
Standard fallback path using compiler optimizations (for example `clang -O3`).

## Pipelines

## Pipeline 1 — LLM-Vectorizer First
Best when direct vectorization works immediately.

```text
Scalar Code
   ↓
LLM-Vectorizer
   ├─ success → Final Result
   └─ fail → VecTrans
              ├─ success → Final Result
              └─ fail → Compiler Baseline
```

## Pipeline 2 — VecTrans First
Best when preprocessing helps expose vectorization opportunities.

```text
Scalar Code
   ↓
VecTrans Candidate
   ├─ no candidate → Compiler Baseline
   └─ candidate → LLM-Vectorizer on Candidate
                     ├─ success → Final Result
                     └─ fail → Compiler Baseline
```

## Repository Structure

```text
self-refine/
├── src/
│   ├── integration/
│   │   ├── cli.py
│   │   ├── orchestrator.py
│   │   ├── adapters.py
│   │   └── types.py
│   ├── llm-vectorizer/
│   └── ...
├── tests/
│   └── tsvc/
├── scripts/
│   ├── run_all_with_perf.sh
│   └── ...
├── results/
└── README.md
```

## Running a Single Kernel

```bash
PYTHONPATH=. python3 -m src.integration.cli tests/tsvc/s281.c \
  --scalar-function s281 \
  --pipeline pipeline2 \
  --vectrans-root /path/to/self-refine \
  --llmvec-root /path/to/self-refine/src/llm-vectorizer \
  --outdir results/s281_pipeline2
```

## Run Pipeline 1 Only

```bash
PYTHONPATH=. python3 -m src.integration.cli tests/tsvc/s281.c \
  --scalar-function s281 \
  --pipeline pipeline1 \
  --vectrans-root /path/to/self-refine \
  --llmvec-root /path/to/self-refine/src/llm-vectorizer \
  --outdir results/s281_pipeline1
```

## Run Full Benchmark Suite

```bash
PIPELINES=pipeline2 scripts/run_all_with_perf.sh
```

Or:

```bash
PIPELINES=pipeline1 scripts/run_all_with_perf.sh
```

## Output Artifacts

Typical run outputs:

```text
results/<run_name>/
├── summary.json
├── timings.csv
├── generated_candidates/
├── compiler/
└── logs/
```

### summary.json
Contains:
- selected final stage
- per-stage success/failure
- candidate paths
- diagnostic metadata

## Evaluation Metrics

- **Coverage**: how many kernels verify successfully.
- **Speedup vs O3**: runtime improvement over compiler baseline.
- **Geometric Mean Speedup**: robust aggregate performance metric.
- **Median Speedup**: resistant to outliers.
- **Successful Speedups**: number of verified kernels faster than baseline.


## Requirements

Typical environment:
- Python 3.10+
- clang / LLVM toolchain
- macOS or Linux
- API access for supported LLM backends
- Optional: Alive2 for formal validation


## Acknowledgments

Built as a research exploration combining ideas from VecTrans, LLM-vectorizer, compiler optimization, and benchmark-driven evaluation.

