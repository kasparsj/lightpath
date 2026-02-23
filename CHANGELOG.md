# Changelog

## Unreleased

### API

- Added stable high-level public API:
  - `include/lightpath/lightpath.hpp`
  - `include/lightpath/engine.hpp`
  - `include/lightpath/types.hpp`
  - `include/lightpath/status.hpp`
- Removed `include/lightpath/legacy*` compatibility layer.
- Moved source-integration module headers under `include/lightpath/integration/` and `include/lightpath/integration.hpp`.
- Moved source-integration aliases into `lightpath::integration::*` namespace.
- Narrowed `lightpath/lightpath.hpp` to stable installable API headers only (`engine.hpp`, `types.hpp`, `status.hpp`).
- Added typed status/result error model (`ErrorCode`, `Status`, `Result<T>`).
- Added explicit source-integration CMake target (`lightpath::integration`) for
  non-installable `lightpath/integration*.hpp` usage.

### Refactor

- Added `src/api/Engine.cpp` facade over legacy runtime/state internals.
- Improved `LPObject` ownership handling using internal `std::unique_ptr` containers.
- Hardened runtime pixel access against out-of-range reads/writes in `State`.
- Fixed `LightList` reallocation teardown to delete using allocated size.
- Fixed undefined behavior in `Connection::render` index conversion/clamping.
- Replaced recursive source globs with explicit CMake source lists.
- Split third-party color-theory compilation into a dedicated internal target.

### Build

- Added install/export/package-config support (`lightpathConfig.cmake`).
- Added CI-friendly `CMakePresets.json` profiles:
  - `default`, `warnings`, `asan`, `ubsan`

### Tests

- Added stable API coverage (`tests/public_api_test.cpp`).
- Added API fuzz lane (`tests/api_fuzz_test.cpp`).
- Added mutation edge coverage (`tests/core_mutation_edge_test.cpp`).
- Added sanitizer-driven regressions for runtime memory/UB fixes.

### Docs

- Reworked `README.md` to document stable-vs-source-integration header tiers.
- Rewrote `docs/API.md` for the current public surface.
- Updated `MIGRATION.md` with breaking changes and parent migration notes.
- Added thread-safety, determinism, and complexity guarantees to API docs.
- Added a host-loop integration example with multi-object + custom palette strategy.
