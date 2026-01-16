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

## 🌐 Languages | Мови

> This README is available in multiple languages:
> - 🇬🇧 **[English](#english-version)** (Current)
> - 🇺🇦 **[Українська](#українська-версія)** (Scroll down)

---

<a name="english-version"></a>

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
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

using namespace NuEngine;

int main()
{
    // Initialize logger
    auto logResult = Core::Logger::Init("logs/app.log");
    if (logResult.IsError()) {
        std::cerr << "Failed to initialize logger\n";
        return -1;
    }

    LOG_INFO("=== NuEngine Application Started ===");

    // Create application instance
    Core::Application app;
    
    // Math example: Create a view-projection matrix
    Math::Vector3 cameraPos(0.0f, 5.0f, 10.0f);
    Math::Vector3 target = Math::Vector3::Zero();
    Math::Vector3 up = Math::Vector3::UnitY();
    
    auto view = Math::Matrix4x4::CreateLookAt(cameraPos, target, up);
    auto proj = Math::Matrix4x4::CreatePerspective(1.57f, 16.0f/9.0f, 0.1f, 1000.0f);
    auto viewProj = proj * view;
    
    LOG_INFO("Camera initialized at position: {}", cameraPos.ToString());

    // Run the application loop
    auto result = app.Run();
    if (result.IsError()) {
        LOG_ERROR("Application error: {}", Core::ToString(result.UnwrapError()));
        Core::Logger::Shutdown();
        return -1;
    }

    LOG_INFO("=== Application Closed ===");
    Core::Logger::Shutdown();
    return 0;
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
| **ECS Architecture** | 🔨 30% | Q2 2025 | 🔴 High |
| **Vulkan Renderer** | 📋 Planning | Q3 2025 | 🔴 High |
| **Physics Engine** | 📋 Planning | Q4 2025 | 🟡 Medium |
| **Asset Pipeline** | 📋 Planning | Q1 2026 | 🟡 Medium |
| **Level Editor** | 💡 Concept | Q2 2026 | 🟢 Low |

---

## 🏗️ Architecture

NuEngine follows a **modular, layered architecture** where each subsystem is independent and well-defined:

```
┌─────────────────────────────────────────────────┐
│              Game / Editor Layer                │
│  (Your application code, gameplay logic)        │
└─────────────────────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────┐
│            Engine High-Level APIs               │
│  (ECS, Scene Graph, Asset Manager)              │
└─────────────────────────────────────────────────┘
                      ▼
┌──────────────┬──────────────┬───────────────────┐
│   Graphics   │    Physics   │   AI & Gameplay   │
│   Renderer   │    Engine    │   Systems         │
└──────────────┴──────────────┴───────────────────┘
                      ▼
┌──────────────┬──────────────┬───────────────────┐
│     Core     │   Platform   │   Math Library    │
│   Systems    │  Abstraction │   (SIMD-powered)  │
└──────────────┴──────────────┴───────────────────┘
                      ▼
┌─────────────────────────────────────────────────┐
│         Operating System (Win32 / Linux)        │
└─────────────────────────────────────────────────┘
```

### Module Overview

| Module | Purpose | Dependencies | Status |
|--------|---------|--------------|--------|
| **Math** | SIMD-accelerated linear algebra | None | ✅ Complete |
| **Core** | Logging, file system, error handling | Math | ✅ Complete |
| **Platform** | Window creation, input handling | Core | ✅ Windows Ready |
| **Graphics** | Rendering abstraction (Vulkan/DX12) | Platform, Math | 🚧 In Progress |
| **ECS** | Entity-Component-System | Core | 🚧 In Progress |
| **Physics** | Collision and dynamics | Math, ECS | 📋 Planned |
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

### 🚧 Phase 2: ECS & Rendering (Q2 2025 - Q3 2025) — **IN PROGRESS**

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

### 📋 Phase 3: Game Systems (Q4 2025 - Q1 2026) — **PLANNED**

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

### 💡 Phase 4: Tools & Polish (Q2 2026+) — **FUTURE**

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
| **Math** | [Engine/Math/README.md](Engine/Math/README.md) | [API Reference](#) • [Performance Guide](#) |
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
- **[spdlog](https://github.com/gabime/spdlog)** — Fast C++ logging library
- **[Catch2](https://github.com/catchorg/Catch2)** — Modern C++ testing framework
- **[CMake](https://cmake.org/)** — Cross-platform build system

### Learning Resources
- **[Game Engine Architecture](https://www.gameenginebook.com/)** by Jason Gregory
- **[Real-Time Rendering](https://www.realtimerendering.com/)** by Tomas Akenine-Möller et al.
- **[Handmade Hero](https://handmadehero.org/)** by Casey Muratori
- **[The Cherno's Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)**

### Inspiration
- **[Unity Engine](https://unity.com/)** — Component-based design
- **[Unreal Engine](https://www.unrealengine.com/)** — High-performance architecture
- **[Godot Engine](https://godotengine.org/)** — Open-source philosophy

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

<a name="українська-версія"></a>

# 🇺🇦 Українська Версія

# 🎮 NuEngine

**Високопродуктивний 3D ігровий рушій на сучасному C++**

[**Швидкий старт**](#-швидкий-старт) • 
[**Можливості**](#-можливості) • 
[**Документація**](#-документація) • 
[**Дорожня карта**](#️-дорожня-карта) • 
[**Спільнота**](#-спільнота)

---

**NuEngine** — це кастомний, модульний 3D ігровий рушій, розроблений для високопродуктивних застосунків та навчання. Побудований з нуля на C++20, він має SIMD-прискорену математику, кросплатформенне створення вікон та дата-орієнтовану ECS архітектуру.

---

## 🚀 Швидкий старт

Запустіть NuEngine менше ніж за 5 хвилин:

### Системні вимоги

| Вимога | Версія | Примітки |
|--------|--------|----------|
| **C++ Компілятор** | MSVC 19.29+ / GCC 11+ / Clang 14+ | Потрібна підтримка C++20 |
| **CMake** | 3.20+ | Система збірки |
| **Процесор** | x86-64 з SSE2 | SSE4.1 рекомендується для оптимальної продуктивності |
| **ОС** | Windows 10+ / Ubuntu 20.04+ | Підтримка macOS планується |

### Встановлення

```bash
# Клонуйте репозиторій
git clone https://github.com/vladhordiychuk/NuEngine.git
cd NuEngine

# Створіть директорію для збірки
mkdir build && cd build

# Налаштування (Windows з MSVC)
cmake .. -G "Visual Studio 17 2022" -A x64

# Або налаштування (Linux/macOS)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Збірка
cmake --build . --config Release

# Запуск тестів (опціонально)
ctest --output-on-failure
```

### Ваша перша програма

```cpp
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

using namespace NuEngine;

int main()
{
    // Ініціалізація логера
    auto logResult = Core::Logger::Init("logs/app.log");
    if (logResult.IsError()) {
        std::cerr << "Не вдалося ініціалізувати логер\n";
        return -1;
    }

    LOG_INFO("=== Програма NuEngine запущена ===");

    // Створення екземпляру програми
    Core::Application app;
    
    // Приклад математики: створення view-projection матриці
    Math::Vector3 cameraPos(0.0f, 5.0f, 10.0f);
    Math::Vector3 target = Math::Vector3::Zero();
    Math::Vector3 up = Math::Vector3::UnitY();
    
    auto view = Math::Matrix4x4::CreateLookAt(cameraPos, target, up);
    auto proj = Math::Matrix4x4::CreatePerspective(1.57f, 16.0f/9.0f, 0.1f, 1000.0f);
    auto viewProj = proj * view;
    
    LOG_INFO("Камера ініціалізована на позиції: {}", cameraPos.ToString());

    // Запуск головного циклу програми
    auto result = app.Run();
    if (result.IsError()) {
        LOG_ERROR("Помилка програми: {}", Core::ToString(result.UnwrapError()));
        Core::Logger::Shutdown();
        return -1;
    }

    LOG_INFO("=== Програму закрито ===");
    Core::Logger::Shutdown();
    return 0;
}
```

**Очікуваний результат:**
- Відкривається вікно з назвою "NuEngine Application"
- В консолі відображаються логи ініціалізації
- Вікно реагує на введення і може бути закрите

---

## ✨ Можливості

### 🎯 Поточні можливості

<table>
<tr>
<td width="50%">

#### 🧮 **SIMD-прискорена математична бібліотека**
- Оптимізовані векторні операції SSE/SSE4.1
- Операції з матрицями 4x4 (множення, обернення, транспонування)
- Підтримка кватерніонів (незабаром)
- ~5x приріст продуктивності порівняно зі скалярним кодом
- [📖 Повна документація Math](Engine/Math/README.md)

</td>
<td width="50%">

#### 🪟 **Кросплатформенні вікна**
- Нативна реалізація Win32 (Windows)
- Підтримка X11/Wayland (Linux - в розробці)
- Подієва система вводу
- Підтримка клавіатури та миші
- [📖 Документація Platform](Engine/Platform/README.md)

</td>
</tr>
<tr>
<td>

#### 📝 **Надійна система логування**
- Багаторівневе логування (TRACE → FATAL)
- Вивід у файл та консоль
- Форматовані повідомлення через `std::format`
- Потокобезпечні операції
- [📖 Документація Core](Engine/Core/README.md)

</td>
<td>

#### 🛡️ **Сучасна обробка помилок**
- Тип Result<T, E> для передачі помилок
- Типобезпечні коди помилок
- Без винятків у критичних шляхах
- Зрозумілі повідомлення про помилки

</td>
</tr>
</table>

### 🚧 В розробці

| Функція | Статус | Термін | Пріоритет |
|---------|--------|--------|-----------|
| **ECS архітектура** | 🔨 30% | Q2 2025 | 🔴 Високий |
| **Vulkan рендерер** | 📋 Планування | Q3 2025 | 🔴 Високий |
| **Фізичний рушій** | 📋 Планування | Q4 2025 | 🟡 Середній |
| **Asset Pipeline** | 📋 Планування | Q1 2026 | 🟡 Середній |
| **Редактор рівнів** | 💡 Концепція | Q2 2026 | 🟢 Низький |

---

## 🏗️ Архітектура

NuEngine слідує **модульній, пошаровій архітектурі**, де кожна підсистема є незалежною та чітко визначеною:

```
┌─────────────────────────────────────────────────┐
│              Рівень гри / редактора             │
│  (Код вашого застосунку, ігрова логіка)         │
└─────────────────────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────┐
│        Високорівневі API рушія                  │
│  (ECS, граф сцени, менеджер ресурсів)           │
└─────────────────────────────────────────────────┘
                      ▼
┌──────────────┬──────────────┬───────────────────┐
│   Графіка    │    Фізика    │   AI і геймплей   │
│   Рендерер   │    Рушій     |                   |
└──────────────┴──────────────┴───────────────────┘
                       ▼
┌──────────────┬──────────────┬───────────────────┐
│ Ядро         │ Платформа    │ Математична       │
│ Системи      │ Абстракція   │ бібліотека (SIMD) │
└──────────────┴──────────────┴───────────────────┘
                       ▼
┌─────────────────────────────────────────────────┐
│ Операційна система (Win32 / Linux)              │
└─────────────────────────────────────────────────┘
```

### Огляд модулів

| Модуль | Призначення | Залежності | Статус |
|--------|-------------|------------|--------|
| **Math** | SIMD-прискорена лінійна алгебра | Немає | ✅ Завершено |
| **Core** | Логування, файлова система, обробка помилок | Math | ✅ Завершено |
| **Platform** | Створення вікон, обробка вводу | Core | ✅ Windows готово |
| **Graphics** | Абстракція рендерингу (Vulkan/DX12) | Platform, Math | 🚧 В розробці |
| **ECS** | Entity-Component-System | Core | 🚧 В розробці |
| **Physics** | Колізії та динаміка | Math, ECS | 📋 Заплановано |
| **Assets** | Завантаження та управління ресурсами | Core | 📋 Заплановано |

---

## 📊 Продуктивність

**Тестове середовище:**
- Процесор: Intel i5-12700K
- Компілятор: MSVC 19.39 з `/O2`
- ОС: Windows 11

### Продуктивність математичної бібліотеки (порівняно зі скалярною реалізацією)

| Операція | Скалярна | SIMD (SSE4.1) | Прискорення |
|----------|----------|---------------|-------------|
| `Vector3::Dot()` | 92 нс | 26 нс | **3.5×** ⚡ |
| `Vector4::Normalize()` | 140 нс | 41 нс | **3.4×** ⚡ |
| `Matrix4x4::Multiply()` | 410 нс | 120 нс | **3.4×** ⚡ |
| `Matrix4x4::Inverse()` | 640 нс | 195 нс | **3.3×** ⚡ |
| `Vector3::Cross()` | 65 нс | 22 нс | **3.0×** ⚡ |

> 💡 **Примітка:** Продуктивність масштабується з можливостями процесора. Сучасні процесори з підтримкою AVX2 покажуть ще кращі результати в майбутніх версіях.

---

## 📂 Структура проєкту

```
NuEngine/
├── 🎮 Game/              # Приклад ігрового проєкту
├── 🛠️ Editor/            # Редактор рівнів (планується)
├── 🔧 Engine/            # Основні модулі рушія
│   ├── Math/             # SIMD математична бібліотека
│   ├── Core/             # Фундаментальні системи
│   ├── Platform/         # Шар абстракції ОС
│   ├── Graphics/         # Бекенд рендерингу
│   └── ECS/              # Entity-Component-System
├── 🧪 Tests/             # Юніт-тести та бенчмарки
│   ├── Unit/             # Тести функціоналу
│   └── Benchmark/        # Тести продуктивності
├── 📦 Thirdparty/        # Зовнішні залежності
├── 📄 CMakeLists.txt     # Конфігурація збірки
└── 📖 README.md          # Ви тут
```

---

## 🗺️ Дорожня карта

### ✅ Фаза 1: Фундамент (Q4 2024 - Q1 2025) — **ЗАВЕРШЕНО**

- [x] SIMD-прискорена математична бібліотека
  - [x] Vector2, Vector3, Vector4
  - [x] Matrix4x4 з повним набором операцій
  - [x] SSE/SSE4.1 оптимізація
- [x] Основні системи
  - [x] Інфраструктура логування
  - [x] Утиліти файлової системи
  - [x] Обробка помилок (тип Result)
- [x] Платформенний шар
  - [x] Створення вікон Win32
  - [x] Система подій
  - [x] Обробка вводу (клавіатура, миша)

### 🚧 Фаза 2: ECS і рендеринг (Q2 2025 - Q3 2025) — **В РОЗРОБЦІ**

- [ ] **Entity-Component-System** (40% завершено)
  - [x] Менеджер сутностей
  - [ ] Пули компонентів
  - [ ] Планувальник систем
  - [ ] Кеш-орієнтоване розміщення пам'яті
- [ ] **Основи графіки**
  - [ ] Ініціалізація Vulkan
  - [ ] Управління swap chain
  - [ ] Базові render pass
  - [ ] Конвеєр компіляції шейдерів
- [ ] **Asset Pipeline**
  - [ ] Завантаження моделей (GLTF/OBJ)
  - [ ] Завантаження текстур
  - [ ] Гаряче перезавантаження шейдерів

### 📋 Фаза 3: Ігрові системи (Q4 2025 - Q1 2026) — **ЗАПЛАНОВАНО**

- [ ] **Фізичний рушій**
  - [ ] Виявлення колізій (широка/вузька фази)
  - [ ] Динаміка твердих тіл
  - [ ] Інтеграція з ECS
- [ ] **Розширений рендеринг**
  - [ ] Система PBR матеріалів
  - [ ] Мапінг тіней
  - [ ] Постобробка ефектів
- [ ] **Аудіо система**
  - [ ] 3D позиціонування звуку
  - [ ] Відтворення звукових ефектів
  - [ ] Стримінг музики

### 💡 Фаза 4: Інструменти та шліфування (Q2 2026+) — **МАЙБУТНЄ**

- [ ] Редактор рівнів (на базі Qt)
- [ ] Інструменти профайлінгу
- [ ] Серіалізація сцен
- [ ] Система поведінки AI
- [ ] Мережа (підтримка мультиплеєра)

> **📌 Примітка:** Дати є орієнтовними і можуть змінюватись залежно від пріоритетів розробки.

---

## 🎓 Документація

Кожна основна підсистема має власний детальний README:

| Модуль | Документація | Швидкі посилання |
|--------|--------------|------------------|
| **Math** | [Engine/Math/README.md](Engine/Math/README.md) | [API довідник](#) • [Гід продуктивності](#) |
| **Core** | [Engine/Core/README.md](Engine/Core/README.md) | [Використання логера](#) • [Обробка помилок](#) |
| **Platform** | [Engine/Platform/README.md](Engine/Platform/README.md) | [Створення вікон](#) • [Система вводу](#) |
| **Graphics** | [Engine/Graphics/README.md](Engine/Graphics/README.md) | [Налаштування Vulkan](#) • [Гід шейдерів](#) |
| **ECS** | [Engine/ECS/README.md](Engine/ECS/README.md) | [Управління сутностями](#) • [Системи](#) |

### 📚 Додаткові ресурси

- **[Гід збірки](docs/Building.md)** — Детальні інструкції компіляції
- **[Огляд архітектури](docs/Architecture.md)** — Глибоке занурення в дизайн рушія
- **[Гід внесків](CONTRIBUTING.md)** — Як контрибутити код
- **[Гід стилю коду](docs/CodeStyle.md)** — Конвенції кодування
- **[Туторіали](docs/Tutorials/)** — Покрокові навчальні матеріали

---

## 🤝 Внесок у проєкт

Внески вітаються! Ось як ви можете допомогти:

### Способи внеску

- 🐛 **Повідомляйте про баги** через [GitHub Issues](https://github.com/vladhordiychuk/NuEngine/issues)
- 💡 **Пропонуйте функції** в [Discussions](https://github.com/vladhordiychuk/NuEngine/discussions)
- 📝 **Покращуйте документацію** надсилаючи PR
- 🔧 **Надсилайте код** слідуючи нашим [гайдлайнам контрибуції](CONTRIBUTING.md)
- ⭐ **Поставте зірку репозиторію** щоб показати підтримку!

### Робочий процес розробки

1. Зробіть форк репозиторію
2. Створіть гілку функції (`git checkout -b feature/amazing-feature`)
3. Закомітьте зміни (`git commit -m 'Add amazing feature'`)
4. Запуште гілку (`git push origin feature/amazing-feature`)
5. Відкрийте Pull Request

### Стандарти коду

- Дотримуйтесь найкращих практик C++20
- Використовуйте `clang-format` з наданим `.clang-format`
- Пишіть юніт-тести для нових функцій
- Оновлюйте документацію за потреби
- Робіть коміти атомарними та з хорошими описами

---

## ⚖️ Ліцензія

**NuEngine** наразі під **proprietary ліцензією** для освітніх цілей та портфоліо.

### Умови використання

- ✅ **Особисте навчання та експерименти** — дозволено
- ✅ **Освітнє використання в курсах/туторіалах** — дозволено з атрибуцією
- ❌ **Комерційне використання** — потрібен явний дозвіл
- ❌ **Розповсюдження** — не дозволено без дозволу
- ❌ **Використання в комерційних продуктах** — зв'яжіться для ліцензування

> 📧 Для запитів про комерційне ліцензування: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)

*Більш відкрита ліцензія може бути розглянута в майбутньому після досягнення зрілості проєкту.*

---

## ⚠️ Відомі проблеми та обмеження

| Проблема | Важливість | Статус | Обхідний шлях |
|----------|------------|--------|---------------|
| Неповна система вікон Linux | Середня | 🚧 В розробці | Використовуйте Windows збірку |
| Немає підтримки macOS | Низька | 📋 Заплановано | Використовуйте Windows/Linux |
| Відсутні інструменти профайлінгу пам'яті | Низька | 📋 Заплановано | Використовуйте зовнішні інструменти (Tracy) |
| SIMD вимагає процесор SSE2+ | Інфо | ✅ За дизайном | Відкат до скалярного коду |

**Повідомлення про проблеми:** Будь ласка, використовуйте наш [Issue Tracker](https://github.com/vladhordiychuk/NuEngine/issues) з детальними кроками відтворення.

---

## 🙏 Подяки

NuEngine не існував би без цих чудових ресурсів та джерел натхнення:

### Бібліотеки та інструменти

- **[spdlog](https://github.com/gabime/spdlog)** — Швидка C++ бібліотека логування
- **[Catch2](https://github.com/catchorg/Catch2)** — Сучасний C++ фреймворк тестування
- **[CMake](https://cmake.org/)** — Кросплатформенна система збірки

### Навчальні ресурси

- **[Game Engine Architecture](https://www.gameenginebook.com/)** від Jason Gregory
- **[Real-Time Rendering](https://www.realtimerendering.com/)** від Tomas Akenine-Möller та ін.
- **[Handmade Hero](https://handmadehero.org/)** від Casey Muratori
- **[Серія про ігрові рушії The Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)**

### Натхнення

- **[Unity Engine](https://unity.com/)** — Компонентний дизайн
- **[Unreal Engine](https://www.unrealengine.com/)** — Високопродуктивна архітектура
- **[Godot Engine](https://godotengine.org/)** — Філософія відкритого коду

---

## 👨‍💻 Автор

**Владислав Гордійчук**

*Розробник C++ ігрових рушіїв | Ентузіаст графічного програмування*

- 📧 Email: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)
- 💼 LinkedIn: [Vladyslav Hordiychuk](https://www.linkedin.com/in/%D0%B2%D0%BB%D0%B0%D0%B4%D0%B8%D1%81%D0%BB%D0%B0%D0%B2-%D0%B3%D0%BE%D1%80%D0%B4%D1%96%D0%B9%D1%87%D1%83%D0%BA-8a2704292/)
- 🐙 GitHub: [@vladhordiychuk](https://github.com/vladhordiychuk)
- 📍 Локація: Вінниця, Україна

> 💼 **Відкритий до можливостей!** Шукаю віддалені позиції C++/розробки ігрових рушіїв.

---

## 💬 Спільнота та підтримка

### Отримайте допомогу

- 📖 Спершу перевірте [Документацію](#-документація)
- 💬 Задавайте питання в [GitHub Discussions](https://github.com/vladhordiychuk/NuEngine/discussions)
- 🐛 Повідомляйте про баги через [GitHub Issues](https://github.com/vladhordiychuk/NuEngine/issues)
- 📧 Email: [gordijcukvlad64@gmail.com](mailto:gordijcukvlad64@gmail.com)

### Залишайтесь в курсі

- ⭐ **Поставте зірку** репозиторію щоб слідкувати за розробкою
- 👀 **Стежте** за новими релізами
- 🔔 Увімкніть сповіщення для важливих оновлень

---

## 📈 Статистика проєкту

![GitHub stars](https://img.shields.io/github/stars/vladhordiychuk/NuEngine?style=social)
![GitHub forks](https://img.shields.io/github/forks/vladhordiychuk/NuEngine?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/vladhordiychuk/NuEngine?style=social)

---

</details>

---

⬆️ [Повернутися на початок](#-nuengine)

</details>

---