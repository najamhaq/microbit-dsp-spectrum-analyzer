#!/usr/bin/env bash
set -euo pipefail

command -v git >/dev/null 2>&1 || { echo "ERROR: git not found"; exit 1; }
command -v clang-format >/dev/null 2>&1 || { echo "ERROR: clang-format not found"; exit 1; }

echo "Formatting source files..."

# Stream NUL-separated file list directly into xargs -0 (do NOT store in a variable)
git ls-files -z '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hpp' \
  | xargs -0 -r -n 50 clang-format -i

echo "OK: formatted."
