# VecFusion
VecFusion combines ideas from **[VecTrans](https://arxiv.org/abs/2503.19449)** and a basic recreation of **[LLM-Vectorizer](https://arxiv.org/abs/2406.04693)** into a unified auto-vectorization framework for improving compiler vectorization on loop kernels.

The repository currently supports two combined pipelines:

- **Pipeline 1:** LLM-Vectorizer first, then VecTrans, then compiler baseline fallback
- **Pipeline 2:** VecTrans first, then LLM-Vectorizer refinement, then compiler baseline fallback

The goal is to increase the number of correctly transformed kernels that can be verified and benefit from vectorization.

## Project Context
Developed as a course project for **CSE 583** at the **University of Michigan**.

## Team
- Torence Mwindaare  
- Prithviraj Prasad
- Roy Huang 
- Farhan Siddik
## Repository structure

```text
src/
  integration/
    cli.py               # command-line entry point
    orchestrator.py      # pipeline orchestration
    adapters.py          # VecTrans / LLM-Vectorizer / compiler adapters
scripts/
  run_all_with_perf.sh   # run benchmark suite with timing
results/                 # output directories for per-run artifacts
```

## Prerequisites

- Python 3.10+
- Clang/LLVM toolchain
- A working C compiler environment
- Access to the LLM backend(s) you plan to use
- The VecTrans and LLM-Vectorizer codebases available locally if this repo is still structured as an integration layer over both

Depending on your setup, you may also need:

- `SDKROOT` configured on macOS
- API credentials or base URLs for your selected LLM provider
- Any verification tools used by your local workflow

## Installation

Clone the repository and create a virtual environment:

```bash
git clone <your-public-repo-url>
cd VecFusion
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

If your environment depends on local paths to VecTrans and LLM-Vectorizer, make sure those paths are available and passed into the CLI when running experiments.

## Running a single benchmark

Use the integration CLI to run one source file through a chosen pipeline:

```bash
PYTHONPATH=. python3 -m src.integration.cli \
  <source_file.c> \
  --scalar-function <function_name> \
  --pipeline pipeline1 \
  --vectrans-root <path-to-vectrans> \
  --llmvec-root <path-to-llm-vectorizer> \
  --outdir results/<run_name>
```

Example:

```bash
PYTHONPATH=. python3 -m src.integration.cli \
  tsvc/cfiles/s112.c \
  --scalar-function s112 \
  --pipeline pipeline2 \
  --vectrans-root ../VecTrans \
  --llmvec-root ../llm-vectorizer \
  --outdir results/s112_pipeline2
```

## Running the benchmark suite

To run the full suite with performance measurement:

```bash
PERF_REPEATS=5 scripts/run_all_with_perf.sh
```

If your script supports pipeline filtering, you can run only one pipeline by passing the corresponding option in your local version.

## Output

Each run writes artifacts to the selected output directory, typically including:

- generated candidate code
- stage-by-stage intermediate outputs
- validation or verification diagnostics
- a `summary.json` file describing the final result

Typical summary fields include:

- `pipeline`
- `success`
- `final_stage`
- `final_code_path`
- `steps`

## Notes

This project builds on **VecTrans** and related tooling. If you redistribute code derived from upstream Apache-licensed components, keep the required notices and license text in the repository.

## License

This repository is distributed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for the full text.
