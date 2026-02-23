# Lightpath Migration Guide

## Summary

Lightpath now publishes a stable public API under `include/lightpath/` while
preserving compatibility with existing `src/` includes.

## What Changed

1. Added public namespaced API headers:
   - `lightpath/lightpath.hpp` (umbrella)
   - `lightpath/types.hpp`, `topology.hpp`, `runtime.hpp`, `rendering.hpp`, `objects.hpp`, `factory.hpp`, `debug.hpp`
2. Added CMake target alias:
   - `lightpath::lightpath` (aliases existing `lightpath_core`)
3. Added `lightpath::Engine` facade and `lightpath::makeObject(...)` factory helpers.
4. Added example target and public API test coverage.
5. Updated parent integrations (simulator + firmware) to include the new public headers.

## Why

- Provide a professional, discoverable API boundary for external users.
- Decouple public usage from internal file layout under `src/`.
- Keep existing behavior and integrations stable via compatibility layers.

## Migration Steps

1. Prefer replacing direct `src/*` includes with:
   - `#include <lightpath/lightpath.hpp>`
   - or module-specific headers under `lightpath/`.
2. In CMake consumers, link `lightpath::lightpath`.
3. Optionally migrate to facade-based startup:
   - `auto object = lightpath::makeObject(...);`
   - `lightpath::Engine engine(std::move(object));`

## Compatibility / Deprecation Notes

- Legacy `src/` include paths remain exported by default for compatibility.
- This behavior is controlled by `LIGHTPATH_CORE_ENABLE_LEGACY_INCLUDE_PATHS`.
- No runtime behavior changes are intended in this migration.

## Parent Migration Notes (MeshLED)

The parent repository was migrated in the same change set:

1. Firmware headers now consume public API headers:
   - `firmware/esp/LightPath.h` -> `<lightpath/lightpath.hpp>`
   - `firmware/esp/WebServerLayers.h` -> `<lightpath/rendering.hpp>`
   - `firmware/esp/homo_deus.ino` (debug path) -> `<lightpath/debug.hpp>`
2. PlatformIO now exports the public include directory:
   - `firmware/esp/platformio.ini` adds `-I../../packages/lightpath/include`
3. Simulator now includes the umbrella API header:
   - `apps/simulator/src/ofApp.h` -> `\"lightpath/lightpath.hpp\"`
   - `apps/simulator/config.make` adds include path `-I../../packages/lightpath/include`
