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
   - `#include <lightpath/legacy/rendering.hpp>` -> `#include <lightpath/rendering.hpp>`
   - `#include <lightpath/legacy/debug.hpp>` -> `#include <lightpath/debug.hpp>`
3. Legacy install option removed:
   - `LIGHTPATH_CORE_INSTALL_LEGACY_HEADERS`

## New Public Header Layout

Top-level module headers are now part of the primary API surface:

- `lightpath/topology.hpp`
- `lightpath/runtime.hpp`
- `lightpath/rendering.hpp`
- `lightpath/objects.hpp`
- `lightpath/factory.hpp`
- `lightpath/debug.hpp`

High-level typed facade remains available:

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

Or include modules directly as needed:

```cpp
#include <lightpath/runtime.hpp>
#include <lightpath/topology.hpp>
```

## Parent Migration Notes (MeshLED)

Updated in the same change set:

1. `/Users/kasparsj/Work2/meshled/apps/simulator/src/ofApp.h`
   - `lightpath/legacy.hpp` -> `lightpath/lightpath.hpp`
2. `/Users/kasparsj/Work2/meshled/firmware/esp/LightPath.h`
   - `lightpath/legacy.hpp` -> `lightpath/lightpath.hpp`
3. `/Users/kasparsj/Work2/meshled/firmware/esp/WebServerLayers.h`
   - `lightpath/legacy/rendering.hpp` -> `lightpath/rendering.hpp`
4. `/Users/kasparsj/Work2/meshled/firmware/esp/homo_deus.ino`
   - `lightpath/legacy/debug.hpp` -> `lightpath/debug.hpp`

## Notes

- Existing namespaced aliases used by MeshLED integrations (`lightpath::Object`, `lightpath::RuntimeState`, `lightpath::EmitParams`, etc.) remain available via promoted module headers.
- `lightpath::Engine` (typed facade) remains the recommended entry point for new host integrations.
