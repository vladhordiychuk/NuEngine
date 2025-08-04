# NuEngine

## 🌐 Мови | Languages

- 🇺🇦 [Українська](#українська)
- 🇬🇧 [English](#english)

---

## Українська

**NuEngine** — це кастомний рушій для 3D-ігор, написаний на C++, з акцентом на продуктивність, модульність і майбутню підтримку складної симуляції світу.

> 🚧 В розробці: кастомний C++ рушій для 3D-ігор з підтримкою SIMD, власної математики та майбутнім AI-поведінкою у постапокаліптичному світі.

## 🧮 Реалізовано: SIMD-математика

На даному етапі реалізовано високопродуктивну математичну бібліотеку на основі **SSE**, аналогічну `DirectXMath`, з підтримкою:

- ✅ Вектори: `Vector2`, `Vector3`, `Vector4`
- ✅ Матриці: `Matrix4x4` з множенням, транспонуванням, оберненням, `Determinant`, `LookAt`, `Perspective`, `Decompose`
- ✅ Кватерніони: повна підтримка поворотів, `Slerp`, `Normalize`, `ToMatrix`
- ✅ Трансформації: `Transform`, `Scale`, `Rotate`, `Translate`
- ✅ SIMD-функції: додавання, віднімання, множення, dot/cross product, порівняння
- ✅ Строгі перевірки assert'ами

Бібліотека побудована навколо власного low-level SIMD-API `Simd::`, який абстрагує роботу з `__m128` для зручності та безпеки.

---

## 📂 Структура

*скоро буде*


## Для збірки проєкту необхідно:

```bash
git clone https://github.com/vladhordiychuk/NuEngine.git
```
```
cd NuEngine
```
```
mkdir build && cd build
```
```
cmake ..
```

## English

**NuEngine** is a custom 3D game engine written in C++, focused on performance, modularity, and future support for complex world simulation.

> 🚧 In development: a custom C++ game engine with SIMD-powered math and future AI behavior support for a post-apocalyptic world.

## 🧮 Implemented: SIMD Math Library

At the current stage, a high-performance SIMD math library has been implemented based on **SSE**, similar to `DirectXMath`, featuring:

- ✅ Vectors: `Vector2`, `Vector3`, `Vector4`
- ✅ Matrices: `Matrix4x4` with multiplication, transposition, inversion, `Determinant`, `LookAt`, `Perspective`, `Decompose`
- ✅ Quaternions: full support for rotations, `Slerp`, `Normalize`, `ToMatrix`
- ✅ Transformations: `Transform`, `Scale`, `Rotate`, `Translate`
- ✅ SIMD functions: addition, subtraction, multiplication, dot/cross product, comparisons
- ✅ Strict assert-based validations

The library is built around a custom low-level SIMD API `Simd::`, which abstracts `__m128` operations for ease of use and safety.

---

## 📂 Project Structure

*to be added*

---

## 🛠️ Build Instructions

```bash
git clone https://github.com/vladhordiychuk/NuEngine.git
```
```
cd NuEngine
```
```
mkdir build && cd build
```
```
cmake ..
```

## Author

Vladyslav Hordiychuk – C++ Game Engine Developer
[Send email](mailto:gordijcukvlad64@gmail.com) [LinkedIn](https://www.linkedin.com/in/%D0%B2%D0%BB%D0%B0%D0%B4%D0%B8%D1%81%D0%BB%D0%B0%D0%B2-%D0%B3%D0%BE%D1%80%D0%B4%D1%96%D0%B9%D1%87%D1%83%D0%BA-8a2704292/)
