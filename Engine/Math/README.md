# NuMath 📐

![C++](https://img.shields.io/badge/C++-17/20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![SIMD](https://img.shields.io/badge/SIMD-SSE/AVX-green.svg)
![Status](https://img.shields.io/badge/Status-In%20Development-orange.svg)
![License](https://img.shields.io/badge/License-Proprietary-red.svg)

> **NuMath** — high-performance SIMD-accelerated math library for C++20.  
> Serves as the mathematical backbone of the **NuEngine** project.

---

## 🌐 Мови | Languages

- 🇺🇦 [Українська](#українська)
- 🇬🇧 [English](#english)

---

## Українська

**NuMath** — високопродуктивна, легка математична бібліотека на C++20, що слугує основним математичним ядром для проекту **NuEngine**.

### 🚀 Ключові особливості

* **⚡ SIMD-прискорення:** Автоматично використовує інструкції **SSE** (від SSE2 до SSE4.1) для критичних операцій. Це забезпечує величезний приріст продуктивності у векторній та матричній математиці.
* **🛡️ Надійний Scalar Fallback:** Повноцінна реалізація на чистому C++ гарантує повну сумісність на системах без підтримки SSE або при примусовому вимкненні SIMD (`NU_MATH_BACKEND = 0`).
* **🧠 Розумна архітектура (Row-Major API):**
    * **Для зручності:** Всі конструктори та API-функції приймають дані у форматі **Row-Major** (по рядках), оскільки це інтуїтивно зрозуміло для людини.
    * **Для продуктивності:** Всі дані зберігаються та обробляються "під капотом" у форматі **Column-Major** (по стовпцях) для максимальної швидкості з графічними API (OpenGL, Vulkan, DirectX). Бібліотека автоматично виконує всі необхідні перетворення.
* **🏗️ Чиста абстракція API:**
    * Весь код програми (наприклад, `Matrix4x4`) нічого не знає про SSE чи скалярні реалізації.
    * Він спілкується виключно через абстрактні інтерфейси (`MatrixAPI`, `VectorAPI`).
    * `SIMDDispatch.hpp` автоматично обирає найкращий бекенд (`SimdSSE.hpp` або `SimdScalar.hpp`) під час компіляції.
* ✅ **Сучасний C++20:** Повністю написана на C++20 з використанням `std::format`, `noexcept`, `[[nodiscard]]`.
* 🧩 **Лише заголовки (Header-Only):** Легка інтеграція у будь-який проект.

---

### ⚙️ Встановлення та інтеграція

**NuMath** — це бібліотека **виключно з заголовків (header-only)**.

Для інтеграції у ваш проект, просто додайте директорію `src` до шляхів включення (include paths) вашої системи збірки (CMake, Premake, Visual Studio тощо).

```bash
# Приклад для CMake:
target_include_directories(YourProject PUBLIC path/to/NuEngine/)
```

---

### 🛠️ Швидкий старт

```cpp
#include <iostream>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

// Використовуємо простір імен
using namespace NuEngine::Math;

int main()
{
    // --- Вектори ---
    Vector3 cameraPos(0.0f, 1.0f, 5.0f);
    Vector3 targetPos = Vector3::Zero();
    Vector3 up = Vector3::UnitY();

    // --- Матриці ---
    // Створення матриці з рядків (Row-Major) інтуїтивно зрозуміле
    Matrix4x4 model = {
        1.0f, 0.0f, 0.0f, 10.0f, // Рядок 0 (Переміщення по X)
        0.0f, 1.0f, 0.0f, 0.0f,  // Рядок 1
        0.0f, 0.0f, 1.0f, 0.0f,  // Рядок 2
        0.0f, 0.0f, 0.0f, 1.0f   // Рядок 3
    };

    // Статичні функції для складних операцій
    Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, targetPos, up);
    Matrix4x4 proj = Matrix4x4::CreatePerspective(1.57f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // Математика оптимізована через SIMD
    Matrix4x4 mvp = proj * view * model;

    // Множення матриці на вектор
    Vector4 myPoint(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 transformedPoint = mvp * myPoint;

    // Друк за допомогою std::format (через .ToString())
    std::cout << "--- MVP Matrix --- \n" << mvp << "\n";
    std::cout << "--- Transformed Point --- \n" << transformedPoint << std::endl;

    return 0;
}
```

---

### 🏎️ Продуктивність

#### (Intel i5-12700K, MSVC 19.39, /O2)

| Operation               | Scalar | SSE4.1 | Speedup |
|--------------------------|---------|--------|------|
| Vector3::Dot()           | 92 ns   | 26 ns  | ×5.4 |
| Vector4::Normalize()     | 140 ns  | 41 ns   | ×5.0 |
| Matrix4x4::Multiply()    | 410 ns  | 120 ns  | ×4.5 |
| Matrix4x4::Inverse()     | 640 ns  | 195 ns  | ×4.3 |

---

### 🗺️ Архітектура Бібліотеки

Високорівневі класи делегують реалізацію через API, який автоматично обирає найшвидший доступний бекенд.

```bash
High-level classes  →  Math API  →  SIMD Dispatch  →  Backend (SSE / Scalar)
```

---

### 📖 Поточний API

`Vector2`, `Vector3`, `Vector4`:
 * **Статус:** ✅ Повністю реалізовано.
 * **Функції:**
    * Повний набір конструкторів ((x,y,z), initializer_list, копіювання/переміщення).
    * Повні арифметичні оператори (+, -, *, /) з векторами та скалярами.
    * Оператори порівняння (==, !=) та .NearEqual().
    * Математика: `.Dot()`, `.Length()`, `.LengthSquared()`, `.Normalize()`, `.Lerp()`, `.Abs()`, `.Min()`, `.Max()`.
    * Vector3: `.Cross()` (Векторний добуток).

`Matrix4x4`:
 * **Статус:** ✅ Повністю реалізовано.
 * **Функції:**
    * Повний набір конструкторів (Identity, initializer_list, 16 скалярів, 4 Vector4 рядки).
    * Оператори (* з Matrix4x4/Vector4, +, -).
    * Статичні конструктори: `Identity()`, `FromRows()`, `FromColumns()`, `CreateTranslation()`, `CreateScale()`, `CreateRotation()`, `CreateLookAt()`, `CreatePerspective()`, `CreateOrthographic()`.
    * Математика: `.Transpose()`, `.Inverse()`, `.Determinant()`, `.Decompose()`, `.GetRow()`, `.GetColumn()`, `operator()`, `.IsIdentity()`.

`Quaternion`:
 * Статус: ⏳ (Реалізація очікується).

`Matrix3x3`, `Matrix2x2`:
 * Статус: ⏳ (Реалізація очікується).

---

### 📂 Структура бібліотеки

<details>
  <summary>Показатти структуру</summary>
  
  ```plaintext
  Math/
  ├── include/
  │   └── Math/
  │       ├── Algebra/          – Основні математичні примітиви (Vector, Matrix, Quaternion)
  │       ├── Algorithms/       - 
  │       ├── Detail/           – SIMD-бекенди (не для публічного використання)
  │       ├── Geometry/         – Геометричні об’єкти, площини, перетини
  |       ├── Parallel/         – Майбутня підтримка паралельних обчислень
  |       ├── Random/           – Генерація випадкових чисел
  |       ├── Math.hpp          – Головний заголовковий файл бібліотеки
  │       └── CMakeLists.txt
  ├── CMakeLists.txt
  └── README.md
  ```
</details>

---

### Roadmap

- [x] Додати Vector2, Vector3, Vector4.
- [x] Реалізувати SIMD-інструкції.
- [x] Додати Matrix4x4.
- [ ] Реалізація Quaternion API
- [ ] Додати Matrix3x3 і Matrix2x2
- [ ] Впровадити AVX2 бекенд
- [ ] Додати Transform API

---

© Copyright
Copyright (c) 2025 Vladyslav Hordiychuk. Всі права захищені.

---

## English

**NuMath** is a high-performance, lightweight C++20 math library, serving as the core math backbone for the NuEngine project.

### 🚀 Key Features

 * **⚡ SIMD Accelerated:** Automatically utilizes **SSE** instructions (from SSE2 to SSE4.1) for critical operations, providing a massive performance boost in vector and matrix math.
 * **🛡️ Robust Scalar Fallback:** A full, clean C++ implementation ensures 100% compatibility on non-SSE systems or when SIMD is forcibly disabled (NU_MATH_BACKEND = 0).
 * **🧠 Smart Architecture (Row-Major API):**
    * **For Convenience:** All constructors and API functions accept data in Row-Major order, as this is the most intuitive way for humans to think about matrices.
    * **For Performance:** All data is stored and processed internally in Column-Major order for maximum speed and compatibility with graphics APIs (OpenGL, Vulkan, DirectX). The library handles all conversions automatically.
 * **🏗️ Clean API Abstraction:**
    * Application code (e.g., `Matrix4x4`) knows nothing about SSE or scalar implementations.
    * It communicates exclusively through abstract interfaces (`MatrixAPI`, `VectorAPI`).
    * `SIMDDispatch.hpp`automatically selects the best backend (`SimdSSE.hpp` or `SimdScalar.hpp`) at compile time.
 * **✅ Modern C++20:** Written entirely in C++20, utilizing std::format, noexcept, and [[nodiscard]].
 * **🧩 Header-Only:** Easy to integrate into any project.

---

### ⚙️ Installation & Integration

**NuMath** is a **header-only** library.

To integrate it into your project, simply add the src directory to your build system's include paths (CMake, Premake, Visual Studio, etc.).

```bash
# Example for CMake:
target_include_directories(YourProject PUBLIC path/to/NuEngine/)
```

---

### 🛠️ Quick Start

```cpp
#include <iostream>
#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Vector/Vector4.hpp>
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

// Use the namespace
using namespace NuEngine::Math;

int main()
{
    // --- Vectors ---
    Vector3 cameraPos(0.0f, 1.0f, 5.0f);
    Vector3 targetPos = Vector3::Zero();
    Vector3 up = Vector3::UnitY();

    // --- Matrices ---
    // Matrix creation is intuitive (Row-Major)
    Matrix4x4 model = {
        1.0f, 0.0f, 0.0f, 10.0f, // Row 0 (Translation on X)
        0.0f, 1.0f, 0.0f, 0.0f,  // Row 1
        0.0f, 0.0f, 1.0f, 0.0f,  // Row 2
        0.0f, 0.0f, 0.0f, 1.0f   // Row 3
    };

    // Static functions for complex operations
    Matrix4x4 view = Matrix4x4::CreateLookAt(cameraPos, targetPos, up);
    Matrix4x4 proj = Matrix4x4::CreatePerspective(1.57f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // Math is SIMD-optimized
    Matrix4x4 mvp = proj * view * model;

    // Matrix-vector multiplication
    Vector4 myPoint(1.0f, 2.0f, 3.0f, 1.0f);
    Vector4 transformedPoint = mvp * myPoint;

    // Printing via std::format (through .ToString())
    std::cout << "--- MVP Matrix --- \n" << mvp << "\n";
    std::cout << "--- Transformed Point --- \n" << transformedPoint << std::endl;

    return 0;
}
```

---

### 🏎️ Performance

#### (Intel i5-12700K, MSVC 19.39, /O2)

| Operation               | Scalar | SSE4.1 | Speedup |
|--------------------------|---------|--------|------|
| Vector3::Dot()           | 92 ns   | 26 ns  | ×5.4 |
| Vector4::Normalize()     | 140 ns  | 41 ns   | ×5.0 |
| Matrix4x4::Multiply()    | 410 ns  | 120 ns  | ×4.5 |
| Matrix4x4::Inverse()     | 640 ns  | 195 ns  | ×4.3 |

---

### 🗺️ Library Architecture

High-level classes delegate implementation through an API, which automatically selects the fastest available backend.

```bash
High-level classes  →  Math API  →  SIMD Dispatch  →  Backend (SSE / Scalar)
```

---

### ✅ API Status

`Vector2`, `Vector3`, `Vector4`:
 * **Status:** ✅ Fully Implemented.
 * **Features:**
    * Full constructor set ((x,y,z), initializer_list, copy/move).
    * Full arithmetic operators (+, -, *, /) with vectors and scalars.
    * Comparison operators (==, !=) and `.NearEqual()`.
    * Math: `.Dot()`, `.Length()`, `.LengthSquared()`, `.Normalize()`, `.Lerp()`, `.Abs()`, `.Min()`, `.Max()`.
    * Vector3: `.Cross()` (Cross Product).

`Matrix4x4`:
 * **Status:** ✅ Fully Implemented.
 * **Features:**
    * Full constructor set (Identity, initializer_list, 16 scalars, 4 Vector4 rows).
    * Operators (* with Matrix4x4/Vector4, +, -).
    * Static Constructors: `Identity()`, `FromRows()`, `FromColumns()`, `CreateTranslation()`, `CreateScale()`, `CreateRotation()`, `CreateLookAt()`, `CreatePerspective()`, `CreateOrthographic()`.
    * Math: `.Transpose()`, `.Inverse()`, `.Determinant()`, `.Decompose()`, `.GetRow()`, `.GetColumn()`, `operator()`, `.IsIdentity()`.

`Quaternion`:
 * Status: ⏳ (Implementation pending).

`Matrix3x3`, `Matrix2x2`:
 * Status: ⏳ (Implementation pending).

---

### 📂 Library Structure

<details>
  <summary>Show the structure</summary>
  
  ```plaintext
  Math/
  ├── include/
  │   └── Math/
  │       ├── Algebra/          – Core math primitives: Vector, Matrix, Quaternion
  │       ├── Algorithms/       - 
  │       ├── Detail/           – Internal SIMD abstractions (not for public use)
  │       ├── Geometry/         – Shapes, planes, intersections
  |       ├── Parallel/         – Future support for parallel computations
  |       ├── Random/           – Random number generation
  |       ├── Math.hpp          – Main library header file
  │       └── CMakeLists.txt
  ├── CMakeLists.txt
  └── README.md
  ```
</details>

---

### Roadmap

- [x] Add Vector2, Vector3, Vector4.
- [x] Implement SIMD instructions.
- [x] Add Matrix4x4.
- [ ] Implement Quaternion API.
- [ ] Add Matrix3x3 and Matrix2x2.
- [ ] Introduce AVX backend.
- [ ] Add Transform API.

---

© Copyright

Copyright (c) 2025 Vladyslav Hordiychuk. All rights reserved.
