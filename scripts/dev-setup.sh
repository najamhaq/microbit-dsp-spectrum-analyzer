#!/usr/bin/env bash
set -euo pipefail

git submodule update --init --recursive
git config core.hooksPath .githooks
echo "OK: git hooksPath set to .githooks"

# Optional: show tool presence
for t in git cmake ninja clang-format; do
  command -v "$t" >/dev/null 2>&1 && echo "FOUND: $t" || echo "MISSING: $t"
done

