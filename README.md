# NuEngine

## 🌐 Мови | Languages

- 🇺🇦 [Українська](#українська)
- 🇬🇧 [English](#english)

---

## Українська

**NuEngine** — це кастомний рушій для 3D-ігор, написаний на C++, з акцентом на продуктивність, модульність і майбутню підтримку складної симуляції світу.

> 🚧 В розробці: кастомний C++ рушій для 3D-ігор з підтримкою SIMD-оптимізованої математики, кросплатформенного створення вікон та майбутньої AI-поведінки у постапокаліптичному світі.

## ⚙️ Реалізовані підсистеми

На даному етапі NuEngine включає такі готові підсистеми:
 - SIMD-математика: високопродуктивна бібліотека на основі SSE, аналогічна DirectXMath, з підтримкою:
    - ✅ Вектори (Vector2, Vector3, Vector4)
    - ✅ Матриці (Matrix4x4: множення, транспонування, обернення, Determinant, LookAt, Perspective, Decompose)
    - ✅ Кватерніони (повороти, Slerp, Normalize, ToMatrix)
    - ✅ Трансформації (Transform, Scale, Rotate, Translate)
    - ✅ SIMD-операції (додавання, віднімання, множення, dot/cross product, порівняння)
 - Кросплатформенна віконна система: створення та керування вікнами (підтримка Windows, Linux у процесі).
 - Логер: гнучка система логування для дебагу та відстеження помилок.
 - Обробка помилок: надійна система винятків і перевірок.
 - Файлова система: кросплатформенний ввід/вивід файлів для роботи з ресурсами.
 - Тестування: юніт-тести та бенчмарки для перевірки коректності математики й ядра.
Математична бібліотека побудована навколо low-level API Simd::, який абстрагує роботу з __m128 для зручності та безпеки.

---

### 🛣️ Дорожня карта

 - [x] SIMD-математика
 - [x] Кросплатформенна віконна система (Windows)
 - [x] Логер
 - [x] Обробка помилок
 - [x] Файлова система
 - [ ] ECS (Entity Component System)
 - [ ] Графічний модуль (Vulkan/DirectX 12)
 - [ ] Інтеграція фізики (PhysX або власна)
 - [ ] Редактор рівнів (Qt)
 - [ ] AI-поведінка

---

## 📖 Приклад коду

Нижче наведено приклад створення вікна та запису повідомлення з використанням API NuEngine:

```cpp
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>

int main()
{
    NuEngine::Core::Logger::Init("logs/app.log");
    LOG_INFO("Starting NuEgine...");

    NuEngine::Core::Application app;
    auto result = app.Run();
    if (result.IsError()
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

## ⚙️ Implemented Subsystems

NuEngine currently features the following production-ready subsystems:

- **SIMD Math Library**: High-performance math based on **SSE**, similar to `DirectXMath`, with support for:
  - ✅ Vectors (`Vector2`, `Vector3`, `Vector4`)
  - ✅ Matrices (`Matrix4x4`: multiplication, transposition, inversion, `Determinant`, `LookAt`, `Perspective`, `Decompose`)
  - ✅ Quaternions (rotations, `Slerp`, `Normalize`, `ToMatrix`)
  - ✅ Transformations (`Transform`, `Scale`, `Rotate`, `Translate`)
  - ✅ SIMD operations (addition, subtraction, multiplication, dot/cross product, comparisons)
- **Cross-Platform Windowing**: Window creation and management (Windows supported, Linux in progress).
- **Logger**: Flexible logging system for debugging and error tracking.
- **Error Handling**: Robust error handling with custom exceptions and assertions.
- **File System**: Cross-platform file I/O for resource management.
- **Testing Framework**: Unit tests and benchmarks for validating math and core functionality.

The math library is built around a low-level `Simd::` API, abstracting `__m128` for safety and ease of use.

---

### 🛣️ Roadmap

- [x] SIMD Math Library
- [x] Cross-Platform Windowing (Windows)
- [x] Logger
- [x] Error Handling
- [x] File System
- [ ] Entity Component System (ECS)
- [ ] Graphics Module (Vulkan/DirectX 12)
- [ ] Physics Integration (PhysX or custom)
- [ ] Level Editor (Qt-based)
- [ ] AI Behavior System

---

## 📖 Code Sample

Below is an example of creating a window and logging a message using NuEngine's API:

```cpp
#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>

int main()
{
    NuEngine::Core::Logger::Init("logs/app.log");
    LOG_INFO("Starting NuEgine...");

    NuEngine::Core::Application app;
    auto result = app.Run();
    if (result.IsError()
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
