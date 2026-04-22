import os
from typing import Any, Dict
from anthropic import Anthropic

from prompt_lib.backends.wrapper import BaseAPIWrapper


ANTHROPIC_API_KEY = os.environ.get("ANTHROPIC_API_KEY") or os.environ.get("API_KEY")
client = Anthropic(api_key=ANTHROPIC_API_KEY)


class AnthropicAPIWrapper(BaseAPIWrapper):
    @staticmethod
    def _normalize_messages(prompt):
        if isinstance(prompt, list):
            messages = []
            for msg in prompt:
                role = msg.get("role", "user")
                content = msg.get("content", "")
                if role in {"system", "developer"}:
                    role = "user"
                messages.append({"role": role, "content": content})
            return messages
        return [{"role": "user", "content": str(prompt)}]

    @staticmethod
    def _call_api(
        prompt,
        max_tokens: int,
        engine: str,
        stop_token: str,
        temperature: float,
        num_completions: int = 1,
    ):
        messages = AnthropicAPIWrapper._normalize_messages(prompt)

        kwargs = {
            "model": engine,
            "max_tokens": max_tokens,
            "temperature": temperature,
            "messages": messages,
        }

        if stop_token:
            kwargs["stop_sequences"] = [stop_token]

        response = client.messages.create(**kwargs)
        return response

    @staticmethod
    def call(
        prompt,
        max_tokens: int,
        engine: str,
        stop_token: str,
        temperature: float,
        num_completions: int = 1,
    ):
        return AnthropicAPIWrapper._call_api(
            prompt=prompt,
            max_tokens=max_tokens,
            engine=engine,
            stop_token=stop_token,
            temperature=temperature,
            num_completions=num_completions,
        )

    @staticmethod
    def get_first_response(response) -> str:
        texts = []
        for block in response.content:
            if getattr(block, "type", None) == "text":
                texts.append(block.text)
        return "".join(texts)

    @staticmethod
    def get_majority_answer(response) -> Dict[str, Any]:
        raise NotImplementedError

    @staticmethod
    def get_all_responses(response) -> Dict[str, Any]:
        raise NotImplementedError