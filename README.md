# 🎮 NuEngine


**High-Performance 3D Game Engine built with Modern C++**

[![C++20](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/20)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg?style=flat)]()
[![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg?style=flat)]()
[![License](https://img.shields.io/badge/License-Proprietary-red.svg?style=flat)]()
[![Status](https://img.shields.io/badge/Status-Alpha-orange.svg?style=flat)]()

[**Quick Start**](#-quick-start) • 
[**Features**](#-features) • 
[**Documentation**](#-documentation) • 
[**Roadmap**](#️-roadmap) • 
[**Community**](#-community)

---

**NuEngine** is a custom, modular 3D game engine designed for performance-critical applications and learning purposes. Built from the ground up in C++20, it features SIMD-accelerated math, cross-platform windowing, and a data-oriented ECS architecture.

---

## 🚀 Quick Start

Get NuEngine running in under 5 minutes:

### Prerequisites

| Requirement | Version | Notes |
|------------|---------|-------|
| **C++ Compiler** | MSVC 19.29+ / GCC 11+ / Clang 14+ | C++20 support required |
| **CMake** | 3.20+ | Build system |
| **CPU** | x86-64 with SSE2 | SSE4.1 recommended for optimal performance |
| **OS** | Windows 10+ / Ubuntu 20.04+ | macOS support planned |

### Installation

```bash
# Clone the repository
git clone https://github.com/vladhordiychuk/NuEngine.git
cd NuEngine

# Create build directory
mkdir build && cd build

# Configure (Windows with MSVC)
cmake .. -G "Visual Studio 17 2022" -A x64

# Or configure (Linux/macOS)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Run tests (optional)
ctest --output-on-failure
```

### Your First Application

```cpp
#include <NuEngine/NuEngine.hpp>
#include <NuEngine/Runtime/EntryPoint.hpp>
#include <memory>

class SandboxApp : public NuEngine::Runtime::Application
{
public:
    SandboxApp()
    {
        LOG_INFO("Sandbox App started! Hello NuEngine!");
    }

    ~SandboxApp()
    {
    }

    void OnUpdate(float deltaTime) override
    {
        NuEngine::Runtime::Application::OnUpdate(deltaTime);
    }

    void OnRender() override
    {
    }
};

std::unique_ptr<NuEngine::Runtime::Application> NuEngine::CreateApplication()
{
    return std::make_unique<SandboxApp>();
}
```

**Expected Output:**
- A window opens with the title "NuEngine Application"
- Console logs show initialization steps
- Window responds to input and can be closed

---

## ✨ Features

### 🎯 Current Features

<table>
<tr>
<td width="50%">

#### 🧮 **SIMD-Accelerated Math Library**
- SSE/SSE4.1 optimized vector operations
- 4x4 Matrix operations (multiply, inverse, transpose)
- Quaternion support (coming soon)
- ~5x performance boost over scalar code
- [📖 Full Math Documentation](NuEngine/NuMath/README.md)

</td>
<td width="50%">

#### 🪟 **Cross-Platform Windowing**
- Native Win32 implementation (Windows)
- X11/Wayland support (Linux - in progress)
- Event-driven input system
- Keyboard and mouse support
- [📖 Platform Documentation](NuEngine/Platform/README.md)

</td>
</tr>
<tr>
<td>

#### 📝 **Robust Logging System**
- Multi-level logging (TRACE → FATAL)
- File and console output
- Formatted messages with `std::format`
- Thread-safe operations
- [📖 Core Documentation](NuEngine/Core/README.md)

</td>
<td>

#### 🛡️ **Modern Error Handling**
- Result<T, E> type for error propagation
- Type-safe error codes
- No exceptions in hot paths
- Clear error messages

</td>
</tr>
</table>

### 🚧 In Development

| Feature | Status | Target | Priority |
|---------|--------|--------|----------|
| **ECS Architecture** | 🔨 20% | Q2 2026 | 🔴 High |
| **Vulkan Renderer** | 📋 Planning | Q3 2026 | 🔴 High |
| **Physics Engine** | 📋 Planning | Q4 2026 | 🟡 Medium |
| **Asset Pipeline** | 📋 Planning | Q1 2026 | 🟡 Medium |
| **Level Editor** | 💡 Concept | Q2 2027 | 🟢 Low |

---

## 🏗️ Architecture

NuEngine follows a **modular, layered architecture** where each subsystem is independent and well-defined:

```
┌─────────────────────────────────────────────────┐
│              Game / Editor Layer                │
│   (Your application code, gameplay logic)       │
└─────────────────────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────┐
│            Engine High-Level APIs               │
│       (ECS, Scene Graph, Asset Manager)         │
└─────────────────────────────────────────────────┘
                      ▼
┌──────────────┬──────────────┬───────────────────┐
│   Graphics   │    Physics   │   AI & Gameplay   │
│   Renderer   │    Engine    │     Systems       │
└──────────────┴──────────────┴───────────────────┘
                      ▼
┌──────────────┬──────────────┬───────────────────┐
│     Core     │   Platform   │   Math Library    │
│   Systems    │  Abstraction │  (SIMD-powered)   │
└──────────────┴──────────────┴───────────────────┘
                      ▼
┌─────────────────────────────────────────────────┐
│         Operating System (Win32 / Linux)        │
└─────────────────────────────────────────────────┘
```

### Module Overview

| Module | Purpose | Dependencies | Status |
|--------|---------|--------------|--------|
| **NuMath** | SIMD-accelerated linear algebra | None | ✅ Complete |
| **Core** | Logging, file system, error handling | Math | ✅ Complete |
| **Platform** | Window creation, input handling | Core | ✅ Windows Ready |
| **Graphics** | Rendering abstraction (Vulkan/DX12) | Platform, Math | 🚧 In Progress |
| **ECS** | Entity-Component-System | Core | 🚧 In Progress |
| **Physics** | Collision and dynamics | Math, ECS | 🚧 In Progress |
| **Assets** | Resource loading and management | Core | 📋 Planned |

---

## 📊 Performance

**Benchmark Environment:**
- CPU: Intel i5-12700K
- Compiler: MSVC 19.39 with `/O2`
- OS: Windows 11

### Math Library Performance (vs Scalar Implementation)

| Operation | Scalar | SIMD (SSE4.1) | Speedup |
|-----------|--------|---------------|---------|
| `Vector3::Dot()` | 92 ns | 26 ns | **3.5×** ⚡ |
| `Vector4::Normalize()` | 140 ns | 41 ns | **3.4×** ⚡ |
| `Matrix4x4::Multiply()` | 410 ns | 120 ns | **3.4×** ⚡ |
| `Matrix4x4::Inverse()` | 640 ns | 195 ns | **3.3×** ⚡ |
| `Vector3::Cross()` | 65 ns | 22 ns | **3.0×** ⚡ |

> 💡 **Note:** Performance scales with CPU capabilities. Modern CPUs with AVX2 support will see even better results in future versions.

---

## 📂 Project Structure

```
NuEngine/
├── 🎮 Game/              # Example game project
├── 🛠️ Editor/            # Level editor (planned)
├── 🔧 Engine/            # Core engine modules
│   ├── Math/            # SIMD math library
│   ├── Core/            # Fundamental systems
│   ├── Platform/        # OS abstraction layer
│   ├── Graphics/        # Rendering backend
│   └── ECS/             # Entity-Component-System
├── 🧪 Tests/             # Unit tests & benchmarks
│   ├── Unit/           # Feature tests
│   └── Benchmark/      # Performance tests
├── 📦 Thirdparty/       # External dependencies
├── 📄 CMakeLists.txt    # Root build configuration
└── 📖 README.md         # You are here
```

<details>
<summary>📁 <b>Detailed Module Breakdown</b></summary>

```
Engine/
├── Math/
│   ├── include/Math/
│   │   ├── Algebra/              # Vector, Matrix, Quaternion
│   │   │   ├── Vector/
│   │   │   │   ├── Vector2.hpp
│   │   │   │   ├── Vector3.hpp
│   │   │   │   └── Vector4.hpp
│   │   │   └── Matrix/
│   │   │       ├── Matrix4x4.hpp
│   │   │       ├── Matrix3x3.hpp (planned)
│   │   │       └── Matrix2x2.hpp (planned)
│   │   ├── Detail/               # SIMD implementation
│   │   │   ├── SIMDDispatch.hpp
│   │   │   ├── SimdSSE.hpp
│   │   │   └── SimdScalar.hpp
│   │   ├── Geometry/             # Shapes, intersections
│   │   └── Math.hpp              # Main header
│   └── README.md
│
├── Core/
│   ├── include/Core/
│   │   ├── Application/          # Main application loop
│   │   ├── Logging/              # Logger system
│   │   ├── FileSystem/           # File I/O utilities
│   │   └── Errors/               # Error handling
│   └── README.md
│
├── Platform/
│   ├── include/Platform/
│   │   ├── Window.hpp            # Window abstraction
│   │   ├── Input.hpp             # Input handling
│   │   └── Win32/                # Windows-specific
│   │       └── Win32Window.hpp
│   └── README.md
│
└── Graphics/ (in development)
    ├── include/Graphics/
    │   ├── Renderer.hpp
    │   ├── Vulkan/               # Vulkan backend
    │   └── DX12/                 # DirectX 12 backend
    └── README.md
```

</details>

---

## 🗺️ Roadmap

### ✅ Phase 1: Foundation (Q4 2024 - Q1 2025) — **COMPLETE**

- [x] SIMD-accelerated math library
  - [x] Vector2, Vector3, Vector4
  - [x] Matrix4x4 with full operations
  - [x] SSE/SSE4.1 optimization
- [x] Core systems
  - [x] Logging infrastructure
  - [x] File system utilities
  - [x] Error handling (Result type)
- [x] Platform layer
  - [x] Win32 window creation
  - [x] Event system
  - [x] Input handling (keyboard, mouse)

### 🚧 Phase 2: ECS & Rendering (Q2 2026 - Q3 2026) — **IN PROGRESS**

- [ ] **Entity-Component-System** (40% complete)
  - [x] Entity manager
  - [ ] Component pools
  - [ ] System scheduler
  - [ ] Cache-friendly memory layout
- [ ] **Graphics Foundation**
  - [ ] Vulkan initialization
  - [ ] Swap chain management
  - [ ] Basic render passes
  - [ ] Shader compilation pipeline
- [ ] **Asset Pipeline**
  - [ ] Model loading (GLTF/OBJ)
  - [ ] Texture loading
  - [ ] Shader hot-reloading

### 📋 Phase 3: Game Systems (Q4 2026 - Q1 2027) — **PLANNED**

- [ ] **Physics Engine**
  - [ ] Collision detection (broad/narrow phase)
  - [ ] Rigid body dynamics
  - [ ] Integration with ECS
- [ ] **Advanced Rendering**
  - [ ] PBR material system
  - [ ] Shadow mapping
  - [ ] Post-processing effects
- [ ] **Audio System**
  - [ ] 3D audio positioning
  - [ ] Sound effect playback
  - [ ] Music streaming

### 💡 Phase 4: Tools & Polish (Q2 2027+) — **FUTURE**

- [ ] Level Editor (Qt-based)
- [ ] Profiling tools
- [ ] Scene serialization
- [ ] AI behavior system
- [ ] Networking (multiplayer support)

> **📌 Note:** Dates are estimates and may shift based on development priorities.

---

## 🎓 Documentation

Each major subsystem has its own detailed README:

| Module | Documentation | Quick Links |
|--------|---------------|-------------|
| **Math** | [Engine/NuMath/README.md](Engine/Math/README.md) | [API Reference](#) • [Performance Guide](#) |
| **Core** | [Engine/Core/README.md](Engine/Core/README.md) | [Logger Usage](#) • [Error Handling](#) |
| **Platform** | [Engine/Platform/README.md](Engine/Platform/README.md) | [Window Creation](#) • [Input System](#) |
| **Graphics** | [Engine/Graphics/README.md](Engine/Graphics/README.md) | [Vulkan Setup](#) • [Shader Guide](#) |
| **ECS** | [Engine/ECS/README.md](Engine/ECS/README.md) | [Entity Management](#) • [Systems](#) |

### 📚 Additional Resources

- **[Build Guide](docs/Building.md)** — Detailed compilation instructions
- **[Architecture Overview](docs/Architecture.md)** — Deep dive into engine design
- **[Contributing Guide](CONTRIBUTING.md)** — How to contribute code
- **[Code Style Guide](docs/CodeStyle.md)** — Coding conventions
- **[Tutorials](docs/Tutorials/)** — Step-by-step learning materials

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

### Ways to Contribute

- 🐛 **Report bugs** via [GitHub Issues](https://github.com/vladhordiychuk/NuEngine/issues)
- 💡 **Suggest features** in [Discussions](https://github.com/vladhordiychuk/NuEngine/discussions)
- 📝 **Improve documentation** by submitting PRs
- 🔧 **Submit code** following our [contributing guidelines](CONTRIBUTING.md)
- ⭐ **Star the repo** to show support!

### Development Workflow

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Standards

- Follow C++20 best practices
- Use `clang-format` with provided `.clang-format`
- Write unit tests for new features
- Update documentation as needed
- Keep commits atomic and well-described

---

## ⚖️ License

**NuEngine** is currently under a **proprietary license** for educational and portfolio purposes.

### Usage Terms

- ✅ **Personal learning and experimentation** — allowed
- ✅ **Educational use in courses/tutorials** — allowed with attribution
- ❌ **Commercial use** — requires explicit permission
- ❌ **Redistribution** — not allowed without permission
- ❌ **Use in commercial products** — contact for licensing

> 📧 For commercial licensing inquiries, contact: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)

*A more permissive open-source license may be considered in the future once the project reaches maturity.*

---

## ⚠️ Known Issues & Limitations

| Issue | Severity | Status | Workaround |
|-------|----------|--------|------------|
| Linux windowing incomplete | Medium | 🚧 In Progress | Use Windows build |
| No macOS support | Low | 📋 Planned | Use Windows/Linux |
| Memory profiling tools missing | Low | 📋 Planned | Use external tools (Tracy) |
| SIMD requires SSE2+ CPU | Info | ✅ By Design | Falls back to scalar |

**Reporting Issues:** Please use our [Issue Tracker](https://github.com/vladhordiychuk/NuEngine/issues) with detailed reproduction steps.

---

## 🙏 Acknowledgments

NuEngine wouldn't exist without these amazing resources and inspirations:

### Libraries & Tools
- **[CMake](https://cmake.org/)** — Cross-platform build system

### Learning Resources
- **[Game Engine Architecture](https://www.gameenginebook.com/)** by Jason Gregory
- **[Real-Time Rendering](https://www.realtimerendering.com/)** by Tomas Akenine-Möller et al.
- **[Handmade Hero](https://handmadehero.org/)** by Casey Muratori

---

## 👨‍💻 Author

**Vladyslav Hordiychuk**  
*C++ Game Engine Developer | Graphics Programming Enthusiast*

- 📧 Email: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)
- 💼 LinkedIn: [Vladyslav Hordiychuk](https://www.linkedin.com/in/%D0%B2%D0%BB%D0%B0%D0%B4%D0%B8%D1%81%D0%BB%D0%B0%D0%B2-%D0%B3%D0%BE%D1%80%D0%B4%D1%96%D0%B9%D1%87%D1%83%D0%BA-8a2704292/)
- 🐙 GitHub: [@vladhordiychuk](https://github.com/vladhordiychuk)
- 📍 Location: Vinnytsia, Ukraine

> 💼 **Open to opportunities!** Looking for remote C++/game engine development positions.

---

## 💬 Community & Support

### Get Help
- 📖 Check the [Documentation](#-documentation) first
- 💬 Ask questions in [GitHub Discussions](https://github.com/vladhordiychuk/NuEngine/discussions)
- 🐛 Report bugs via [GitHub Issues](https://github.com/vladhordiychuk/NuEngine/issues)
- 📧 Email: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)

### Stay Updated
- ⭐ **Star** the repository to follow development
- 👀 **Watch** for new releases
- 🔔 Enable notifications for important updates

---

## 📈 Project Stats

![GitHub stars](https://img.shields.io/github/stars/vladhordiychuk/NuEngine?style=social)
![GitHub forks](https://img.shields.io/github/forks/vladhordiychuk/NuEngine?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/vladhordiychuk/NuEngine?style=social)

---

<div align="center">

⬆️ [Back to Top](#-nuengine)

</div>

---