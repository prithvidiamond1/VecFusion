import argparse
import os
from pathlib import Path
from typing import Optional

from src.vectorizer.task_split_init import SplitGenTaskInit
from src.vectorizer.task_split_iterate import SplitGenTaskIterate
from src.vectorizer.task_split_feedback import SplitGenFeedback
from src.vectorizer.compilerTest import CompilerTest
from src.vectorizer.compilerTest import CorrectTest
from src.vectorizer.compilerTest import PerformanceTest
from src.vectorizer.compilerTest import reset_cache
from src.vectorizer.compilerTest import FormalVerification
from src.utils import retry_parse_fail_prone_cmd

CODEX = "code-davinci-002"
GPT3 = "text-davinci-003"
CHAT_GPT = "gpt-3.5-turbo"
GPT4 = "gpt-4"
DEEPSEEKCHAT = "deepseek-chat"
DEEPSEEKCODER = "deepseek-coder"

TencentDeepSeek = "deepseek-v3"

SiliconFlow = "deepseek-ai/DeepSeek-V3"
SiliconFlow2 = "Pro/deepseek-ai/DeepSeek-R1"

ProSiliconFlow = "Pro/deepseek-ai/DeepSeek-V3"
QwenSilicon = "Qwen/Qwen2.5-Coder-32B-Instruct"
QwenSilicon2 = "Qwen/Qwen2.5-72B-Instruct"
QwenSilicon3 = "deepseek-ai/DeepSeek-R1-Distill-Qwen-32B"

SYSU = "starlight/DeepSeek-R1-671B"
QWen = "Qwen2.5-Coder-32B-Instruct"
Nvidia = "meta/llama3-70b-instruct"

Ali = "llama3.1-8b-instruct"

ENGINE = os.getenv("ENGINE")
if ENGINE is None:
    ENGINE = ProSiliconFlow


@retry_parse_fail_prone_cmd
def iterative_vectorize(source_code: str, max_attempts: int, outputFileName: str) -> Optional[str]:
    task_init = SplitGenTaskInit(engine=ENGINE, prompt_examples="data/prompt/vectorize/init.jsonl")
    task_feedback = SplitGenFeedback(engine=ENGINE, prompt_examples="data/prompt/vectorize/feedback.jsonl")
    task_iterate = SplitGenTaskIterate(engine=ENGINE, prompt_examples="data/prompt/vectorize/feedback.jsonl")

    n_attempts = 0
    print("####################### SOURCE CODE #######################")
    print(f"""```c
{source_code}
```""")
    print("####################### SOURCE CODE #######################")

    clang_feedback = CompilerTest(vectorize_code=source_code)
    llm_feedback = ""

    unit_test_error = ""
    unit_test_output = "PASS"
    unit_test_feedback = ""

    while n_attempts < max_attempts:
        print(f"######################### ROUND {n_attempts} #########################\n")
        with open(outputFileName, "a") as of:
            of.write(f"\n# ROUND {n_attempts}\n")

        if n_attempts == 0:
            vectorize_code = task_init(
                code=source_code,
                compiler_feedback=clang_feedback,
                outputFileName=outputFileName,
            )
        else:
            vectorize_code = task_iterate(
                code=source_code,
                vectorize_code=vectorize_code,
                llm_feedback=llm_feedback,
                compiler_feedback=clang_feedback,
                unit_test_feedback=unit_test_feedback,
                outputFileName=outputFileName,
            )

        clang_feedback = CompilerTest(vectorize_code=vectorize_code)

        if n_attempts == 0:
            unit_test_output = "SKIPPED"
            unit_test_error = ""
            unit_test_feedback = "UNIT TEST SKIPPED: init step only returns a declaration."
        else:
            print("######################## UNIT TEST ########################")
            unit_test_output, unit_test_error = CorrectTest("", source_code, vectorize_code, ENGINE)
            print("###################### UNIT TEST OUT ######################")
            print(unit_test_output)
            print("###################### UNIT TEST OUT ######################")

            unit_test_feedback = (
                "\nUnit Test analysis: \nSource code and optimized code semantics are inconsistent.\n"
                if unit_test_error != "" or "PASS" not in unit_test_output
                else "UNIT TEST PASS"
            )

        unit_test_feedback = (
            "\nUnit Test analysis: \nSource code and optimized code semantics are inconsistent.\n"
            if unit_test_error != "" or "PASS" not in unit_test_output
            else "UNIT TEST PASS"
        )

        llm_feedback = task_feedback(
            code=source_code,
            vectorize_code=vectorize_code,
            compiler_feedback=clang_feedback,
            outputFileName=outputFileName,
            unit_test_feedback=unit_test_feedback,
        )

        if "PASS" in llm_feedback and "FAIL" not in llm_feedback:
            print("######################## UNIT TEST ########################")
            unit_test_output, unit_test_error = CorrectTest("", source_code, vectorize_code, ENGINE)
            print("###################### UNIT TEST OUT ######################")
            print(unit_test_output)
            print("###################### UNIT TEST OUT ######################")

            unit_test_feedback = (
                "\nUnit Test analysis: \nSource code and optimized code semantics are inconsistent.\n"
                if unit_test_error != "" or "PASS" not in unit_test_output
                else "UNIT TEST PASS"
            )
            llm_feedback += unit_test_feedback

            if unit_test_error != "" or "PASS" not in unit_test_output:
                n_attempts += 1
                continue

            print("################### Formal Verification ###################")
            formalVerify = FormalVerification(source_code, vectorize_code)
            print(formalVerify)
            print("################### Formal Verification ###################")

            if "1 incorrect transformations" in formalVerify:
                n_attempts += 1
                continue

            with open(outputFileName, "a") as of:
                of.write("\n# FINAL CODE\n")
                of.write(f"""
```c
{vectorize_code}
```""")

            print("####################### FINAL OUTPUT ######################")
            print(vectorize_code)
            print("####################### FINAL OUTPUT ######################")
            return vectorize_code

        n_attempts += 1

    return None


def run_file(source_file: str | Path, max_attempts: int = 20, log_dir: str | Path = "log") -> Optional[str]:
    source_file = Path(source_file).resolve()
    log_dir = Path(log_dir)
    log_dir.mkdir(parents=True, exist_ok=True)

    output_path = log_dir / f"{source_file.stem}.md"
    if output_path.exists():
        output_path.unlink()

    reset_cache()
    source_code = source_file.read_text()

    return iterative_vectorize(
        source_code=source_code,
        max_attempts=max_attempts,
        outputFileName=str(output_path),
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source_file", help="Path to a single benchmark .c file")
    parser.add_argument("--max-attempts", type=int, default=20)
    parser.add_argument("--log-dir", default="log")
    args = parser.parse_args()

    candidate = run_file(
        source_file=args.source_file,
        max_attempts=args.max_attempts,
        log_dir=args.log_dir,
    )
    return 0 if candidate is not None else 1


if __name__ == "__main__":
    raise SystemExit(main())