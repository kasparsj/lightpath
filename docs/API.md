# Lightpath API Reference

This document describes the public API exposed under `include/lightpath/`.

## Entry Points

- Umbrella include: `#include <lightpath/lightpath.hpp>`
- Modular includes:
  - `lightpath/types.hpp`
  - `lightpath/topology.hpp`
  - `lightpath/runtime.hpp`
  - `lightpath/rendering.hpp`
  - `lightpath/objects.hpp`
  - `lightpath/factory.hpp`
  - `lightpath/debug.hpp`

## Core Types

### `lightpath::Color`

Alias of legacy `ColorRGB`.

Invariants:

- Channels are 8-bit RGB (`R`, `G`, `B`)
- `get()` returns packed `0xRRGGBB`

### `lightpath::EmitParams`

Defines one emit request.

Invariants:

- `model` must reference a valid model for the active object (`State::emit` returns `-1` when invalid)
- `setLength(...)` controls requested list length (or random when omitted)

## Topology Module (`topology.hpp`)

- `lightpath::Object` / `lightpath::LPObject`
- `lightpath::Intersection`
- `lightpath::Connection`
- `lightpath::Model`
- `lightpath::Port`, `lightpath::InternalPort`, `lightpath::ExternalPort`

Invariants:

- Grouped topology arrays are bounded by `lightpath::kMaxGroups`
- `Connection` LED count controls light traversal granularity

## Runtime Module (`runtime.hpp`)

- `lightpath::RuntimeState` / `lightpath::State`
- `lightpath::LightList`, `lightpath::BgLight`
- `lightpath::LPLight`, `lightpath::Light`
- `lightpath::Behaviour`
- `lightpath::Engine` facade

### `lightpath::Engine`

RAII wrapper owning both topology object and runtime state.

Behavior:

- `Engine::update(millis)` updates global runtime time and advances state.
- `Engine::state()` exposes full `State` API.

## Rendering Module (`rendering.hpp`)

- `lightpath::Palette`
- `lightpath::paletteCount()`
- `lightpath::paletteAt(index)`

Invariants:

- Palette interpolation is deterministic for fixed inputs.
- Wrap behavior is controlled via `kWrapNoWrap`, `kWrapClampToEdge`, `kWrapRepeat`, `kWrapRepeatMirror`.

## Built-In Objects (`objects.hpp` + `factory.hpp`)

Built-ins:

- `Heptagon919`
- `Heptagon3024`
- `Line`
- `Cross`
- `Triangle`

Factory:

- `lightpath::makeObject(lightpath::BuiltinObjectType type, uint16_t pixelCount = 0)`

Default pixel counts for line/cross/triangle are exposed as:

- `lightpath::kLinePixelCount`
- `lightpath::kCrossPixelCount`
- `lightpath::kTrianglePixelCount`

## Debug Module (`debug.hpp`)

- `lightpath::Debugger` / `lightpath::LPDebugger`

Provides inspection helpers for intersections, connections, and weighted model coverage.

## Compatibility Notes

- Legacy symbols remain available from `src/` headers.
- Public aliases intentionally map to existing types to preserve behavior.
