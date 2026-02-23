# Lightpath API Reference

This document describes the supported public API in `include/lightpath/`.

## 1) Umbrella Include

```cpp
#include <lightpath/lightpath.hpp>
```

The umbrella header re-exports stable installable API headers:

- `lightpath/engine.hpp`
- `lightpath/types.hpp`
- `lightpath/status.hpp`

## 2) High-Level Engine API

### `lightpath::ObjectType`

Built-in object selection enum:

- `Heptagon919`
- `Heptagon3024`
- `Line`
- `Cross`
- `Triangle`

### `lightpath::EngineConfig`

Engine configuration fields:

- `object_type`
- `pixel_count` (`0` uses object default)
- `auto_emit`

### `lightpath::EmitCommand`

Value command for one emit request.

Key fields:

- `model`, `speed`, `length`, `trail`, `color`
- `note_id`, `min_brightness`, `max_brightness`
- `behaviour_flags`, `emit_groups`, `emit_offset`
- `duration_ms`, `from`, `linked`

### `lightpath::ErrorCode`, `lightpath::Status`, `lightpath::Result<T>`

Typed error and result model used by `Engine`.

### `lightpath::Engine`

Thread-safe runtime facade:

- `Result<int8_t> emit(const EmitCommand&)`
- `void update(uint64_t millis)`
- `void tick(uint64_t delta_millis)`
- `void stopAll()`
- `bool isOn() const`, `void setOn(bool)`
- `bool autoEmitEnabled() const`, `void setAutoEmitEnabled(bool)`
- `uint16_t pixelCount() const`
- `Result<Color> pixel(uint16_t index, uint8_t max_brightness = 255) const`

## 3) Source-Integration Module Headers

These headers expose broader topology/runtime/rendering integration types used by MeshLED.
They are source-level integration headers and are not part of the installable stable package contract.

### `lightpath/topology.hpp`

Namespace aliases:

- `lightpath::Object` (`LPObject`)
- `lightpath::Intersection`
- `lightpath::Connection`
- `lightpath::Model`
- `lightpath::Owner`
- `lightpath::Port`, `lightpath::InternalPort`, `lightpath::ExternalPort`
- `lightpath::Weight`

### `lightpath/runtime.hpp`

Namespace aliases:

- `lightpath::EmitParam`
- `lightpath::EmitParams`
- `lightpath::Behaviour`
- `lightpath::RuntimeLight`
- `lightpath::Light`
- `lightpath::LightList`
- `lightpath::BgLight`
- `lightpath::RuntimeState`

### `lightpath/rendering.hpp`

Namespace aliases/helpers:

- `lightpath::Palette`
- `lightpath::kWrapNoWrap`
- `lightpath::kWrapClampToEdge`
- `lightpath::kWrapRepeat`
- `lightpath::kWrapRepeatMirror`
- `lightpath::paletteCount()`
- `lightpath::paletteAt(index)`

### `lightpath/objects.hpp`

Namespace aliases/constants:

- `lightpath::Heptagon919`, `lightpath::Heptagon3024`, `lightpath::Line`, `lightpath::Cross`, `lightpath::Triangle`
- model enums for built-ins
- default pixel-count constants (`kLinePixelCount`, etc.)

### `lightpath/factory.hpp`

- `lightpath::BuiltinObjectType`
- `lightpath::makeObject(...)`

### `lightpath/debug.hpp`

- `lightpath::Debugger`

## 4) Invariants

- Stable engine API never returns raw owning pointers.
- `Engine::pixel(...)` returns `ErrorCode::OutOfRange` for invalid indices.
- `EmitCommand::max_brightness` must be `>= min_brightness` (`InvalidArgument` otherwise).
- Runtime update and output access in `Engine` are mutex-protected.

## 5) Internal Layout (Non-API)

- `src/topology`
- `src/runtime`
- `src/rendering`
- `src/objects`
- `src/debug`

Internal headers under `src/` are implementation details and may change.
