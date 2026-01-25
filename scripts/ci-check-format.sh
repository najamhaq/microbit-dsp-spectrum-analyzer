#!/usr/bin/env bash
set -euo pipefail

command -v git >/dev/null 2>&1 || { echo "ERROR: git not found"; exit 1; }
command -v clang-format >/dev/null 2>&1 || { echo "ERROR: clang-format not found"; exit 1; }

# Check formatting for tracked sources, excluding vendor file
git ls-files -z '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hpp' ':!src/rtos/heap_4.c' \
  | xargs -0 -r -n 50 clang-format --dry-run --Werror

echo "OK: formatting clean."
