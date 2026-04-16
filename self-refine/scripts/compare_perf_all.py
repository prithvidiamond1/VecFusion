#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import math
import re
import statistics
import subprocess
import tempfile
import textwrap
from dataclasses import dataclass
from pathlib import Path


@dataclass
class ParamSpec:
    raw: str
    name: str
    base_type: str
    is_const: bool
    is_pointer: bool
    is_array: bool
    array_dims: list[str]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--root",
        type=Path,
        default=Path("/Users/torence/VecTrans/self-refine"),
        help="Repo root",
    )
    parser.add_argument(
        "--result-root",
        type=Path,
        required=True,
        help="Directory containing per-test pipeline result folders",
    )
    parser.add_argument(
        "--tests",
        nargs="*",
        default=None,
        help="Optional explicit test names. Defaults to every .c file in test_case.",
    )
    parser.add_argument(
        "--pipelines",
        nargs="+",
        default=["pipeline1", "pipeline2"],
        help="Pipelines to compare",
    )
    parser.add_argument(
        "--repeats",
        type=int,
        default=5,
        help="Median over this many runs",
    )
    parser.add_argument(
        "--compiler",
        default="clang",
        help="Compiler to use",
    )
    return parser.parse_args()


def load_summary(path: Path) -> dict:
    return json.loads(path.read_text())


def split_params(param_text: str) -> list[str]:
    parts: list[str] = []
    cur: list[str] = []
    depth = 0
    for ch in param_text:
        if ch == "," and depth == 0:
            piece = "".join(cur).strip()
            if piece:
                parts.append(piece)
            cur = []
            continue
        if ch in "([<":
            depth += 1
        elif ch in ")]>":
            depth -= 1
        cur.append(ch)
    piece = "".join(cur).strip()
    if piece:
        parts.append(piece)
    return parts


def parse_param(raw: str) -> ParamSpec:
    text = raw.strip()
    dims = re.findall(r"\[([^\]]*)\]", text)
    is_array = len(dims) > 0
    no_arrays = re.sub(r"\[[^\]]*\]", " ", text)
    toks = no_arrays.replace("*", " * ").split()

    if len(toks) < 2:
        raise ValueError(f"Could not parse parameter: {raw}")

    name = toks[-1]
    left = toks[:-1]
    is_pointer = ("*" in left) or is_array
    is_const = "const" in left
    base_tokens = [t for t in left if t not in {"*", "const", "restrict", "volatile"}]
    base_type = " ".join(base_tokens).strip()

    if not base_type:
        raise ValueError(f"Could not determine base type for parameter: {raw}")

    return ParamSpec(
        raw=raw,
        name=name,
        base_type=base_type,
        is_const=is_const,
        is_pointer=is_pointer,
        is_array=is_array,
        array_dims=dims,
    )


def extract_function_signature(source_text: str, fn_name: str) -> tuple[str, list[ParamSpec]]:
    pattern = re.compile(
        rf"\b(?P<ret>[A-Za-z_][\w\s\*]*?)\s+{re.escape(fn_name)}\s*\((?P<params>.*?)\)\s*\{{",
        re.DOTALL,
    )
    m = pattern.search(source_text)
    if not m:
        raise ValueError(f"Could not extract signature for {fn_name}")

    ret_type = " ".join(m.group("ret").split())
    params_text = m.group("params").strip()
    if not params_text or params_text == "void":
        return ret_type, []

    params = [parse_param(p) for p in split_params(params_text)]
    return ret_type, params


def rename_function(source_text: str, old_name: str, new_name: str) -> str:
    return re.sub(rf"\b{re.escape(old_name)}\b", new_name, source_text)


def detect_candidate_function(test_name: str, candidate_source: str) -> str:
    for fn in (f"vectorized_{test_name}", f"{test_name}_opt", test_name):
        if re.search(rf"\b{re.escape(fn)}\s*\(", candidate_source):
            return fn
    raise ValueError(f"Could not detect candidate entry function for {test_name}")


def is_float_type(t: str) -> bool:
    t = t.strip()
    return "float" in t and "double" not in t


def is_double_type(t: str) -> bool:
    return "double" in t.strip()


def is_bool_type(t: str) -> bool:
    return t.strip() == "bool"


def is_int_like_type(t: str) -> bool:
    t = t.strip()
    return t in {"int", "long", "short", "unsigned", "unsigned int", "size_t", "ptrdiff_t"} or "int" in t


def scalar_value_for(name: str) -> str:
    lower = name.lower()
    if "iter" in lower:
        return "50"
    if lower in {"n", "m", "count", "len", "len_1d"} or "len" in lower or "count" in lower:
        return "n"
    if "mid" in lower:
        return "n / 2"
    return "7"


