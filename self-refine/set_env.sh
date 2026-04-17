#!/bin/bash
# export SDKROOT="$(xcrun --show-sdk-path)"
# export CPATH="$SDKROOT/usr/include"
# export C_INCLUDE_PATH="$SDKROOT/usr/include"
# export CPLUS_INCLUDE_PATH="$SDKROOT/usr/include"

# export ALIVE2_HOME="$HOME/ALIVE2/build"
# export LLM_BASE_URL="https://api.anthropic.com/v1/"
# export ANTHROPIC_API_KEY="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"
# export OPENAI_API_KEY="$ANTHROPIC_API_KEY"
# export API_KEY="$ANTHROPIC_API_KEY"
# export ENGINE="claude-sonnet-4-6"
# export PATH=$ALIVE2_HOME:$PATH
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
# export CLANG_PATH=/usr/local/bin/clang
# export PYTHONPATH=".:../:src:../../:prompt-lib"

export DEEPSEEK_API_KEY="sk-1de7ea9c92824294bc0b732709c704a4"

export LLM_BASE_URL="https://api.deepseek.com/v1"
export OPENAI_API_KEY="$DEEPSEEK_API_KEY"
export API_KEY="$DEEPSEEK_API_KEY"
export ENGINE="deepseek-chat"

export ALIVE2_HOME=~/ALIVE2
export BISHENG_HOME=~/BISHENG
export PATH="$BISHENG_HOME/bin:$ALIVE2_HOME:$PATH"