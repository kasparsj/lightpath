# Lightpath Migration Guide

This guide covers migration to the current single-layer header layout.

## Summary

The `lightpath/legacy*` compatibility layer was removed.
Former compatibility headers were promoted to top-level public headers.

## Breaking Changes

1. Removed:
   - `lightpath/legacy.hpp`
   - `lightpath/legacy/*.hpp`
2. Include paths changed:
   - `#include <lightpath/legacy.hpp>` -> `#include <lightpath/lightpath.hpp>`
   - `#include <lightpath/legacy/rendering.hpp>` -> `#include <lightpath/integration/rendering.hpp>`
   - `#include <lightpath/legacy/debug.hpp>` -> `#include <lightpath/integration/debug.hpp>`
3. Legacy install option removed:
   - `LIGHTPATH_CORE_INSTALL_LEGACY_HEADERS`

## New Public Header Layout

Integration headers are now explicitly separated under `lightpath/integration*`:

- `lightpath/integration.hpp`
- `lightpath/integration/topology.hpp`
- `lightpath/integration/runtime.hpp`
- `lightpath/integration/rendering.hpp`
- `lightpath/integration/objects.hpp`
- `lightpath/integration/factory.hpp`
- `lightpath/integration/debug.hpp`

High-level typed stable facade:

- `lightpath/engine.hpp`
- `lightpath/types.hpp`
- `lightpath/status.hpp`

## Typical Migration

### Before

```cpp
#include <lightpath/legacy.hpp>
```

### After

```cpp
#include <lightpath/lightpath.hpp>
```

If you previously depended on legacy topology/runtime internals, include modules directly:

```cpp
#include <lightpath/integration/runtime.hpp>
#include <lightpath/integration/topology.hpp>
```

## Parent Migration Notes (MeshLED)

Updated in the same change set:

1. `/Users/kasparsj/Work2/meshled/apps/simulator/src/ofApp.h`
   - `lightpath/legacy.hpp` -> `lightpath/integration.hpp`
2. `/Users/kasparsj/Work2/meshled/firmware/esp/LightPath.h`
   - `lightpath/legacy.hpp` -> `lightpath/integration.hpp`
3. `/Users/kasparsj/Work2/meshled/firmware/esp/WebServerLayers.h`
   - `lightpath/legacy/rendering.hpp` -> `lightpath/integration/rendering.hpp`
4. `/Users/kasparsj/Work2/meshled/firmware/esp/homo_deus.ino`
   - `lightpath/legacy/debug.hpp` -> `lightpath/integration/debug.hpp`

## Notes

- Existing source-integration aliases now live under `lightpath::integration::*`.
- `lightpath::Engine` (typed facade) remains the recommended entry point for new host integrations.
- Install/export package consumers should prefer the stable umbrella (`lightpath/lightpath.hpp`).
