# 🗝️ Ofir & Georgy's Adventure — A C++ Console Game Engine

A two-player cooperative dungeon-adventure game, engineered from scratch in **C++** with **zero external game libraries** — every system you see (rendering, physics, lighting, collision, input, save/replay) is hand-built directly on top of the Windows console.

Built by **Ofir Yehonatan** & **Georgy Frolov** as a systems-programming project focused on clean C++ architecture, deterministic simulation, and real git/GitHub collaboration.

---

## ✨ Highlights

- 🕹️ **Local two-player co-op** — independent movement, health, score, and inventory for each player on a shared 80×25 grid
- 💡 **Dynamic lighting engine** — levels can plunge into darkness, with real-time radius-based visibility computed around a held torch
- 🌀 **Physics-driven puzzles** — compressible springs that store energy and launch a player on release; multi-cell obstacles that can be pushed with directional collision resolution
- 🔐 **Progression systems** — key-gated doors, pressure-switch puzzles, and text-based riddles that must be solved to advance
- 💣 **Timed hazards** — fused bombs with countdown timers and blast-radius explosion logic
- ❤️ **Live HUD** — a dedicated legend panel rendering real-time health bars and score without flicker
- 🧪 **Deterministic replay & automated testing** — every run can be recorded (seed + per-cycle input) and replayed frame-for-frame, emitting structured event logs for CI-style regression testing with no human at the keyboard

---

## 🏗️ Architecture

The codebase is organized into focused, single-responsibility modules:

| Module | Responsibility |
|---|---|
| `Game` | Menu, main loop, mode selection, save/load orchestration |
| `Level` | Map state, doors, switches, lighting, riddle/spring/obstacle registries |
| `Player` | Position, movement, inventory, health, score, spring physics |
| `Obstacle` | Multi-cell pushable objects with collision resolution |
| `Spring` | Compression/release mechanics and launch physics |
| `Bomb` | Fuse countdown and explosion logic |
| `Torch` | Radius-based light source calculations |
| `Riddle` | Question/answer puzzle state |
| `io_utils` / `Color` | Low-level console rendering and ANSI color helpers |

This separation keeps gameplay systems testable and independently extensible — new hazards, puzzle types, or levels can be added without touching the core game loop.

---

## 🎮 Controls

| Action | Player 1 | Player 2 |
|---|:---:|:---:|
| Up | `W` | `I` |
| Down | `X` | `M` |
| Left | `A` | `J` |
| Right | `D` | `L` |
| Stay / Hold | `S` | `K` |
| Dispose Item | `E` | `O` |

**Map legend:** Wall (`W`) · Key (`K`) · Door (`1`–`9`) · Spring (`#`) · Obstacle (`*`) · Torch (`!`)

- **Door 1** requires 2 keys to unlock
- **Door 2** opens only while both pressure switches are held simultaneously

---

## 🧪 Deterministic Replay System

The engine ships with a record/replay pipeline built for automated testing, driven entirely from the command line:

```
OfirGeorgyProject.exe -save            # record a live playthrough
OfirGeorgyProject.exe -load -silent    # replay and verify a recorded run headlessly
```

- **`adv-world.steps`** — the random seed plus every `[cycle, key, optional-answer]` input, enough to reproduce a run bit-for-bit
- **`adv-world.result`** — a structured event log (`RIDDLE`, `LIFE_LOST`, `LEVEL_COMPLETE`, `DEATH`, `GAME_END`) emitted during playback

Running a recorded `.steps` file with `-load -silent` replays it against the expected `.result` file and prints **`Test Finished.`** on success — enabling regression testing of gameplay logic without any manual input. Example recordings are included under [`OfirGeorgyProject/examples_steps_result`](OfirGeorgyProject/examples_steps_result).

---

## 🚀 Getting Started

**Requirements:** Windows + Visual Studio (C++ Desktop Development workload)

1. Clone the repository
2. Open [`OfirGeorgyProject.sln`](OfirGeorgyProject/OfirGeorgyProject.sln) in Visual Studio
3. Build and run (`Ctrl+F5`)

> ⚠️ To use `-save` / `-load`, run the compiled `.exe` from the project's main folder (alongside the `.cpp`/`.h` files), so it can find level and riddle data files.

---

## 👥 Authors

- **Ofir Yehonatan**
- **Georgy Frolov**

*A university systems-programming project — and a hands-on lesson in collaborative git workflow along the way.*
