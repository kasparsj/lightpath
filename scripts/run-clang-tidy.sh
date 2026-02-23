#!/usr/bin/env bash
set -euo pipefail

if ! command -v clang-tidy >/dev/null 2>&1; then
  echo "clang-tidy is required but was not found in PATH" >&2
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${1:-$ROOT_DIR/build/static-analysis}"
COMPILE_COMMANDS="$BUILD_DIR/compile_commands.json"

if [[ ! -f "$COMPILE_COMMANDS" ]]; then
  echo "Missing compile_commands.json at $COMPILE_COMMANDS" >&2
  exit 1
fi

files=(
  "$ROOT_DIR/src/api/Engine.cpp"
  "$ROOT_DIR/examples/minimal_usage.cpp"
  "$ROOT_DIR/tests/public_api_test.cpp"
  "$ROOT_DIR/tests/api_fuzz_test.cpp"
  "$ROOT_DIR/tests/core_mutation_edge_test.cpp"
  "$ROOT_DIR/benchmarks/core_benchmark.cpp"
)

for file in "${files[@]}"; do
  clang-tidy \
    "$file" \
    -p "$BUILD_DIR" \
    --quiet \
    --checks=-*,clang-analyzer-* \
    --warnings-as-errors=*
done