def pointer_length_for(param: ParamSpec) -> str:
    lower = param.name.lower()
    if "flat_2d" in lower or "2d" in lower:
        return "MATRIX_DIM * MATRIX_DIM"
    return "n"


def array_decl(param: ParamSpec, prefix: str = "") -> str:
    dims = "".join(f"[{d if d else 'MATRIX_DIM'}]" for d in param.array_dims)
    return f"{param.base_type} {prefix}{param.name}{dims};"


def nested_fill_code(param: ParamSpec, target_name: str) -> str:
    dims = [d if d else "MATRIX_DIM" for d in param.array_dims]
    idxs = [f"i{k}" for k in range(len(dims))]
    fill_fn = "fill_i32"
    if is_float_type(param.base_type):
        fill_fn = "fill_f32"
    elif is_double_type(param.base_type):
        fill_fn = "fill_f64"

    lines: list[str] = []
    indent = ""
    for idx, dim in zip(idxs, dims):
        lines.append(f"{indent}for (int {idx} = 0; {idx} < {dim}; ++{idx}) {{")
        indent += "    "
    access = "".join(f"[{idx}]" for idx in idxs)
    if fill_fn == "fill_i32":
        value = "next_i32(&seed)"
    elif fill_fn == "fill_f32":
        value = "next_f32(&seed)"
    else:
        value = "next_f64(&seed)"
    lines.append(f"{indent}{target_name}{access} = {value};")
    for _ in idxs:
        indent = indent[:-4]
        lines.append(f"{indent}}}")
    return "\n".join(lines)


def sample_expr(param: ParamSpec, target_name: str) -> str:
    if param.is_array:
        dims = "".join("[0]" for _ in param.array_dims)
        return f"{target_name}{dims}"
    return f"{target_name}[0]"


def build_run_function(
    run_name: str,
    call_name: str,
    ret_type: str,
    params: list[ParamSpec],
) -> str:
    pre_loop_decls: list[str] = [
        "const int n = VECTOR_LEN;",
        "const int MATRIX_DIM = 256;",
        "unsigned seed = 7u;",
    ]
    loop_setup: list[str] = []
    post_loop_frees: list[str] = []
    call_args: list[str] = []
    sink_stmt = ""
    first_mutable_sample: tuple[str, str] | None = None

    for p in params:
        if p.is_array:
            pre_loop_decls.append(array_decl(p))
            loop_setup.append(nested_fill_code(p, p.name))
            call_args.append(p.name)
            if not p.is_const and first_mutable_sample is None:
                first_mutable_sample = (p.base_type, sample_expr(p, p.name))

        elif p.is_pointer:
            length_expr = pointer_length_for(p)
            pre_loop_decls.append(
                f"{p.base_type} *{p.name} = ({p.base_type}*)malloc(({length_expr}) * sizeof({p.base_type}));"
            )
            pre_loop_decls.append(
                f'if (!{p.name}) {{ fprintf(stderr, "malloc failed for {p.name}\\n"); exit(1); }}'
            )
            fill_fn = "fill_i32"
            if is_float_type(p.base_type):
                fill_fn = "fill_f32"
            elif is_double_type(p.base_type):
                fill_fn = "fill_f64"
            loop_setup.append(f"{fill_fn}({p.name}, {length_expr}, &seed);")
            post_loop_frees.append(f"free({p.name});")
            call_args.append(p.name)
            if not p.is_const and first_mutable_sample is None:
                first_mutable_sample = (p.base_type, sample_expr(p, p.name))

        else:
            pre_loop_decls.append(f"{p.base_type} {p.name} = {scalar_value_for(p.name)};")
            call_args.append(p.name)

    call_expr = f"{call_name}({', '.join(call_args)})"

    if ret_type != "void":
        if is_bool_type(ret_type) or is_int_like_type(ret_type):
            sink_stmt = f"perf_sink_i64 += (long long){call_expr};"
        elif is_float_type(ret_type) or is_double_type(ret_type):
            sink_stmt = f"perf_sink_f64 += (double){call_expr};"
        else:
            sink_stmt = f"(void){call_expr};"
    else:
        call_stmt = f"{call_expr};"
        if first_mutable_sample is not None:
            sample_type, expr = first_mutable_sample
            if is_float_type(sample_type) or is_double_type(sample_type):
                sink_stmt = call_stmt + f"\n        perf_sink_f64 += (double)({expr});"
            else:
                sink_stmt = call_stmt + f"\n        perf_sink_i64 += (long long)({expr});"
        else:
            sink_stmt = call_stmt

    return textwrap.dedent(
        f"""
        static double {run_name}(int reps) {{
            {' '.join(pre_loop_decls)}

            clock_t t0 = clock();
            for (int r = 0; r < reps; ++r) {{
                {' '.join(loop_setup)}
                {sink_stmt}
            }}
            clock_t t1 = clock();

            {' '.join(post_loop_frees)}
            return (double)(t1 - t0) / CLOCKS_PER_SEC;
        }}
        """
    ).strip()


