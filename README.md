# NuEngine

![C++](https://img.shields.io/badge/C++-17/20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Status](https://img.shields.io/badge/Status-In%20Development-orange.svg)
![License](https://img.shields.io/badge/License-Proprietary-red.svg)

## 🌐 Мови | Languages

- 🇺🇦 [Українська](#українська)
- 🇬🇧 [English](#english)

---

## Українська

**NuEngine** — це кастомний рушій для 3D-ігор, написаний на C++, з акцентом на продуктивність, модульність і майбутню підтримку складної симуляції світу.

> 🚧 В розробці: кастомний C++ рушій для 3D-ігор з підтримкою SIMD-оптимізованої математики, кросплатформенного створення вікон та майбутньої AI-поведінки у постапокаліптичному світі.

---

## ⚙️ Реалізовані підсистеми

NuEngine побудований на модульній архітектурі. Кожен ключовий компонент розробляється як незалежна підсистема з власним API та документацією.

| Модуль | Директорія | Опис | Документація |
|---|---|---|---|
| **Math** | `Engine/Math/` | Високопродуктивна SIMD (SSE) математична бібліотека. | [**README.md**](Engine/Math/README.md) |
| **Core** | `Engine/Core/` | Фундаментальні сервіси: логер, ФС, обробка помилок, Application. | [README.md](Engine/Core/README.md) |
| **Platform** | `Engine/Platform/` | Абстракція над ОС: створення вікон та обробка вводу (Win32, Linux). | [README.md](Engine/Platform/README.md) |
| **Graphics** | `Engine/Graphics/` | (В розробці) Абстракції рендерингу (Vulkan, DX12). | [README.md](Engine/Graphics/README.md) |
| **ECS** | `Engine/ECS/` | (В розробці) Система компонентів-сутностей. | [README.md](Engine/ECS/README.md) |
| **Tests** | `Tests/` | Unit-тести та бенчмарки для всіх підсистем. | [README.md](Tests/README.md) |

---

## 🛣️ Дорожня карта

| Модуль | Пріоритет | Очікуване завершення | Статус | Коментар |
|--------|-----------|-------------------|--------|----------|
| SIMD Math Library | Високий | Завершено | ✅ | Повна підтримка Vector, Matrix, Quaternion, Transform |
| Кросплатформене вікно | Високий | Завершено | ✅ | Windows готово, Linux в процесі |
| ECS (Entity Component System) | Середній | Q1 2026 | 🚧 | Основна структура для об’єктів і компонентів |
| Графічний модуль (Vulkan/DirectX 12) | Високий | Q2 2026 | ⬜ | Перші сцени з рендерингом куба та моделей |
| Фізика (PhysX або власна) | Середній | Q3 2026 | ⬜ | Колізії, рух об’єктів, інтеграція з ECS |
| Редактор рівнів (Qt) | Низький | Q4 2026 | ⬜ | Інструмент для швидкого створення сцен і рівнів |
| AI-поведінка | Середній | Q2 2027 | ⬜ | Патрулі, взаємодія NPC, реакція на події |

---

## 📖 Приклад коду

Нижче наведено приклад створення вікна та запису повідомлення з використанням API NuEngine:

```cpp
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Core/Errors/WindowError.hpp>
#include <Core/Errors/FileSystemError.hpp>

int main()
{
    auto initResult = NuEngine::Core::Logger::Init("logs/app.log");
    if (initResult.IsError())
    {
        std::cerr << "Logger init failed: " << NuEngine::Core::ToString(initResult.UnwrapError()) << std::endl;
        return -1;
    }
    LOG_INFO("Starting NuEngine...");

    NuEngine::Core::Application app;
    auto result = app.Run();
    if (result.IsError())
    {
        LOG_ERROR("Application run failed: {}", NuEngine::Core::ToString(result.UnwrapError()));
        NuEngine::Core::Logger::Shutdown();
        return -1;
    }

    NuEngine::Core::Logger::Shutdown();
    return 0;
}
```

---

## 📂 Структура проекту

<details>
  <summary>Показати структуру проекту</summary>
  
  ```plaintext
  NuEngine/
  ├── Game/
  ├── Editor/
  ├── Engine/
  │   ├── Math/
  │   ├── Core/
  │   ├── Platform/
  │   ├── ECS/
  │   ├── Graphics/
  │   └── CMakeLists.txt
  ├── Tests/
  │   ├── Unit/
  │   ├── Benchmark/
  │   ├── CMakeLists.txt
  │   └── testMain.cpp
  ├── Thirdparty/
  ├── CMakeLists.txt
  └── README.md
  ```
</details>

## 🛠️ Build Instructions

**Підтримувані платформи**: Windows (Linux в майбутньому)

**Залежності**: C++20, CMake, SSE

```bash
git clone https://github.com/vladhordiychuk/NuEngine.git
```
```bash
cd NuEngine
```
```bash
mkdir build && cd build
```
```bash
cmake ..
```

---

## English

**NuEngine** is a custom 3D game engine written in C++, focused on performance, modularity, and future support for complex world simulation.

> 🚧 In development: a custom C++ game engine with SIMD-powered math, cross-platform windowing, and future AI behavior support for a post-apocalyptic world.

---

## ⚙️ Implemented Subsystems

NuEngine is built on a modular architecture. Each key component is developed as an independent subsystem with its own API and documentation.

| Module | Directory | Description | Documentation |
|---|---|---|---|
| **Math** | `Engine/Math/` | High-performance SIMD (SSE) math library. | [**README.md**](Engine/Math/README.md) |
| **Core** | `Engine/Core/` | Fundamental services: Logger, FS, Error Handling, Application. | [README.md](Engine/Core/README.md) |
| **Platform** | `Engine/Platform/` | OS abstraction: Windowing and Input (Win32, Linux). | [README.md](Engine/Platform/README.md) |
| **Graphics** | `Engine/Graphics/` | (In development) Rendering abstractions (Vulkan, DX12). | [README.md](Engine/Graphics/README.md) |
| **ECS** | `Engine/ECS/` | (In development) Entity Component System. | [README.md](Engine/ECS/README.md) |
| **Tests** | `Tests/` | Unit tests and benchmarks for all subsystems. | [README.md](Tests/README.md) |

---

## 🛣️ Roadmap

| Module | Priority | Expected Completion | Status | Comments |
|--------|---------|-------------------|--------|----------|
| SIMD Math Library | High | Completed | ✅ | Full support for Vector, Matrix, Quaternion, Transform |
| Cross-Platform Window | High | Completed | ✅ | Windows ready, Linux in progress |
| ECS (Entity Component System) | Medium | Q1 2026 | 🚧 | Core structure for objects and components |
| Graphics Module (Vulkan/DirectX 12) | High | Q2 2026 | ⬜ | First scenes with cube and model rendering |
| Physics (PhysX or custom) | Medium | Q3 2026 | ⬜ | Collisions, object movement, integration with ECS |
| Level Editor (Qt) | Low | Q4 2026 | ⬜ | Tool for quickly creating scenes and levels |
| AI Behavior | Medium | Q2 2027 | ⬜ | Patrols, NPC interaction, reaction to events |

---

## 📖 Code Sample

Below is an example of creating a window and logging a message using NuEngine's API:

```cpp
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Core/Errors/WindowError.hpp>
#include <Core/Errors/FileSystemError.hpp>

int main()
{
    auto initResult = NuEngine::Core::Logger::Init("logs/app.log");
    if (initResult.IsError())
    {
        std::cerr << "Logger init failed: " << NuEngine::Core::ToString(initResult.UnwrapError()) << std::endl;
        return -1;
    }
    LOG_INFO("Starting NuEngine...");

    NuEngine::Core::Application app;
    auto result = app.Run();
    if (result.IsError())
    {
        LOG_ERROR("Application run failed: {}", NuEngine::Core::ToString(result.UnwrapError()));
        NuEngine::Core::Logger::Shutdown();
        return -1;
    }

    NuEngine::Core::Logger::Shutdown();
    return 0;
}
```

---

## 📂 Project Structure

<details>
  <summary>Show the project structure</summary>
  
  ```plaintext
  NuEngine/
  ├── Game/
  ├── Editor/
  ├── Engine/
  │   ├── Math/
  │   ├── Core/
  │   ├── Platform/
  │   ├── ECS/
  │   ├── Graphics/
  │   └── CMakeLists.txt
  ├── Tests/
  │   ├── Unit/
  │   ├── Benchmark/
  │   ├── CMakeLists.txt
  │   └── testMain.cpp
  ├── Thirdparty/
  ├── CMakeLists.txt
  └── README.md
  ```
</details>

---

## 🛠️ Build Instructions

**Supported platforms**: Windows (Linux support in progress)

**Dependencies**: C++20, CMake, SSE

```bash
git clone https://github.com/vladhordiychuk/NuEngine.git
```
```bash
cd NuEngine
```
```bash
mkdir build && cd build
```
```bash
cmake ..
```

---

## 👨‍💻 Author

Vladyslav Hordiychuk – C++ Game Engine Developer  
 📧 [Send email](mailto:gordijcukvlad64@gmail.com)  
 🔗 [LinkedIn](https://www.linkedin.com/in/%D0%B2%D0%BB%D0%B0%D0%B4%D0%B8%D1%81%D0%BB%D0%B0%D0%B2-%D0%B3%D0%BE%D1%80%D0%B4%D1%96%D0%B9%D1%87%D1%83%D0%BA-8a2704292/)  
 💼 Open to work and collaborations!
