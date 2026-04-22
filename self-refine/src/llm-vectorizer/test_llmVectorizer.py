import io
import os
import tempfile
import unittest
from pathlib import Path
from unittest import mock

import llmVectorizer as lv


GOOD_CANDIDATE = """
void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) {
    int i = 0;
    for (; i + 3 < n; i += 4) {
        out[i] = a[i] + 2 * b[i];
        out[i + 1] = a[i + 1] + 2 * b[i + 1];
        out[i + 2] = a[i + 2] + 2 * b[i + 2];
        out[i + 3] = a[i + 3] + 2 * b[i + 3];
    }
    for (; i < n; ++i) {
        out[i] = a[i] + 2 * b[i];
    }
}
""".strip()

BAD_CANDIDATE = """
void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) {
    for (int i = 0; i < n; ++i) {
        out[i] = a[i] + b[i];
    }
}
""".strip()

KERNEL_CASES = [
    (
        "example_kernel.c",
        "saxpy_like",
        """
        void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) {
            int i = 0;
            for (; i + 3 < n; i += 4) {
                out[i] = a[i] + 2 * b[i];
                out[i + 1] = a[i + 1] + 2 * b[i + 1];
                out[i + 2] = a[i + 2] + 2 * b[i + 2];
                out[i + 3] = a[i + 3] + 2 * b[i + 3];
            }
            for (; i < n; ++i) {
                out[i] = a[i] + 2 * b[i];
            }
        }
        """.strip(),
    ),
    (
        "kernel_mul_add.c",
        "mul_add",
        """
        void vectorized_mul_add(const int *a, const int *b, int *out, int n) {
            int i = 0;
            for (; i + 3 < n; i += 4) {
                out[i] = (a[i] * 3) + b[i];
                out[i + 1] = (a[i + 1] * 3) + b[i + 1];
                out[i + 2] = (a[i + 2] * 3) + b[i + 2];
                out[i + 3] = (a[i + 3] * 3) + b[i + 3];
            }
            for (; i < n; ++i) {
                out[i] = (a[i] * 3) + b[i];
            }
        }
        """.strip(),
    ),
    (
        "kernel_max_pair.c",
        "max_pair",
        """
        void vectorized_max_pair(const int *a, const int *b, int *out, int n) {
            int i = 0;
            for (; i + 3 < n; i += 4) {
                out[i] = (a[i] > b[i]) ? a[i] : b[i];
                out[i + 1] = (a[i + 1] > b[i + 1]) ? a[i + 1] : b[i + 1];
                out[i + 2] = (a[i + 2] > b[i + 2]) ? a[i + 2] : b[i + 2];
                out[i + 3] = (a[i + 3] > b[i + 3]) ? a[i + 3] : b[i + 3];
            }
            for (; i < n; ++i) {
                out[i] = (a[i] > b[i]) ? a[i] : b[i];
            }
        }
        """.strip(),
    ),
    (
        "kernel_absdiff.c",
        "absdiff",
        """
        void vectorized_absdiff(const int *a, const int *b, int *out, int n) {
            int i = 0;
            for (; i + 3 < n; i += 4) {
                int d0 = a[i] - b[i];
                int d1 = a[i + 1] - b[i + 1];
                int d2 = a[i + 2] - b[i + 2];
                int d3 = a[i + 3] - b[i + 3];
                out[i] = (d0 < 0) ? -d0 : d0;
                out[i + 1] = (d1 < 0) ? -d1 : d1;
                out[i + 2] = (d2 < 0) ? -d2 : d2;
                out[i + 3] = (d3 < 0) ? -d3 : d3;
            }
            for (; i < n; ++i) {
                int diff = a[i] - b[i];
                out[i] = (diff < 0) ? -diff : diff;
            }
        }
        """.strip(),
    ),
    (
        "kernel_clamp_sum.c",
        "clamp_sum",
        """
        void vectorized_clamp_sum(const int *a, const int *b, int *out, int n) {
            int i = 0;
            for (; i + 3 < n; i += 4) {
                int s0 = a[i] + b[i];
                int s1 = a[i + 1] + b[i + 1];
                int s2 = a[i + 2] + b[i + 2];
                int s3 = a[i + 3] + b[i + 3];
                if (s0 > 500) s0 = 500; else if (s0 < -500) s0 = -500;
                if (s1 > 500) s1 = 500; else if (s1 < -500) s1 = -500;
                if (s2 > 500) s2 = 500; else if (s2 < -500) s2 = -500;
                if (s3 > 500) s3 = 500; else if (s3 < -500) s3 = -500;
                out[i] = s0;
                out[i + 1] = s1;
                out[i + 2] = s2;
                out[i + 3] = s3;
            }
            for (; i < n; ++i) {
                int sum = a[i] + b[i];
                if (sum > 500) {
                    sum = 500;
                } else if (sum < -500) {
                    sum = -500;
                }
                out[i] = sum;
            }
        }
        """.strip(),
    ),
]


class LLMVectorizerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.repo_root = Path(__file__).resolve().parent
        cls.scalar_source = (cls.repo_root / "example_kernel.c").read_text().strip()

    def make_config(self, work_dir: Path, dry_run: bool = True, source_name: str = "example_kernel.c", scalar_function: str = "saxpy_like") -> lv.RunConfig:
        return lv.RunConfig(
            source_path=self.repo_root / source_name,
            scalar_function=scalar_function,
            model="dummy-model",
            max_rounds=3,
            num_trials=8,
            array_len=16,
            random_seed=7,
            compiler="clang",
            compiler_flags=[],
            work_dir=work_dir,
            target_hint="portable SIMD-style C",
            dry_run=dry_run,
            api_base_url="https://api.anthropic.com",
            api_timeout=120,
        )

    def test_extract_code_block_prefers_fenced_code(self):
        text = "prefix\n```c\nint x;\n```\nsuffix"
        self.assertEqual(lv.extract_code_block(text), "int x;")

    def test_extract_code_block_falls_back_to_raw_text(self):
        self.assertEqual(lv.extract_code_block("int y;"), "int y;")

    def test_known_good_candidate_compiles_and_passes(self):
        with tempfile.TemporaryDirectory(prefix="llmvec_test_good_") as tmp:
            config = self.make_config(Path(tmp))
            result = lv.run_compile_and_tests(self.scalar_source, GOOD_CANDIDATE, config, 1)
            self.assertTrue(result.compile_ok, result.report)
            self.assertTrue(result.tests_ok, result.report)
            self.assertIn("PASS trials=", result.report)
            self.assertTrue(result.candidate_path.exists())

    def test_additional_kernel_candidates_compile_and_pass(self):
        for source_name, scalar_function, candidate in KERNEL_CASES:
            with self.subTest(kernel=source_name):
                with tempfile.TemporaryDirectory(prefix="llmvec_test_kernel_") as tmp:
                    scalar_source = (self.repo_root / source_name).read_text().strip()
                    config = self.make_config(Path(tmp), source_name=source_name, scalar_function=scalar_function)
                    result = lv.run_compile_and_tests(scalar_source, candidate, config, 1)
                    self.assertTrue(result.compile_ok, result.report)
                    self.assertTrue(result.tests_ok, result.report)

    def test_bad_candidate_compiles_but_fails_semantics(self):
        with tempfile.TemporaryDirectory(prefix="llmvec_test_bad_") as tmp:
            config = self.make_config(Path(tmp))
            result = lv.run_compile_and_tests(self.scalar_source, BAD_CANDIDATE, config, 2)
            self.assertTrue(result.compile_ok, result.report)
            self.assertFalse(result.tests_ok, result.report)
            self.assertIn("Mismatch on trial", result.report)
            self.assertIn("first_diff_index=", result.report)

    def test_compile_failure_is_reported(self):
        broken = "void vectorized_saxpy_like(const int *a, const int *b, int *out, int n) { BROKEN }"
        with tempfile.TemporaryDirectory(prefix="llmvec_test_compile_") as tmp:
            config = self.make_config(Path(tmp))
            result = lv.run_compile_and_tests(self.scalar_source, broken, config, 3)
            self.assertFalse(result.compile_ok)
            self.assertFalse(result.tests_ok)
            self.assertIn("Compile failed.", result.report)

    def test_get_api_key_requires_api_key(self):
        with mock.patch.dict(os.environ, {}, clear=True):
            with self.assertRaises(SystemExit) as exc:
                lv.get_api_key()
        self.assertIn("ANTHROPIC_API_KEY or CLAUDE_API_KEY is required", str(exc.exception))

    def test_load_dotenv_populates_missing_env_vars(self):
        with tempfile.TemporaryDirectory(prefix="llmvec_test_env_") as tmp:
            dotenv = Path(tmp) / ".env"
            dotenv.write_text("ANTHROPIC_API_KEY=dotenv-test-key\n")
            with mock.patch.dict(os.environ, {}, clear=True):
                lv.load_dotenv(dotenv)
                self.assertEqual(os.environ.get("ANTHROPIC_API_KEY"), "dotenv-test-key")

    def test_parse_args_reads_env_defaults_after_dotenv_load(self):
        with mock.patch.dict(os.environ, {"LLM_VECTORIZER_MODEL": "claude-test-model"}, clear=True):
            with mock.patch("sys.argv", ["llmVectorizer.py", "example_kernel.c", "--scalar-function", "saxpy_like"]):
                config = lv.parse_args()
        self.assertEqual(config.model, "claude-test-model")

    def test_main_dry_run_exits_cleanly(self):
        argv = [
            "llmVectorizer.py",
            "example_kernel.c",
            "--scalar-function",
            "saxpy_like",
            "--dry-run",
        ]
        stdout = io.StringIO()
        with mock.patch("sys.argv", argv), mock.patch("sys.stdout", stdout):
            rc = lv.main()
        self.assertEqual(rc, 0)
        out = stdout.getvalue()
        self.assertIn("== Vectorizer system prompt ==", out)
        self.assertIn("Produce a candidate function named `vectorized_saxpy_like`.", out)

    def test_main_succeeds_with_mocked_agent(self):
        argv = [
            "llmVectorizer.py",
            "example_kernel.c",
            "--scalar-function",
            "saxpy_like",
            "--work-dir",
            tempfile.mkdtemp(prefix="llmvec_test_main_ok_"),
        ]
        stdout = io.StringIO()
        with mock.patch("sys.argv", argv), mock.patch.dict(os.environ, {"ANTHROPIC_API_KEY": "test-key"}):
            with mock.patch.object(lv, "new_agent", side_effect=["vectorizer", "tester"]):
                with mock.patch.object(lv, "ask_agent", side_effect=[f"```c\n{GOOD_CANDIDATE}\n```"]):
                    with mock.patch("sys.stdout", stdout):
                        rc = lv.main()
        self.assertEqual(rc, 0)
        out = stdout.getvalue()
        self.assertIn("=== SUCCESSFUL CANDIDATE ===", out)
        self.assertIn("candidate=", out)

    def test_main_retries_and_fails_when_round_budget_is_exhausted(self):
        with tempfile.TemporaryDirectory(prefix="llmvec_test_main_fail_") as tmp:
            argv = [
                "llmVectorizer.py",
                "example_kernel.c",
                "--scalar-function",
                "saxpy_like",
                "--max-rounds",
                "2",
                "--work-dir",
                tmp,
            ]
            stdout = io.StringIO()
            stderr = io.StringIO()
            side_effects = [
                f"```c\n{BAD_CANDIDATE}\n```",
                "- Fix the arithmetic factor.",
                f"```c\n{BAD_CANDIDATE}\n```",
                "- Still wrong; restore `2 * b[i]`.",
            ]
            with mock.patch("sys.argv", argv), mock.patch.dict(os.environ, {"CLAUDE_API_KEY": "test-key"}):
                with mock.patch.object(lv, "new_agent", side_effect=["vectorizer", "tester"]):
                    with mock.patch.object(lv, "ask_agent", side_effect=side_effects):
                        with mock.patch("sys.stdout", stdout), mock.patch("sys.stderr", stderr):
                            rc = lv.main()
            self.assertEqual(rc, 1)
            self.assertIn("=== TESTER FEEDBACK ===", stdout.getvalue())
            self.assertIn("Vectorization did not converge within the round budget.", stderr.getvalue())


if __name__ == "__main__":
    unittest.main()