def build_driver(
    test_name: str,
    original_source: str,
    candidate_source: str,
    baseline_fn: str,
    candidate_fn: str,
    ret_type: str,
    params: list[ParamSpec],
) -> str:
    return textwrap.dedent(
        f"""
        #include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>
        #include <time.h>

        static volatile long long perf_sink_i64 = 0;
        static volatile double perf_sink_f64 = 0.0;

        static uint32_t next_u32(unsigned *state) {{
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }}

        static int next_i32(unsigned *state) {{
            return (int)(next_u32(state) % 2001u) - 1000;
        }}

        static float next_f32(unsigned *state) {{
            return ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
        }}

        static double next_f64(unsigned *state) {{
            return ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
        }}

        static void fill_i32(int *buf, int n, unsigned *state) {{
            for (int i = 0; i < n; ++i) {{
                buf[i] = next_i32(state);
            }}
        }}

        static void fill_f32(float *buf, int n, unsigned *state) {{
            for (int i = 0; i < n; ++i) {{
                buf[i] = next_f32(state);
            }}
        }}

        static void fill_f64(double *buf, int n, unsigned *state) {{
            for (int i = 0; i < n; ++i) {{
                buf[i] = next_f64(state);
            }}
        }}

        #define VECTOR_LEN 1024

        {original_source}

        {candidate_source}

        {build_run_function("run_original", baseline_fn, ret_type, params)}

        {build_run_function("run_candidate", candidate_fn, ret_type, params)}

        int main(void) {{
            const int reps = 200;
            printf("orig=%f\\n", run_original(reps));
            printf("cand=%f\\n", run_candidate(reps));
            if (perf_sink_i64 == -1) {{
                printf("sink=%lld\\n", perf_sink_i64);
            }}
            if (perf_sink_f64 == -1.0) {{
                printf("sinkf=%f\\n", perf_sink_f64);
            }}
            return 0;
        }}
        """
    ).strip() + "\n"


def time_pair(
    test_name: str,
    source_path: Path,
    candidate_path: Path,
    compiler: str,
) -> tuple[float, float]:
    src_text = source_path.read_text()
    cand_text = candidate_path.read_text()

    ret_type, params = extract_function_signature(src_text, test_name)

    baseline_fn = f"scalar_baseline_{test_name}"
    renamed_original = rename_function(src_text, test_name, baseline_fn)
    candidate_fn = detect_candidate_function(test_name, cand_text)

    driver = build_driver(
        test_name=test_name,
        original_source=renamed_original,
        candidate_source=cand_text,
        baseline_fn=baseline_fn,
        candidate_fn=candidate_fn,
        ret_type=ret_type,
        params=params,
    )

    with tempfile.TemporaryDirectory() as td:
        cfile = Path(td) / "bench.c"
        exe = Path(td) / "bench"
        cfile.write_text(driver)

        subprocess.run(
            [compiler, str(cfile), "-O3", "-std=c11", "-Wall", "-Wextra", "-o", str(exe)],
            check=True,
            capture_output=True,
            text=True,
        )

        out = subprocess.check_output([str(exe)], text=True)
        vals: dict[str, float] = {}
        for line in out.strip().splitlines():
            if "=" not in line:
                continue
            k, v = line.split("=", 1)
            try:
                vals[k.strip()] = float(v.strip())
            except ValueError:
                pass

        return vals["orig"], vals["cand"]


def median_timing(
    test_name: str,
    source_path: Path,
    candidate_path: Path,
    compiler: str,
    repeats: int,
) -> tuple[float, float]:
    origs: list[float] = []
    cands: list[float] = []

    for _ in range(repeats):
        o, c = time_pair(test_name, source_path, candidate_path, compiler)
        origs.append(o)
        cands.append(c)

    return statistics.median(origs), statistics.median(cands)


def geomean(vals: list[float]) -> float | None:
    vals = [v for v in vals if v is not None and v > 0]
    if not vals:
        return None
    return math.exp(sum(math.log(v) for v in vals) / len(vals))


def arithmetic_mean(vals: list[float]) -> float | None:
    vals = [v for v in vals if v is not None]
    if not vals:
        return None
    return sum(vals) / len(vals)


def median(vals: list[float]) -> float | None:
    vals = [v for v in vals if v is not None]
    if not vals:
        return None
    return statistics.median(vals)


def format_num(x: float | None) -> str:
    return "" if x is None else str(x)


