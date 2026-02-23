#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../src/Globals.h"
#include "../src/objects/Cross.h"
#include "../src/objects/Line.h"
#include "../src/runtime/EmitParams.h"
#include "../src/runtime/LightList.h"
#include "../src/runtime/State.h"
#include "../src/topology/Connection.h"
#include "../src/topology/Intersection.h"
#include "../src/topology/Model.h"

namespace {

int fail(const std::string& message) {
    std::cerr << "FAIL: " << message << std::endl;
    return 1;
}

bool isNonBlack(const ColorRGB& color) {
    return color.R > 0 || color.G > 0 || color.B > 0;
}

bool isApproxColor(const ColorRGB& color, uint8_t r, uint8_t g, uint8_t b, int tolerance = 1) {
    return std::abs(static_cast<int>(color.R) - static_cast<int>(r)) <= tolerance &&
           std::abs(static_cast<int>(color.G) - static_cast<int>(g)) <= tolerance &&
           std::abs(static_cast<int>(color.B) - static_cast<int>(b)) <= tolerance;
}

uint8_t countConnectedPorts(const Intersection& intersection) {
    uint8_t used = 0;
    for (uint8_t i = 0; i < intersection.numPorts; ++i) {
        if (intersection.ports[i] != nullptr) {
            ++used;
        }
    }
    return used;
}

int findIntersectionIndexByTopPixel(TopologyObject& object, uint16_t topPixel, uint8_t groups = GROUP1) {
    const uint8_t count = object.countIntersections(groups);
    for (uint8_t i = 0; i < count; ++i) {
        Intersection* intersection = object.getIntersection(i, groups);
        if (intersection != nullptr && intersection->topPixel == topPixel) {
            return i;
        }
    }
    return -1;
}

Intersection* findIntersectionByTopPixel(TopologyObject& object, uint16_t topPixel, uint8_t groups = GROUP1) {
    const int index = findIntersectionIndexByTopPixel(object, topPixel, groups);
    if (index < 0) {
        return nullptr;
    }
    return object.getIntersection(static_cast<uint8_t>(index), groups);
}

int findPhysicalConnectionIndex(TopologyObject& object, uint8_t groups = GROUP1) {
    const uint8_t count = object.countConnections(groups);
    for (uint8_t i = 0; i < count; ++i) {
        Connection* connection = object.getConnection(i, groups);
        if (connection != nullptr && connection->numLeds > 0) {
            return i;
        }
    }
    return -1;
}

bool connectionMatches(const Connection* connection, uint16_t leftTopPixel, uint16_t rightTopPixel, int16_t numLeds = -1) {
    if (connection == nullptr || connection->from == nullptr || connection->to == nullptr) {
        return false;
    }

    const bool endpointsMatch =
        (connection->from->topPixel == leftTopPixel && connection->to->topPixel == rightTopPixel) ||
        (connection->from->topPixel == rightTopPixel && connection->to->topPixel == leftTopPixel);
    if (!endpointsMatch) {
        return false;
    }

    return numLeds < 0 || connection->numLeds == static_cast<uint16_t>(numLeds);
}

Connection* findConnectionByTopPixels(TopologyObject& object, uint16_t leftTopPixel, uint16_t rightTopPixel, int16_t numLeds = -1) {
    const uint8_t count = object.countConnections(GROUP1);
    for (uint8_t i = 0; i < count; ++i) {
        Connection* connection = object.getConnection(i, GROUP1);
        if (connectionMatches(connection, leftTopPixel, rightTopPixel, numLeds)) {
            return connection;
        }
    }
    return nullptr;
}

Port* portAtIntersection(Connection* connection, const Intersection* intersection) {
    if (connection == nullptr || intersection == nullptr) {
        return nullptr;
    }
    if (connection->from == intersection) {
        return connection->fromPort;
    }
    if (connection->to == intersection) {
        return connection->toPort;
    }
    return nullptr;
}

uint16_t countLitPixels(State& state, uint16_t startInclusive, uint16_t endExclusive) {
    uint16_t lit = 0;
    for (uint16_t pixel = startInclusive; pixel < endExclusive; ++pixel) {
        if (isNonBlack(state.getPixel(pixel))) {
            ++lit;
        }
    }
    return lit;
}

void advanceFrame(State& state, unsigned long deltaMillis = 16) {
    gMillis += deltaMillis;
    state.update();
}

} // namespace

