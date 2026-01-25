#!/usr/bin/env bash
set -euo pipefail

command -v cmake >/dev/null 2>&1 || { echo "ERROR: cmake not found"; exit 1; }

BUILD_DIR="${BUILD_DIR:-build-host}"

cmake -S . -B "$BUILD_DIR" -G Ninja
cmake --build "$BUILD_DIR"
ctest --test-dir "$BUILD_DIR" --output-on-failure

