#!/bin/bash

export DEEPSEEK_API_KEY="sk-1de7ea9c92824294bc0b732709c704a4"

# VecTrans / OpenAI-compatible DeepSeek path
export LLM_BASE_URL="https://api.deepseek.com/v1"
export OPENAI_API_KEY="$DEEPSEEK_API_KEY"
export API_KEY="$DEEPSEEK_API_KEY"
export ENGINE="deepseek-chat"

# LLM-Vectorizer / Anthropic-compatible DeepSeek path
export ANTHROPIC_API_KEY="$DEEPSEEK_API_KEY"
export ANTHROPIC_BASE_URL="https://api.deepseek.com/anthropic"
export LLM_VECTORIZER_MODEL="deepseek-chat"

# Clear stale old-provider vars
unset CLAUDE_API_KEY
unset ANTHROPIC_AUTH_TOKEN

export ALIVE2_HOME=$HOME/ALIVE2/build
export BISHENG_HOME=~/BISHENG
export PATH="$BISHENG_HOME/bin:$ALIVE2_HOME:$PATH"