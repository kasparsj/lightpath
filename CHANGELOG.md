# Changelog

## Unreleased

### API

- Added a public namespaced header surface under `include/lightpath/`.
- Added umbrella include `lightpath/lightpath.hpp`.
- Added object factory helper `lightpath::makeObject(...)`.
- Added RAII runtime facade `lightpath::Engine`.

### Build

- Added target alias `lightpath::lightpath`.
- Added `LIGHTPATH_CORE_BUILD_EXAMPLES` option and minimal example target.
- Added `LIGHTPATH_CORE_ENABLE_LEGACY_INCLUDE_PATHS` option to keep old include layout available.

### Tests

- Added `tests/public_api_test.cpp` for public API coverage.
- Added example execution to CTest when tests are enabled.

### Docs

- Reworked `README.md` with public API quickstart and integration guidance.
- Added API reference at `docs/API.md`.
- Added migration guide at `MIGRATION.md`.
