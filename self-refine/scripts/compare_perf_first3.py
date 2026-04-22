#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
import statistics
import subprocess
import tempfile
import textwrap
from pathlib import Path


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
        nargs="+",
        default=["AllPointsEq", "s1113", "s112"],
        help="Test names to compare",
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
        default=15,
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


def rename_function(source_text: str, old_name: str, new_name: str) -> str:
    return re.sub(rf"\b{re.escape(old_name)}\b", new_name, source_text)


def detect_candidate_function(test_name: str, candidate_source: str) -> str:
    candidates = [
        f"vectorized_{test_name}",
        f"{test_name}_opt",
        test_name,
    ]
    for fn in candidates:
        if re.search(rf"\b{re.escape(fn)}\s*\(", candidate_source):
            return fn
    raise ValueError(f"Could not detect callable candidate function for {test_name}")


def build_driver(
    test_name: str,
    original_source: str,
    candidate_source: str,
    baseline_fn: str,
    candidate_fn: str,
) -> str:
    if test_name == "s112":
        return textwrap.dedent(
            f"""
            #include <stdio.h>
            #include <stdlib.h>
            #include <string.h>
            #include <math.h>
            #include <time.h>

            {original_source}

            {candidate_source}

            static float frandv(unsigned *s) {{
                *s = *s * 1664525u + 1013904223u;
                return ((float)(*s % 2001u) - 1000.0f) / 17.0f;
            }}

            static double run_original(int reps) {{
                const int LEN_1D = 1024;
                const int iterations = 50;
                unsigned seed = 7u;
                float *a = (float*)malloc(LEN_1D * sizeof(float));
                float *b = (float*)malloc(LEN_1D * sizeof(float));

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < LEN_1D; ++i) {{
                        a[i] = frandv(&seed);
                        b[i] = frandv(&seed);
                    }}
                    {baseline_fn}(a, b, iterations, LEN_1D);
                }}
                clock_t t1 = clock();
                free(a);
                free(b);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            static double run_candidate(int reps) {{
                const int LEN_1D = 1024;
                const int iterations = 50;
                unsigned seed = 7u;
                float *a = (float*)malloc(LEN_1D * sizeof(float));
                float *b = (float*)malloc(LEN_1D * sizeof(float));

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < LEN_1D; ++i) {{
                        a[i] = frandv(&seed);
                        b[i] = frandv(&seed);
                    }}
                    {candidate_fn}(a, b, iterations, LEN_1D);
                }}
                clock_t t1 = clock();
                free(a);
                free(b);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            int main(void) {{
                const int reps = 400;
                printf("orig=%f\\n", run_original(reps));
                printf("cand=%f\\n", run_candidate(reps));
                return 0;
            }}
            """
        ).strip() + "\n"

    if test_name == "s1113":
        return textwrap.dedent(
            f"""
            #include <stdio.h>
            #include <stdlib.h>
            #include <string.h>
            #include <math.h>
            #include <time.h>

            {original_source}

            {candidate_source}

            static float frandv(unsigned *s) {{
                *s = *s * 1664525u + 1013904223u;
                return ((float)(*s % 2001u) - 1000.0f) / 17.0f;
            }}

            static double run_original(int reps) {{
                const int LEN_1D = 1024;
                const int iterations = 50;
                unsigned seed = 7u;
                float *a = (float*)malloc(LEN_1D * sizeof(float));
                float *b = (float*)malloc(LEN_1D * sizeof(float));

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < LEN_1D; ++i) {{
                        a[i] = frandv(&seed);
                        b[i] = frandv(&seed);
                    }}
                    {baseline_fn}(iterations, LEN_1D, a, b);
                }}
                clock_t t1 = clock();
                free(a);
                free(b);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            static double run_candidate(int reps) {{
                const int LEN_1D = 1024;
                const int iterations = 50;
                unsigned seed = 7u;
                float *a = (float*)malloc(LEN_1D * sizeof(float));
                float *b = (float*)malloc(LEN_1D * sizeof(float));

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < LEN_1D; ++i) {{
                        a[i] = frandv(&seed);
                        b[i] = frandv(&seed);
                    }}
                    {candidate_fn}(iterations, LEN_1D, a, b);
                }}
                clock_t t1 = clock();
                free(a);
                free(b);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            int main(void) {{
                const int reps = 400;
                printf("orig=%f\\n", run_original(reps));
                printf("cand=%f\\n", run_candidate(reps));
                return 0;
            }}
            """
        ).strip() + "\n"

    if test_name == "AllPointsEq":
        return textwrap.dedent(
            f"""
            #include <stdio.h>
            #include <stdlib.h>
            #include <stdbool.h>
            #include <time.h>

            {original_source}

            {candidate_source}

            static int irandv(unsigned *s) {{
                *s = *s * 1664525u + 1013904223u;
                return (int)(*s % 2001u) - 1000;
            }}

            static double run_original(int reps) {{
                const int n = 4096;
                unsigned seed = 7u;
                int *pts = (int*)malloc(n * sizeof(int));
                volatile int sink = 0;

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < n; ++i) {{
                        pts[i] = irandv(&seed);
                    }}
                    sink += {baseline_fn}(pts, n);
                }}
                clock_t t1 = clock();
                free(pts);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            static double run_candidate(int reps) {{
                const int n = 4096;
                unsigned seed = 7u;
                int *pts = (int*)malloc(n * sizeof(int));
                volatile int sink = 0;

                clock_t t0 = clock();
                for (int r = 0; r < reps; ++r) {{
                    for (int i = 0; i < n; ++i) {{
                        pts[i] = irandv(&seed);
                    }}
                    sink += {candidate_fn}(pts, n);
                }}
                clock_t t1 = clock();
                free(pts);
                return (double)(t1 - t0) / CLOCKS_PER_SEC;
            }}

            int main(void) {{
                const int reps = 4000;
                printf("orig=%f\\n", run_original(reps));
                printf("cand=%f\\n", run_candidate(reps));
                return 0;
            }}
            """
        ).strip() + "\n"

    raise ValueError(f"No performance driver defined for {test_name}")