int main() {
    std::srand(23);
    gMillis = 0;

    // Topology setup: line should expose one bridge and one physical run.
    {
        Line line(300);
        const uint16_t expectedPhysicalLength = static_cast<uint16_t>(line.pixelCount - 3);
        if (line.countIntersections(GROUP1) != 2) {
            return fail("Line topology should create 2 intersections");
        }
        if (line.countConnections(GROUP1) != 2) {
            return fail("Line topology should create 2 connections");
        }
        if (line.getModel(L_DEFAULT) == nullptr || line.getModel(L_BOUNCE) == nullptr) {
            return fail("Line topology should create default and bounce models");
        }

        uint8_t zeroLengthConnections = 0;
        uint8_t physicalConnections = 0;
        const uint8_t connectionCount = line.countConnections(GROUP1);
        for (uint8_t i = 0; i < connectionCount; ++i) {
            Connection* connection = line.getConnection(i, GROUP1);
            if (connection == nullptr) {
                return fail("Line connection list should not contain null pointers");
            }
            if (connection->numLeds == 0) {
                ++zeroLengthConnections;
            }
            if (connection->numLeds == expectedPhysicalLength) {
                ++physicalConnections;
            }
        }
        if (zeroLengthConnections != 1 || physicalConnections != 1) {
            return fail("Line topology has unexpected connection lengths");
        }

        const uint8_t intersectionCount = line.countIntersections(GROUP1);
        for (uint8_t i = 0; i < intersectionCount; ++i) {
            Intersection* intersection = line.getIntersection(i, GROUP1);
            if (intersection == nullptr) {
                return fail("Line intersection list should not contain null pointers");
            }
            if (intersection->numPorts != 2) {
                return fail("Line intersections should expose exactly 2 ports");
            }
            if (countConnectedPorts(*intersection) != 2) {
                return fail("Line intersections should have both ports connected");
            }
        }
    }

    // Topology setup: cross should include center intersection and expected segment types.
    {
        Cross cross(CROSS_PIXEL_COUNT);
        const uint16_t expectedMediumLength = static_cast<uint16_t>(cross.pixelCount / 4 - 3);
        const uint16_t expectedLongLength = static_cast<uint16_t>(cross.pixelCount / 2 - 2);
        if (cross.countIntersections(GROUP1) != 5) {
            return fail("Cross topology should create 5 intersections");
        }
        if (cross.countConnections(GROUP1) != 8) {
            return fail("Cross topology should create 8 connections");
        }
        if (cross.getModel(C_DEFAULT) == nullptr ||
            cross.getModel(C_HORIZONTAL) == nullptr ||
            cross.getModel(C_VERTICAL) == nullptr ||
            cross.getModel(C_DIAGONAL) == nullptr) {
            return fail("Cross topology should create all model variants");
        }

        uint8_t zeroLengthConnections = 0;
        uint8_t mediumConnections = 0;
        uint8_t longConnections = 0;
        const uint8_t connectionCount = cross.countConnections(GROUP1);
        for (uint8_t i = 0; i < connectionCount; ++i) {
            Connection* connection = cross.getConnection(i, GROUP1);
            if (connection == nullptr) {
                return fail("Cross connection list should not contain null pointers");
            }
            if (connection->numLeds == 0) {
                ++zeroLengthConnections;
            } else if (connection->numLeds == expectedMediumLength) {
                ++mediumConnections;
            } else if (connection->numLeds == expectedLongLength) {
                ++longConnections;
            }
        }
        if (zeroLengthConnections != 2 || mediumConnections != 4 || longConnections != 2) {
            return fail("Cross topology has unexpected connection-length distribution");
        }

        Intersection* center = findIntersectionByTopPixel(cross, CROSS_PIXEL_COUNT / 4);
        if (center == nullptr) {
            return fail("Cross topology should include a center intersection at pixel 72");
        }
        if (center->bottomPixel != static_cast<int16_t>(CROSS_PIXEL_COUNT / 4 * 3)) {
            return fail("Cross center intersection has unexpected bottom pixel");
        }
        if (center->numPorts != 4 || countConnectedPorts(*center) != 4) {
            return fail("Cross center intersection should expose 4 connected ports");
        }

        const std::array<uint16_t, 4> edgePixels = {0, CROSS_PIXEL_COUNT / 2 - 1, CROSS_PIXEL_COUNT / 2, CROSS_PIXEL_COUNT - 1};
        for (uint16_t edgePixel : edgePixels) {
            Intersection* edge = findIntersectionByTopPixel(cross, edgePixel);
            if (edge == nullptr) {
                return fail("Cross topology missing expected edge intersection");
            }
            if (edge->numPorts != 3 || countConnectedPorts(*edge) != 3) {
                return fail("Cross edge intersections should expose 3 connected ports");
            }
        }
    }

    // Crossing behavior: deterministic horizontal routing should pass through center and stay on horizontal branch.
    {
        Cross cross(CROSS_PIXEL_COUNT);
        State state(cross);
        state.lightLists[0]->visible = false;

        Model* horizontalModel = cross.getModel(C_HORIZONTAL);
        if (horizontalModel == nullptr) {
            return fail("Cross horizontal model is missing");
        }

        Intersection* left = findIntersectionByTopPixel(cross, 0);
        Intersection* center = findIntersectionByTopPixel(cross, CROSS_PIXEL_COUNT / 4);
        Intersection* right = findIntersectionByTopPixel(cross, CROSS_PIXEL_COUNT / 2 - 1);
        if (left == nullptr || center == nullptr || right == nullptr) {
            return fail("Cross horizontal route endpoints could not be resolved");
        }

        const uint16_t expectedMediumLength = static_cast<uint16_t>(cross.pixelCount / 4 - 3);
        const uint16_t expectedLongLength = static_cast<uint16_t>(cross.pixelCount / 2 - 2);
        Connection* leftToCenter = findConnectionByTopPixels(cross, left->topPixel, center->topPixel, expectedMediumLength);
        Connection* centerToRight = findConnectionByTopPixels(cross, center->topPixel, right->topPixel, expectedMediumLength);
        Connection* leftToRightBridge = findConnectionByTopPixels(cross, left->topPixel, right->topPixel, expectedLongLength);
        if (leftToCenter == nullptr || centerToRight == nullptr || leftToRightBridge == nullptr) {
            return fail("Cross horizontal routing connections are missing");
        }

        Port* leftToCenterPortAtLeft = portAtIntersection(leftToCenter, left);
        Port* bridgePortAtLeft = portAtIntersection(leftToRightBridge, left);
        Port* centerToRightPortAtCenter = portAtIntersection(centerToRight, center);
        if (leftToCenterPortAtLeft == nullptr || bridgePortAtLeft == nullptr || centerToRightPortAtCenter == nullptr) {
            return fail("Unable to resolve ports needed for deterministic cross routing");
        }

        horizontalModel->clearWeights();
        horizontalModel->defaultW = 0;
        horizontalModel->setRoutingStrategy(RoutingStrategy::Deterministic);
        horizontalModel->put(leftToCenterPortAtLeft, 40);
        horizontalModel->put(bridgePortAtLeft, 1);
        horizontalModel->put(centerToRightPortAtCenter, 40);

        EmitParams params(C_HORIZONTAL, 1.0f, 0x00FF00);
        params.setLength(1);
        params.linked = false;
        params.from = findIntersectionIndexByTopPixel(cross, left->topPixel);
        if (params.from < 0) {
            return fail("Could not resolve source intersection index for cross routing test");
        }

        if (state.emit(params) < 0) {
            return fail("Cross routing emit failed unexpectedly");
        }

        bool sawCenter = false;
        bool sawRightHorizontal = false;
        bool sawVerticalBranch = false;
        for (int frame = 0; frame < 80; ++frame) {
            advanceFrame(state);
            if (isNonBlack(state.getPixel(center->topPixel))) {
                sawCenter = true;
            }
            if (countLitPixels(state, static_cast<uint16_t>(center->topPixel + 1), CROSS_PIXEL_COUNT / 2) > 0) {
                sawRightHorizontal = true;
            }
            if (countLitPixels(state, CROSS_PIXEL_COUNT / 2, CROSS_PIXEL_COUNT) > 0) {
                sawVerticalBranch = true;
            }
        }

        if (!sawCenter) {
            return fail("Cross routing never rendered the center intersection");
        }
        if (!sawRightHorizontal) {
            return fail("Cross routing never rendered pixels on the right horizontal branch");
        }
        if (sawVerticalBranch) {
            return fail("Cross routing leaked into the vertical branch in horizontal scenario");
        }
    }

    // Emit scenario: intersection emission should first render at emitter intersection pixel.
    {
        Line line(30);
        State state(line);
        state.lightLists[0]->visible = false;

        EmitParams params(L_BOUNCE, 1.0f, 0xCC2200);
        params.setLength(1);
        params.linked = false;
        params.from = findIntersectionIndexByTopPixel(line, 0);
        if (params.from < 0) {
            return fail("Unable to resolve line intersection index for emit test");
        }

        if (state.emit(params) < 0) {
            return fail("Line intersection emit failed unexpectedly");
        }
        advanceFrame(state);

        const ColorRGB sourceColor = state.getPixel(0);
        if (!isApproxColor(sourceColor, 0xCC, 0x22, 0x00, 1)) {
            return fail(
                "Intersection emit did not render expected color on source pixel (actual: " +
                std::to_string(sourceColor.R) + "," +
                std::to_string(sourceColor.G) + "," +
                std::to_string(sourceColor.B) + ")");
        }
        if (isNonBlack(state.getPixel(1))) {
            return fail("Intersection emit should not start on the first connection LED");
        }
    }

    // Emit scenario: connection emission + offset should start on shifted physical LED.
    {
        Line line(30);
        State state(line);
        state.lightLists[0]->visible = false;

        const int physicalConnectionIndex = findPhysicalConnectionIndex(line);
        if (physicalConnectionIndex < 0) {
            return fail("Unable to resolve physical line connection index");
        }

        EmitParams params(L_BOUNCE, 1.0f, 0x00CC00);
        params.setLength(1);
        params.linked = false;
        params.behaviourFlags = B_EMIT_FROM_CONN;
        params.from = physicalConnectionIndex;
        params.emitOffset = 5;

        if (state.emit(params) < 0) {
            return fail("Line connection emit with offset failed unexpectedly");
        }
        advanceFrame(state);

        if (!isApproxColor(state.getPixel(6), 0x00, 0xCC, 0x00, 1)) {
            return fail("Connection emit with offset did not render at expected pixel");
        }
        if (isNonBlack(state.getPixel(1))) {
            return fail("Connection emit offset should shift away from the unshifted origin pixel");
        }
    }

    // Emit scenario: render-segment mode should paint the full connection span (including endpoints).
    {
        Line line(30);
        State state(line);
        state.lightLists[0]->visible = false;

        const int physicalConnectionIndex = findPhysicalConnectionIndex(line);
        if (physicalConnectionIndex < 0) {
            return fail("Unable to resolve physical line connection for segment test");
        }

        EmitParams params(L_BOUNCE, 1.0f, 0x2255FF);
        params.setLength(1);
        params.linked = false;
        params.behaviourFlags = static_cast<uint16_t>(B_EMIT_FROM_CONN | B_RENDER_SEGMENT);
        params.from = physicalConnectionIndex;

        if (state.emit(params) < 0) {
            return fail("Line render-segment emit failed unexpectedly");
        }
        advanceFrame(state);

        if (!isNonBlack(state.getPixel(0)) || !isNonBlack(state.getPixel(29))) {
            return fail("Render-segment emit should include both line endpoints");
        }
        if (countLitPixels(state, 0, 30) < 20) {
            return fail("Render-segment emit did not light enough pixels");
        }
    }

    // Emit scenario: mirror-rotate mode should render mirrored pixel pairs on line topology.
    {
        Line line(30);
        State state(line);
        state.lightLists[0]->visible = false;

        EmitParams params(L_BOUNCE, 1.0f, 0x0000EE);
        params.setLength(1);
        params.linked = false;
        params.behaviourFlags = B_MIRROR_ROTATE;
        params.from = findIntersectionIndexByTopPixel(line, 0);
        if (params.from < 0) {
            return fail("Unable to resolve line intersection for mirror scenario");
        }

        if (state.emit(params) < 0) {
            return fail("Mirror-rotate emit failed unexpectedly");
        }
        advanceFrame(state);

        if (!isApproxColor(state.getPixel(0), 0x00, 0x00, 0xEE, 1)) {
            return fail("Mirror-rotate emit did not render expected source pixel color");
        }
        if (!isApproxColor(state.getPixel(29), 0x00, 0x00, 0xEE, 1)) {
            return fail("Mirror-rotate emit did not render expected mirrored pixel color");
        }
    }

    // Emit scenario: max brightness should scale rendered LED intensity.
    {
        Line line(30);
        State state(line);
        state.lightLists[0]->visible = false;

        EmitParams params(L_BOUNCE, 1.0f, 0xC86432);
        params.setLength(1);
        params.linked = false;
        params.maxBri = 128;
        params.from = findIntersectionIndexByTopPixel(line, 0);
        if (params.from < 0) {
            return fail("Unable to resolve line intersection for brightness scenario");
        }

        if (state.emit(params) < 0) {
            return fail("Brightness-limited emit failed unexpectedly");
        }
        advanceFrame(state);

        const ColorRGB actual = state.getPixel(0);
        if (!isApproxColor(actual, 100, 50, 25, 2)) {
            return fail("Brightness-limited emit rendered unexpected color intensity");
        }
    }

    return 0;
}
