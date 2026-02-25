# Lightgraph

## What It Is

Lightgraph is a graph-native lighting engine for installations that are not just straight LED strips.

It treats a lighting piece as a connected structure and lets light move through that structure as if it were a spatial system. You can think in terms of flow, branching, and movement across a sculpture, facade, or 3D object, instead of programming each strip as isolated lines.

It solves a common problem in advanced LED work: once a project has intersections, loops, branches, and non-linear geometry, traditional strip-first tooling becomes hard to manage. Lightgraph gives you one abstraction for those layouts.

## Core Concept

The core idea is simple:

- Your LED installation is modeled as a graph (a mesh of connected segments).
- Intersections are nodes.
- LED segments between intersections are edges.
- Light events travel through this graph over time.

This means animation is not limited to left-to-right strip playback. Motion can route through arbitrary topology, making the installation itself part of the composition logic.

### Topology Building Blocks

- `Intersection`: a junction where light arrives and a next direction is chosen.
- `Connection`: a path segment between two intersections, mapped to a run of LEDs.
- `Bridge`: a convenience way to create a simple path between two pixel endpoints (it creates the endpoints and links them).
- `Model` (with `Weight`s): a routing profile that defines how likely each outgoing path is at a junction.

## Why It's Different

Most traditional LED strip workflows are optimized for linear channels and fixture maps. They are effective for many systems, but the mental model is still strip-first.

Lightgraph is topology-first:

- Geometry is a primary input.
- Routing through branches and intersections is built into the model.
- Complex structures can be programmed as one connected light space.

## Example Use Cases

- LED sculpture: route light through branching physical structures.
- 3D installations: drive volumetric pieces where movement should travel in depth.
- Architectural lighting: treat corridors, columns, and facade segments as a connected network.
- Generative light art: let topology influence emergent behavior.

## Quick Start (2 Minutes)

```bash
git submodule update --init --recursive
cmake -S . -B build -DLIGHTGRAPH_CORE_BUILD_EXAMPLES=ON -DLIGHTGRAPH_CORE_BUILD_TESTS=OFF
cmake --build build --target lightgraph_core_minimal_example --parallel
./build/lightgraph_core_minimal_example
```

If the example exits with code `0`, the engine emitted valid non-black pixel output.

### Minimal API Example

```cpp
#include <lightgraph/lightgraph.hpp>

int main() {
    lightgraph::EngineConfig config;
    config.object_type = lightgraph::ObjectType::Line;
    config.pixel_count = 64;

    lightgraph::Engine engine(config);

    lightgraph::EmitCommand emit;
    emit.model = 0;
    emit.length = 8;
    emit.speed = 1.0f;
    emit.color = 0x33CC99;

    if (!engine.emit(emit)) {
        return 1;
    }

    engine.tick(16);
    return engine.pixel(0) ? 0 : 1;
}
```

A compiling example is provided in `examples/minimal_usage.cpp`.
For source-level topology/runtime integration, see `examples/integration_host_loop.cpp`.

## Built-in Topology Objects

Currently defined objects:

- `Heptagon919`
- `Heptagon3024`
- `Line`
- `Cross`
- `Triangle`
- `HeptagonStar` (shared integration-layer base used by the heptagon variants)

Stable `lightgraph::Engine` `ObjectType` values:
`Heptagon919`, `Heptagon3024`, `Line`, `Cross`, `Triangle`.

## Defining New Topologies

Fast path:

1. Create `src/objects/<YourObject>.h` and `.cpp` inheriting `TopologyObject`.
2. In `setup()`, define graph structure with `addIntersection(...)`, `addConnection(...)`, and optional `addBridge(...)`.
3. Add one or more `Model`s and tune routing with `model->put(...)` weights.
4. Optionally override `getModelParams(...)`, `getParams(...)`, and mirror helpers.
5. Expose through integration and/or stable API surface.

Full authoring guide and scaffold:
- [docs/TOPOLOGY_AUTHORING.md](docs/TOPOLOGY_AUTHORING.md)

## Where To Go Next

- Build and integration details: [docs/BUILD_AND_INTEGRATION.md](docs/BUILD_AND_INTEGRATION.md)
- Development workflows (CI, analysis, coverage, benchmarks): [docs/DEVELOPMENT_WORKFLOWS.md](docs/DEVELOPMENT_WORKFLOWS.md)
- Topology authoring guide: [docs/TOPOLOGY_AUTHORING.md](docs/TOPOLOGY_AUTHORING.md)
- API reference: [docs/API.md](docs/API.md)
- API compatibility policy: [docs/API_POLICY.md](docs/API_POLICY.md)
- Packaging guide: [docs/PACKAGING.md](docs/PACKAGING.md)
- Release process: [docs/RELEASE.md](docs/RELEASE.md)
- Migration notes: [MIGRATION.md](MIGRATION.md)
- Changelog: [CHANGELOG.md](CHANGELOG.md)