def main() -> None:
    args = parse_args()

    if args.tests:
        tests = args.tests
    else:
        tests = sorted(p.stem for p in (args.root / "test_case").glob("*.c"))

    rows: list[dict] = []

    for test in tests:
        src = args.root / "test_case" / f"{test}.c"
        for pipeline in args.pipelines:
            summary_path = args.result_root / f"{test}_{pipeline}" / "summary.json"
            if not summary_path.exists():
                print(f"Skipping missing summary: {summary_path}")
                continue

            summary = load_summary(summary_path)
            final_code = args.root / summary["final_code_path"]

            if not final_code.exists():
                print(f"Skipping missing final code: {final_code}")
                continue

            try:
                orig, cand = median_timing(
                    test_name=test,
                    source_path=src,
                    candidate_path=final_code,
                    compiler=args.compiler,
                    repeats=args.repeats,
                )
                speedup = orig / cand if cand > 0 else None
            except Exception as exc:
                print(f"Timing failed for {test} / {pipeline}: {exc}")
                orig, cand, speedup = None, None, None

            verified = bool(summary.get("success"))
            final_stage = str(summary.get("final_stage"))

            rows.append({
                "test": test,
                "pipeline": pipeline,
                "final_stage": final_stage,
                "verified": verified,
                "o3_seconds": orig,
                "pipeline_seconds": cand,
                "speedup_vs_o3": speedup,
                "notes": "",
            })

    for pipeline in args.pipelines:
        subset = [r for r in rows if r["pipeline"] == pipeline]
        timed = [r for r in subset if r["speedup_vs_o3"] is not None]
        verified = [r for r in subset if r["verified"]]
        eligible_speedups = [
            r for r in timed
            if r["verified"] and r["final_stage"] != "compiler_baseline"
        ]
        speedups_all_verified = [r["speedup_vs_o3"] for r in timed if r["verified"]]
        successful_speedups = [r for r in eligible_speedups if r["speedup_vs_o3"] > 1.0]

        rows.append({
            "test": "__ARITH_MEAN_SPEEDUP__",
            "pipeline": pipeline,
            "final_stage": "aggregate",
            "verified": "",
            "o3_seconds": None,
            "pipeline_seconds": None,
            "speedup_vs_o3": arithmetic_mean(speedups_all_verified),
            "notes": f"verified_timed_cases={len(speedups_all_verified)}",
        })
        rows.append({
            "test": "__GEOMEAN_SPEEDUP__",
            "pipeline": pipeline,
            "final_stage": "aggregate",
            "verified": "",
            "o3_seconds": None,
            "pipeline_seconds": None,
            "speedup_vs_o3": geomean(speedups_all_verified),
            "notes": f"verified_timed_cases={len(speedups_all_verified)}",
        })
        rows.append({
            "test": "__MEDIAN_SPEEDUP__",
            "pipeline": pipeline,
            "final_stage": "aggregate",
            "verified": "",
            "o3_seconds": None,
            "pipeline_seconds": None,
            "speedup_vs_o3": median(speedups_all_verified),
            "notes": f"verified_timed_cases={len(speedups_all_verified)}",
        })
        rows.append({
            "test": "__SUCCESSFUL_SPEEDUPS__",
            "pipeline": pipeline,
            "final_stage": "aggregate",
            "verified": "",
            "o3_seconds": None,
            "pipeline_seconds": None,
            "speedup_vs_o3": len(successful_speedups),
            "notes": (
                f"{len(successful_speedups)}/{len(tests)} total_cases ; "
                f"{len(successful_speedups)}/{len(eligible_speedups)} eligible_verified_nonbaseline"
            ),
        })
        rows.append({
            "test": "__COVERAGE__",
            "pipeline": pipeline,
            "final_stage": "aggregate",
            "verified": "",
            "o3_seconds": None,
            "pipeline_seconds": None,
            "speedup_vs_o3": None,
            "notes": (
                f"timed={len(timed)}/{len(tests)} ; "
                f"verified={len(verified)}/{len(tests)} ; "
                f"eligible_verified_nonbaseline={len(eligible_speedups)}/{len(tests)}"
            ),
        })

    out_csv = args.result_root / "performance_vs_o3.csv"
    with out_csv.open("w") as f:
        f.write("test,pipeline,final_stage,verified,o3_seconds,pipeline_seconds,speedup_vs_o3,notes\n")
        for r in rows:
            f.write(
                f"{r['test']},{r['pipeline']},{r['final_stage']},{r['verified']},"
                f"{format_num(r['o3_seconds'])},{format_num(r['pipeline_seconds'])},"
                f"{format_num(r['speedup_vs_o3'])},{r['notes']}\n"
            )

    print(f"Wrote {out_csv}")
    for r in rows:
        print(r)


if __name__ == "__main__":
    main()