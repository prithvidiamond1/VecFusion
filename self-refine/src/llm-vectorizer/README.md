# Simple LLM Vectorizer

This project is a minimal prototype of an LLM-driven loop vectorizer for simple C kernels.

It does three things:

1. Reads a scalar C kernel.
2. Asks Claude to generate a vectorized version.
3. Compiles and tests the generated code against the scalar version with randomized inputs.

This is intentionally much simpler than the full paper system. It is meant to be easy to run and demo.

## Files

- `llmVectorizer.py`: main script
- `.env`: local API key file
- `example_kernel.c`: simple example kernel
- `kernel_mul_add.c`, `kernel_max_pair.c`, `kernel_absdiff.c`, `kernel_clamp_sum.c`: extra test kernels
- `test_llmVectorizer.py`: unit tests for the local pipeline

## Requirements

- Python 3.9+
- `clang`
- Python package `requests`
- A valid Anthropic API key

The script was tested on macOS with Apple clang.

## Setup

Put your Anthropic key in `.env`:

```bash
ANTHROPIC_API_KEY=your_real_key_here
```

The script loads `.env` automatically from the project directory.

## Run

Basic example:

```bash
python3 llmVectorizer.py example_kernel.c --scalar-function saxpy_like
```

Other examples:

```bash
python3 llmVectorizer.py kernel_mul_add.c --scalar-function mul_add
python3 llmVectorizer.py kernel_max_pair.c --scalar-function max_pair
python3 llmVectorizer.py kernel_absdiff.c --scalar-function absdiff
python3 llmVectorizer.py kernel_clamp_sum.c --scalar-function clamp_sum
```

Useful options:

```bash
python3 llmVectorizer.py example_kernel.c --scalar-function saxpy_like --max-rounds 4
python3 llmVectorizer.py example_kernel.c --scalar-function saxpy_like --dry-run
python3 llmVectorizer.py example_kernel.c --scalar-function saxpy_like --model claude-sonnet-4-6
```

## What The Script Expects

The current harness is intentionally narrow. It expects kernels with this shape:

```c
void function_name(const int *a, const int *b, int *out, int n)
```

The generated function must have the same signature, renamed to:

```c
vectorized_<function_name>
```

This version is best suited for simple elementwise integer array kernels.

## How It Works

For each round:

1. The script sends the scalar function to Claude.
2. Claude returns candidate C code for `vectorized_<function_name>`.
3. The script builds a temporary C harness containing:
   - the original scalar function
   - the generated vectorized function
   - randomized test input generation
   - exact output comparison and checksum reporting
4. The harness is compiled with `clang`.
5. If the outputs differ, the failure report is sent back to Claude for another attempt.

If a candidate passes all tests, the script prints the successful generated code.

## Checking That It Works

Dry-run the prompts:

```bash
python3 llmVectorizer.py example_kernel.c --scalar-function saxpy_like --dry-run
```

Run the local unit tests:

```bash
python3 -m unittest -v test_llmVectorizer.py
```

Optional syntax check:

```bash
PYTHONPYCACHEPREFIX=/tmp/pycache python3 -m py_compile llmVectorizer.py test_llmVectorizer.py
```

## Example Success Output

When it works, you should see output like:

```text
[round 1] candidate=...
Compile succeeded for round 1.
Exit code: 0
stdout:
PASS trials=64 checksum=...

=== SUCCESSFUL CANDIDATE ===
...
```

## Limitations

- Only supports a narrow kernel signature right now
- Only tests integer array kernels
- Uses randomized testing, not formal verification
- Not a full reproduction of the paper pipeline
- No Alive2 integration
- No generalized benchmark ingestion

## Notes

- `.env` is ignored by git through `.gitignore`
- The current default model is `claude-sonnet-4-6`
- If your Anthropic account exposes a different model, pass it with `--model`
