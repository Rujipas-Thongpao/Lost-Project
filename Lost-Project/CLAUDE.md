# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Visual Studio 2022 C++ project. Open `Lost-Project.vcxproj` and build for **x64 Debug** or **x64 Release**. There is no CMake or CLI build system — all builds go through VS2022 (toolset v143).

Dependencies are managed via vcpkg: GLFW3, GLAD (OpenGL), GLM, Assimp, stb_image, ImGui, fmt, pugixml, rapidjson.

The executable outputs to `x64/Debug/` or `x64/Release/`.

## Architecture

**LOST** is a 3D wave-based action roguelike using OpenGL 3.3 core, targeting 120 FPS at 800×800. It uses a manual ECS (Entity-Component-System) pattern — not a framework, just hand-rolled stores and systems.

### Core ECS Pattern

- **Entity**: a `uint16_t` ID + name + destroy flag (`Entity.h`)
- **ComponentStore** (`ComponenetStore.h` — note typo in filename): `unordered_map<EntityID, T>` template; each system holds its own store
- **Tags** (`Tags.h`, `TagStore.h`): categorize entities (Player, Enemy, Camera, etc.) for system queries
- **EntityManager**: creates/tracks all live entities

### Game Loop

`main.cpp` runs the GLFW loop → `Game::Update(dt)` in `Game.cpp`. System update order is deterministic:

```
TransformSystem → CameraSystem → ColliderSystem → GunSystem → EnemySystem
→ WaveSystem → StatSystem → HealthSystem → AnimationSystem → ParticleSystem
→ RendererSystem::Render() → RendererSystem::RenderParticles() → GUI::Update()
```

`Game` is a singleton that owns all system and component store instances.

### World / Entity Setup

All entities (player, enemies, terrain, trees, grass) are created in `World::Init()` (`World.cpp`). This is the entry point for understanding what components any entity has. Enemies are spawned dynamically in `WaveSystem::SpawnEnemies()`.

### Key Systems

| System | File | Notes |
|--------|------|-------|
| Rendering | `RendererSystem.h/cpp` | OpenGL framebuffer, draws meshes and particles |
| Animation | `AnimationSystem.h/cpp`, `animation.h` | Skeletal animation, bone matrices, idle↔walk state machine |
| Stats/Buffs | `StatSystem.h/cpp`, `StatHelper.h` | Base stats + modifier list → resolved final stats each frame |
| Waves | `WaveSystem.h/cpp` | Wave definitions, enemy spawn, triggers buff selection on clear |
| Gun/Bullets | `GunSystem.h/cpp` | Player firing, bullet lifetime, spread |
| Particles | `ParticleSystem.h/cpp` | Burst/loop modes, 200-particle cap per component |
| GUI | `GUI.h/cpp` | ImGui overlay: health, stamina, buff selection UI |

### Stat/Buff Flow

`StatComponent` holds base values (health, damage, speed, etc.). `StatModifierComponent` holds a list of modifiers (Add / Multiply / Override). Each frame, `StatHelper::ResolveStat()` combines them into final values used by gameplay systems. `WaveSystem` triggers `StatSystem::GiveRandomBuff()` on wave clear → player picks from 3 random perks.

### Asset Loading

- Models loaded via Assimp (`Model.h/cpp`) — supports FBX and OBJ with skeletal animation
- `ModelLoader.h` / `ModeLoader.cpp` (note typo) — asset registry, deduplicates loaded models
- `ResourceManager.h` — global texture/shader cache
- `Shader.h/cpp` — GLSL compile + uniform setters
