# Development Workflows

## CI Presets

`CMakePresets.json` includes CI-friendly profiles:

- `default`: normal build + tests + example
- `warnings`: warnings as errors
- `asan`: AddressSanitizer
- `ubsan`: UndefinedBehaviorSanitizer
- `static-analysis`: compile commands + benchmark target for analysis tooling
- `docs`: Doxygen docs target
- `coverage`: gcov/llvm-cov instrumentation profile for report generation

```bash
cmake --preset default
cmake --build --preset default
ctest --preset default
```

## Static Analysis

```bash
./scripts/check-clang-format.sh
cmake -S . -B build/static-analysis -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLIGHTGRAPH_CORE_BUILD_TESTS=OFF -DLIGHTGRAPH_CORE_BUILD_EXAMPLES=OFF -DLIGHTGRAPH_CORE_BUILD_BENCHMARKS=ON
./scripts/run-clang-tidy.sh build/static-analysis
./scripts/check-benchmark.sh build/static-analysis/lightgraph_core_benchmark
```

## Coverage Reporting

```bash
cmake --preset coverage
cmake --build --preset coverage --parallel
ctest --preset coverage
./scripts/generate-coverage.sh build/preset-coverage
```

## Benchmarks

Micro-benchmark target:

```bash
cmake -S . -B build-bench -DLIGHTGRAPH_CORE_BUILD_BENCHMARKS=ON -DLIGHTGRAPH_CORE_BUILD_TESTS=OFF -DLIGHTGRAPH_CORE_BUILD_EXAMPLES=OFF
cmake --build build-bench --parallel
./build-bench/lightgraph_core_benchmark
```

## Source Layout

- `include/lightgraph/`: stable facade + source-integration module headers
- `src/topology/`: graph objects and routing
- `src/runtime/`: state update and animation
- `src/rendering/`: palette and blend implementation
- `src/objects/`: built-in topology definitions
- `src/debug/`: debug helpers
- `src/core/`: shared constants, types, platform macros

## Generated API Reference

The repository publishes Doxygen API docs to GitHub Pages through GitHub Actions.

Local generation:

```bash
doxygen Doxyfile
```

Generated HTML output:

- `build/docs/html/index.html`
