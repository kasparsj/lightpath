#pragma once

#include "runtime.hpp"
#include "lightgraph/internal/runtime/LayerView.h"

/**
 * @file layers.hpp
 * @brief Shared palette and editable-layer view helpers for adapters.
 */

namespace lightgraph::integration {

using PaletteView = ::PaletteView;
using LayerView = ::LayerView;

inline PaletteView paletteView(const Palette& palette) {
    return ::makePaletteView(palette);
}

inline Palette paletteFromView(const PaletteView& view) {
    return ::makePaletteFromView(view);
}

inline PaletteView normalizePalette(const PaletteView& view) {
    return ::normalizePaletteView(view);
}

inline std::vector<LayerView> layerViews(const RuntimeState& state, bool editableOnly = false) {
    return ::snapshotLayers(state, editableOnly);
}

} // namespace lightgraph::integration