def time_pair(
    test_name: str,
    source_path: Path,
    candidate_path: Path,
    compiler: str,
) -> tuple[float, float]:
    src_text = source_path.read_text()
    cand_text = candidate_path.read_text()

    baseline_fn = f"scalar_baseline_{test_name}"
    renamed_original = rename_function(src_text, test_name, baseline_fn)
    candidate_fn = detect_candidate_function(test_name, cand_text)

    driver = build_driver(
        test_name=test_name,
        original_source=renamed_original,
        candidate_source=cand_text,
        baseline_fn=baseline_fn,
        candidate_fn=candidate_fn,
    )

    with tempfile.TemporaryDirectory() as td:
        cfile = Path(td) / "bench.c"
        exe = Path(td) / "bench"
        cfile.write_text(driver)

        subprocess.run(
            [compiler, str(cfile), "-O3", "-std=c11", "-Wall", "-Wextra", "-o", str(exe)],
            check=True,
        )

        out = subprocess.check_output([str(exe)], text=True)
        vals: dict[str, float] = {}
        for line in out.strip().splitlines():
            k, v = line.split("=")
            vals[k.strip()] = float(v.strip())

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


def main() -> None:
    args = parse_args()
    rows = []

    for test in args.tests:
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

            rows.append({
                "test": test,
                "pipeline": pipeline,
                "final_stage": summary.get("final_stage"),
                "o3_seconds": orig,
                "pipeline_seconds": cand,
                "speedup_vs_o3": speedup,
            })

    out_csv = args.result_root / "performance_vs_o3.csv"
    with out_csv.open("w") as f:
        f.write("test,pipeline,final_stage,o3_seconds,pipeline_seconds,speedup_vs_o3\n")
        for r in rows:
            f.write(
                f"{r['test']},{r['pipeline']},{r['final_stage']},"
                f"{r['o3_seconds']},{r['pipeline_seconds']},{r['speedup_vs_o3']}\n"
            )

    print(f"Wrote {out_csv}")
    for r in rows:
        print(r)


if __name__ == "__main__":
    main()