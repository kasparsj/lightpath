#!/usr/bin/env bash
set -euo pipefail

if ! command -v clang-format >/dev/null 2>&1; then
  echo "clang-format is required but was not found in PATH" >&2
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

files=(
  "$ROOT_DIR/include/lightpath/lightpath.hpp"
  "$ROOT_DIR/include/lightpath/engine.hpp"
  "$ROOT_DIR/include/lightpath/status.hpp"
  "$ROOT_DIR/include/lightpath/types.hpp"
  "$ROOT_DIR/include/lightpath/integration.hpp"
  "$ROOT_DIR/include/lightpath/integration/debug.hpp"
  "$ROOT_DIR/include/lightpath/integration/factory.hpp"
  "$ROOT_DIR/include/lightpath/integration/objects.hpp"
  "$ROOT_DIR/include/lightpath/integration/rendering.hpp"
  "$ROOT_DIR/include/lightpath/integration/runtime.hpp"
  "$ROOT_DIR/include/lightpath/integration/topology.hpp"
  "$ROOT_DIR/src/api/Engine.cpp"
  "$ROOT_DIR/examples/minimal_usage.cpp"
  "$ROOT_DIR/tests/public_api_test.cpp"
  "$ROOT_DIR/tests/api_fuzz_test.cpp"
  "$ROOT_DIR/tests/core_mutation_edge_test.cpp"
  "$ROOT_DIR/benchmarks/core_benchmark.cpp"
)

clang-format --dry-run --Werror "${files[@]}"
