#pragma once

#include "topology.hpp"
#include "src/topology/TopologySummary.h"

/**
 * @file topology_summary.hpp
 * @brief Detailed topology summary helpers for adapter-side snapshot/export code.
 */

namespace lightgraph::integration {

using TopologySummary = ::TopologySummary;
using TopologySummaryIntersection = ::TopologySummaryIntersection;
using TopologySummaryConnection = ::TopologySummaryConnection;
using TopologySummaryPort = ::TopologySummaryPort;
using TopologySummaryModel = ::TopologySummaryModel;

inline TopologySummary summarizeTopology(const Object& object) {
    return ::buildTopologySummary(object);
}

} // namespace lightgraph::integration
