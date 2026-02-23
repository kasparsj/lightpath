#!/usr/bin/env bash
set -euo pipefail

if ! command -v gcovr >/dev/null 2>&1; then
  echo "gcovr is required but was not found in PATH" >&2
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${1:-$ROOT_DIR/build/preset-coverage}"
COVERAGE_DIR="$BUILD_DIR/coverage"

if [[ ! -d "$BUILD_DIR" ]]; then
  echo "Build directory not found: $BUILD_DIR" >&2
  exit 1
fi

mkdir -p "$COVERAGE_DIR"

gcovr \
  --root "$ROOT_DIR" \
  --object-directory "$BUILD_DIR" \
  --filter "$ROOT_DIR/src" \
  --filter "$ROOT_DIR/include" \
  --exclude "$ROOT_DIR/vendor" \
  --exclude "$ROOT_DIR/tests" \
  --exclude "$ROOT_DIR/examples" \
  --exclude "$ROOT_DIR/benchmarks" \
  --xml-pretty \
  --output "$COVERAGE_DIR/coverage.xml" \
  --json-summary-pretty \
  --json-summary "$COVERAGE_DIR/coverage-summary.json" \
  --print-summary

