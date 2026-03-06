#pragma once

#include "lightgraph/internal/topology.hpp"

/**
 * @file topology.hpp
 * @brief Public topology and routing graph types.
 */

namespace lightgraph::integration {

using Owner = ::Owner;
using Port = ::Port;
using InternalPort = ::InternalPort;
using ExternalPort = ::ExternalPort;
using Weight = ::Weight;
using Model = ::Model;
using Intersection = ::Intersection;
using Connection = ::Connection;
using Object = ::TopologyObject;
using PixelGap = ::PixelGap;

} // namespace lightgraph::integration
