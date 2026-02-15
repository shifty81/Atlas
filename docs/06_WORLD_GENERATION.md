# Atlas Engine — World Generation

## Core Principle

You do not edit the generated world.
You edit the **generator graph + parameters**.
The world is a cache, not the source of truth.

## World Layout Interface

The engine does not know if a world is 2D, 3D, voxel, planet, or graph-based.
Everything goes through the `WorldLayout` interface.

### Layout Types

| Layout             | Description       |
|--------------------|-------------------|
| `linear_1d`        | Side scroller     |
| `grid_2d`          | Zelda / Stardew   |
| `voxel_3d`         | Minecraft          |
| `cube_sphere`      | Planet surfaces    |
| `orbital_graph`    | Space systems      |
| `hierarchical_cells` | Strategy maps   |

## Planet Generation

- Cube-sphere mapping (6 faces, quadtree per face)
- LOD rings based on camera distance
- Generated via Graph VM
- Editable chunks with delta overlays
- Runtime deformation support

### Cube-Sphere Coordinate

```cpp
enum CubeFace { POS_X, NEG_X, POS_Y, NEG_Y, POS_Z, NEG_Z };

struct CubeSphereCoord {
    CubeFace face;
    int x, y;
    int lod;
};
```

## Galaxy Generation

- Spiral galaxy templates
- Up to 100,000+ solar systems
- Systems generated on demand (seed + rules)
- Only seed, rules, and player changes are persisted

## Chunk Streaming

Worlds are streamed in chunks. You never load the whole planet.
The `WorldStreamer` class manages chunk lifecycle:

- **Loading**: Chunks requested based on viewer proximity
- **Loaded**: Chunk data resident in memory
- **Cached**: Chunk data saved to disk, freed from memory
- **Unloaded**: No data in memory or pending

```cpp
WorldStreamer streamer(layout, "cache/world");
streamer.Update(viewerPos, lod, loadRadius, unloadRadius);
```

### Disk Cache

The streamer supports transparent disk caching. When a chunk is unloaded, its data is
written to a binary file. When the chunk is needed again, it is loaded from disk instead
of regenerated.

### Generation Flow

```
Chunk Request → WorldStreamer → NoiseGenerator → TerrainMeshGenerator → ChunkData
```

## Terrain Mesh Generation

The `TerrainMeshGenerator` produces vertex/index data for terrain chunks:

```cpp
TerrainMesh mesh = TerrainMeshGenerator::Generate(chunk, resolution, chunkSize, heightFn);
```

- Generates a grid of `resolution × resolution` quads
- Supports pluggable height functions
- Computes per-vertex normals from face normals

## Noise Generation

The `NoiseGenerator` provides deterministic procedural noise:

- **Perlin2D**: Classic 2D Perlin noise with configurable seed
- **FBM2D**: Fractal Brownian Motion layering multiple Perlin octaves

```cpp
float height = NoiseGenerator::FBM2D(x, z, octaves, lacunarity, persistence, seed);
```

All noise functions are deterministic given the same seed, supporting the engine's
determinism-first design principle.

## Galaxy Generation

The `GalaxyGenerator` produces spiral galaxy star systems:

- Configurable arm count, spread, rotation
- Core density bias for central bulge
- Deterministic from seed
- Region-filtered queries for streaming

```cpp
GalaxyParams params;
params.armCount = 4;
params.systemCount = 100000;
auto systems = GalaxyGenerator::Generate(params);
```

## GameType Templates

Game types define how worlds are generated, structured, streamed, and simulated:

```json
{
  "id": "planet_demo",
  "type": "GameType",
  "data": {
    "world_layout": "cube_sphere",
    "chunk_schema": "planet_height_chunks",
    "generation_graph": "planet_height_v1",
    "streaming_policy": "planet_streaming",
    "camera_profile": "orbital_follow"
  }
}
```
