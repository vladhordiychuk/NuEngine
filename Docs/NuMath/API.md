<div align="center">

# 📖 NuMath API Reference

**[🏠 Home](../../NuMath/README.md)** • **[⚙️ Installation](INSTALL.md)** • **[🎯 Examples](EXAMPLES.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>

---

## 📑 Table of Contents

1. [Vector Types](#1-vector-types)
   - [Vector2](#vector2)
   - [Vector3](#vector3)
   - [Vector4](#vector4)
2. [Matrix Types](#2-matrix-types)
   - [Matrix4x4](#matrix4x4)
3. [Color Management](#3-color-management)
   - [Color API](#color-api)
   - [Linear Workflow](#linear-workflow)
4. [Batch Processing](#4-batch-processing)
   - [SoA (Structure of Arrays)](#soa-structure-of-arrays)
5. [Architecture](#5-architecture)
   - [SIMD Backend](#simd-backend)
   - [Memory Layout](#memory-layout)

---

## 1. Vector Types

### Vector2

**2D vector with full SIMD utilization (128-bit register)**

#### Construction

```cpp
#include <Math/Algebra/Vector/Vector2.hpp>

// Default constructor (zero vector)
Vector2 v1;  // (0, 0)

// Component constructor
Vector2 v2(1.5f, 2.5f);

// Uniform constructor
Vector2 v3(5.0f);  // (5, 5)

// Copy/Move
Vector2 v4 = v2;
Vector2 v5 = std::move(v2);

// From initializer list
Vector2 v6 = {3.0f, 4.0f};
```

#### Static Constructors

```cpp
Vector2::Zero();      // (0, 0)
Vector2::One();       // (1, 1)
Vector2::UnitX();     // (1, 0)
Vector2::UnitY();     // (0, 1)
```

#### Arithmetic Operations

```cpp
Vector2 a(1, 2);
Vector2 b(3, 4);

// Addition
Vector2 sum = a + b;          // (4, 6)
a += b;                       // a = (4, 6)

// Subtraction
Vector2 diff = a - b;         // (-2, -2)
a -= b;

// Multiplication (component-wise)
Vector2 prod = a * b;         // (3, 8)
a *= b;

// Division (component-wise)
Vector2 quot = a / b;         // (0.333, 0.5)
a /= b;

// Scalar operations
Vector2 scaled = a * 2.0f;    // (2, 4)
Vector2 divided = a / 2.0f;   // (0.5, 1)
a *= 0.5f;
a /= 2.0f;

// Negation
Vector2 neg = -a;             // (-1, -2)
```

#### Mathematical Operations

```cpp
Vector2 a(3, 4);
Vector2 b(1, 0);

// Dot product
float dot = a.Dot(b);         // 3.0

// Length operations
float len = a.Length();       // 5.0
float lenSq = a.LengthSq();   // 25.0

// Normalization
Vector2 norm = a.Normalize(); // (0.6, 0.8)
a.NormalizeInPlace();         // Modifies a

// Distance
float dist = a.Distance(b);   // Distance between vectors
float distSq = a.DistanceSq(b);

// Absolute value
Vector2 abs = a.Abs();        // |x|, |y|

// Min/Max
Vector2 min = Vector2::Min(a, b);  // Component-wise min
Vector2 max = Vector2::Max(a, b);  // Component-wise max

// Linear interpolation
Vector2 lerp = Vector2::Lerp(a, b, 0.5f);  // Midpoint
```

#### Comparison

```cpp
Vector2 a(1, 2);
Vector2 b(1, 2);
Vector2 c(1.00001f, 2.00001f);

// Exact equality
bool exact = (a == b);        // true
bool notEqual = (a != c);     // true

// Near equality (epsilon tolerance)
bool near = a.NearEqual(c, 0.001f);  // true
```

#### Access

```cpp
Vector2 v(1.5f, 2.5f);

// Component access
float x = v.X();              // 1.5
float y = v.Y();              // 2.5

// Raw data pointer (for GPU upload)
const float* data = v.Data(); // [1.5, 2.5, 0, 0] (SIMD padding)
```

#### Swizzling

```cpp
Vector2 v(1, 2);

Vector2 xx = v.XX();          // (1, 1)
Vector2 yy = v.YY();          // (2, 2)
Vector2 yx = v.YX();          // (2, 1) - Swapped
```

---

### Vector3

**3D vector with SIMD acceleration**

#### Additional Operations (vs Vector2)

```cpp
Vector3 a(1, 2, 3);
Vector3 b(4, 5, 6);

// Cross product
Vector3 cross = a.Cross(b);   // (-3, 6, -3)

// Component access
float z = a.Z();              // 3.0

// Static constructors
Vector3::UnitZ();             // (0, 0, 1)
Vector3::Up();                // (0, 1, 0)
Vector3::Down();              // (0, -1, 0)
Vector3::Left();              // (-1, 0, 0)
Vector3::Right();             // (1, 0, 0)
Vector3::Forward();           // (0, 0, 1)
Vector3::Backward();          // (0, 0, -1)
```

#### Swizzling

```cpp
Vector3 v(1, 2, 3);

Vector2 xy = v.XY();          // (1, 2)
Vector2 xz = v.XZ();          // (1, 3)
Vector3 zyx = v.ZYX();        // (3, 2, 1)
```

**All Vector2 operations apply to Vector3.**

---

### Vector4

**4D vector (homogeneous coordinates for transformations)**

#### Additional Operations

```cpp
Vector4 v(1, 2, 3, 1);

// Component access
float w = v.W();              // 1.0

// Conversion
Vector3 v3 = v.XYZ();         // Drop W component

// Static constructor
Vector4::UnitW();             // (0, 0, 0, 1)
```

**All Vector2/Vector3 operations apply to Vector4.**

---

## 2. Matrix Types

### Matrix4x4

**4x4 transformation matrix (Column-Major storage, Row-Major API)**

#### Construction

```cpp
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

// Identity matrix
Matrix4x4 identity = Matrix4x4::Identity();

// From 16 floats (Row-Major input!)
Matrix4x4 m1(
    1, 0, 0, 10,  // Row 0: Translation X = 10
    0, 1, 0, 0,   // Row 1
    0, 0, 1, 0,   // Row 2
    0, 0, 0, 1    // Row 3
);

// From 4 row vectors
Vector4 row0(1, 0, 0, 10);
Vector4 row1(0, 1, 0, 0);
Vector4 row2(0, 0, 1, 0);
Vector4 row3(0, 0, 0, 1);
Matrix4x4 m2(row0, row1, row2, row3);

// From initializer list (Row-Major)
Matrix4x4 m3 = {
    1, 0, 0, 10,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};
```

#### Transformation Matrices

```cpp
// Translation
Matrix4x4 T = Matrix4x4::CreateTranslation({10, 5, 0});
Matrix4x4 T2 = Matrix4x4::CreateTranslation(Vector3(10, 5, 0));

// Rotation (axis-angle)
Matrix4x4 Rx = Matrix4x4::CreateRotationX(1.57f);  // 90° around X
Matrix4x4 Ry = Matrix4x4::CreateRotationY(1.57f);
Matrix4x4 Rz = Matrix4x4::CreateRotationZ(1.57f);
Matrix4x4 R = Matrix4x4::CreateRotation(Vector3::UnitY(), 1.57f);

// Scale
Matrix4x4 S = Matrix4x4::CreateScale({2, 2, 2});
Matrix4x4 S2 = Matrix4x4::CreateScale(2.0f);  // Uniform scale

// Combined TRS (in correct order)
Matrix4x4 transform = T * R * S;
```

#### View & Projection Matrices

```cpp
// Look-At View Matrix
Vector3 eye(0, 5, 10);       // Camera position
Vector3 target(0, 0, 0);     // Look at origin
Vector3 up = Vector3::UnitY();
Matrix4x4 view = Matrix4x4::CreateLookAt(eye, target, up);

// Perspective Projection
float fov = 1.57f;           // 90° in radians
float aspect = 16.0f / 9.0f;
float nearPlane = 0.1f;
float farPlane = 1000.0f;
Matrix4x4 proj = Matrix4x4::CreatePerspective(fov, aspect, nearPlane, farPlane);

// Orthographic Projection
float left = -10, right = 10;
float bottom = -10, top = 10;
float near = 0.1f, far = 100.0f;
Matrix4x4 ortho = Matrix4x4::CreateOrthographic(left, right, bottom, top, near, far);
```

#### Matrix Operations

```cpp
Matrix4x4 A, B;

// Multiplication
Matrix4x4 C = A * B;          // Matrix × Matrix
A *= B;

Vector4 v(1, 2, 3, 1);
Vector4 result = A * v;       // Matrix × Vector

// Addition/Subtraction
Matrix4x4 sum = A + B;
Matrix4x4 diff = A - B;
A += B;
A -= B;

// Transpose
Matrix4x4 AT = A.Transpose();
A.TransposeInPlace();

// Inverse
Matrix4x4 invA = A.Inverse();
A.InvertInPlace();

// Determinant
float det = A.Determinant();
```

#### Access

```cpp
Matrix4x4 m;

// Element access (Row, Column)
float value = m(0, 3);        // Row 0, Column 3 (Translation X)
m(1, 3) = 5.0f;               // Set Translation Y

// Row/Column extraction
Vector4 row0 = m.GetRow(0);
Vector4 col0 = m.GetColumn(0);

// Raw data (Column-Major, GPU-ready)
const float* data = m.Data();
glUniformMatrix4fv(location, 1, GL_FALSE, data);  // Direct upload!
```

#### Decomposition

```cpp
Matrix4x4 transform = ...;

Vector3 translation;
Vector3 scale;
// Quaternion rotation;  // TODO: Quaternion not yet implemented

bool success = transform.Decompose(translation, scale);
// Note: Full TRS decomposition requires Quaternion (v0.2)
```

---

## 3. Color Management

### Color API

**Enforced Linear RGB workflow for physically correct rendering**

#### Philosophy

NuMath enforces a strict workflow:
1. **Input:** Convert sRGB (UI/textures) → Linear RGB
2. **Math:** All operations in linear space
3. **Output:** Convert Linear RGB → sRGB (for display)

```cpp
#include <Math/Algebra/Color/Color.hpp>
```

#### Construction

```cpp
// Linear RGB (direct construction)
Color linear(0.8f, 0.6f, 0.4f, 1.0f);  // RGBA in [0, 1]
Color linear2(0.5f);                    // Gray (0.5, 0.5, 0.5, 1)

// From sRGB (auto-converts to linear)
Color fromSRGB = Color::FromSRGB(1.0f, 0.5f, 0.25f);  // [0, 1] range
Color fromBytes = Color::FromBytes(255, 128, 64, 255); // [0, 255] range (uses LUT)

// From packed integer
uint32_t rgba = 0xFF4080FF;
Color fromPacked = Color::FromRGBA32(rgba);

// Predefined colors (in Linear space)
Color red = Colors::Linear::Red;
Color blue = Colors::Linear::Blue;

// Predefined colors (in sRGB space)
Color uiRed = Colors::SRGB::Red;  // Auto-converts to linear
```

#### Color Operations

```cpp
Color a = Color::FromBytes(255, 100, 50);
Color b = Color::FromBytes(100, 200, 150);

// Arithmetic (in linear space)
Color sum = a + b;
Color diff = a - b;
Color scaled = a * 0.5f;            // Darken
Color tinted = a * Color(0.8f, 0.6f, 0.4f);  // Color multiplication

// Lerp (linear interpolation)
Color mid = Color::Lerp(a, b, 0.5f);

// Alpha blending (premultiplied)
Color premul = a.PremultiplyAlpha();
Color blended = a.BlendOver(b);     // Composites a over b

// Saturation (clamp to [0, 1])
Color clamped = a.Saturate();

// Grayscale (Rec. 709 weights)
Color gray = a.ToGrayscale();       // 0.2126*R + 0.7152*G + 0.0722*B

// Inversion
Color inverted = a.Invert();        // (1-R, 1-G, 1-B, A)
```

#### Conversion & Output

```cpp
Color color = Color::FromBytes(255, 128, 64);

// To sRGB (for rendering)
uint32_t rgba = color.ToRGBA32();   // 0xAABBGGRR (packed RGBA)

// To Vector4 (for shaders)
Vector4 v = color.ToVector4();      // (R, G, B, A) in linear space

// Component access
float r = color.R();
float g = color.G();
float b = color.B();
float a = color.A();

// Raw data (for GPU upload)
const float* data = color.Data();
glUniform4fv(location, 1, data);
```

#### Example: UI Theming

```cpp
// Load theme colors from config (sRGB)
Color primary = Color::FromBytes(66, 126, 234);    // #427EEA
Color secondary = Color::FromBytes(118, 75, 162);  // #764BA2

// Compute derived colors (in linear space)
Color primaryDark = primary * 0.7f;
Color primaryLight = Color::Lerp(primary, Colors::Linear::White, 0.3f);

// Apply tint to textures
Color buttonTex = Color::FromBytes(255, 255, 255);
Color tinted = buttonTex * primary;  // Tints white texture to primary color

// Output to GPU
uint32_t rgba = tinted.ToRGBA32();
```

---

### Linear Workflow

#### Why Linear Space Matters

**Problem:** sRGB is non-linear (gamma ~2.2). Math in sRGB space is physically incorrect.

```cpp
// ❌ WRONG: Blend in sRGB space
uint8_t r1 = 255, r2 = 0;
uint8_t midR = (r1 + r2) / 2;  // 127
// Visual result: Too dark! Should be ~186 in sRGB.

// ✅ RIGHT: NuMath auto-handles this
Color c1 = Color::FromBytes(255, 0, 0);  // Red (sRGB → Linear)
Color c2 = Color::FromBytes(0, 0, 0);    // Black
Color mid = Color::Lerp(c1, c2, 0.5f);   // Blend in linear space
uint32_t result = mid.ToRGBA32();        // Converts back to sRGB
// result: 0xFF0000BC (~188) - Correct!
```

#### Workflow Example: HDR Rendering

```cpp
// 1. Load textures (sRGB)
Color albedo = Color::FromBytes(200, 150, 100);

// 2. Lighting calculations (linear space)
Color light = Color(1.5f, 1.5f, 1.5f);  // HDR light (>1.0)
Color lit = albedo * light;             // Can exceed [0, 1]

// 3. Tone mapping (bring back to [0, 1])
Color toneMapped = lit / (lit + Color(1.0f));  // Reinhard

// 4. Output (linear → sRGB)
uint32_t finalColor = toneMapped.ToRGBA32();
```

---

## 4. Batch Processing

### SoA (Structure of Arrays)

**SIMD-optimized batch operations for large datasets**

#### Concept

Instead of:
```cpp
// AoS (Array of Structures) - cache-inefficient
struct Particle { float x, y, z; };
Particle particles[10000];
```

Use:
```cpp
// SoA (Structure of Arrays) - SIMD-friendly
float x[10000], y[10000], z[10000];
```

**Benefit:** Process 4-8 particles per CPU cycle with SSE/AVX!

#### API

```cpp
#include <NuMath/Batch/BatchVectorSoA.hpp>

// Allocate aligned arrays (required for AVX2)
alignas(32) float pos_x[10000], pos_y[10000], pos_z[10000];
alignas(32) float vel_x[10000], vel_y[10000], vel_z[10000];

// Create views (zero-cost)
auto positions = NuMath::Batch::SoA::MakeView(pos_x, pos_y, pos_z);
auto velocities = NuMath::Batch::SoA::MakeView(vel_x, vel_y, vel_z);

// Batch operations (SIMD-accelerated)
size_t count = 10000;

// Add: positions += velocities
NuMath::Batch::SoA::Add(positions, positions, velocities, count);

// Mul: positions *= 0.99 (damping)
NuMath::Batch::SoA::Mul(positions, positions, 0.99f, count);

// Subtract
NuMath::Batch::SoA::Sub(positions, positions, velocities, count);

// Divide
NuMath::Batch::SoA::Div(positions, positions, 2.0f, count);

// Dot product (returns array of results)
std::vector<float> dotProducts(count);
NuMath::Batch::SoA::Dot(dotProducts.data(), positions, velocities, count);
```

#### Example: Particle System

```cpp
struct ParticleSystem {
    static constexpr size_t MAX_PARTICLES = 10000;
    
    alignas(32) float pos_x[MAX_PARTICLES];
    alignas(32) float pos_y[MAX_PARTICLES];
    alignas(32) float pos_z[MAX_PARTICLES];
    
    alignas(32) float vel_x[MAX_PARTICLES];
    alignas(32) float vel_y[MAX_PARTICLES];
    alignas(32) float vel_z[MAX_PARTICLES];
    
    size_t activeCount = 0;
    
    void Update(float dt) {
        auto pos = Batch::SoA::MakeView(pos_x, pos_y, pos_z);
        auto vel = Batch::SoA::MakeView(vel_x, vel_y, vel_z);
        
        // Apply gravity
        for (size_t i = 0; i < activeCount; ++i) {
            vel_y[i] -= 9.8f * dt;
        }
        
        // Update positions (SIMD: 4-8 particles per cycle!)
        Batch::SoA::Add(pos, pos, vel, activeCount);
        
        // Apply damping
        Batch::SoA::Mul(vel, vel, 0.99f, activeCount);
    }
};
```

---

## 5. Architecture

### SIMD Backend

NuMath auto-detects CPU features and selects the best backend:

```cpp
#include <Math/Core/SimdDetect.hpp>

// Runtime detection
if (NuMath::SIMD::HasAVX2()) {
    std::cout << "Using AVX2 (256-bit, 8 floats/cycle)\n";
} else if (NuMath::SIMD::HasSSE41()) {
    std::cout << "Using SSE4.1 (128-bit, 4 floats/cycle)\n";
} else {
    std::cout << "Using Scalar fallback\n";
}
```

**Backend Selection:**
- **AVX2:** Intel Haswell (2013+), AMD Excavator (2015+)
- **SSE4.1:** Intel Penryn (2008+), AMD Bulldozer (2011+)
- **Scalar:** Fallback for older CPUs

---

### Memory Layout

#### Vectors: Always 128-bit (even Vector2!)

```cpp
// Memory layout (all types)
class Vector2 { __m128 m_data; };  // [X, Y, 0, 0]
class Vector3 { __m128 m_data; };  // [X, Y, Z, 0]
class Vector4 { __m128 m_data; };  // [X, Y, Z, W]
```

**Why this matters:**
- ✅ Consistent SIMD usage across all types
- ✅ No alignment issues
- ✅ Fast swizzling/conversion

#### Matrix4x4: Column-Major (GPU native)

```cpp
// Row-Major API (user input)
Matrix4x4 m = {
    1, 0, 0, 10,  // Row 0
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

// Column-Major Storage (internal)
// m.Data() = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  10,0,0,1 }
//             └─Col0─┘  └─Col1─┘  └─Col2─┘  └─Col3───┘

// Direct GPU upload (no conversion!)
glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data());
```

---

## 📚 Next Steps

- **[⚙️ Installation Guide](INSTALL.md)** - Set up NuMath in your project
- **[🎯 Code Examples](EXAMPLES.md)** - Real-world usage patterns
- **[📊 Benchmarks](benchmarks.html)** - Performance comparison
- **[🏠 Main README](../../NuMath/README.md)** - Library overview

---

<div align="center">

**Questions?** [Open an Issue](https://github.com/VladHordiichuk/NuEngine/issues) • [Email](mailto:vladyslav.hordiychuk@example.com)

**[🏠 Home](../../NuMath/README.md)** • **[⚙️ Install](INSTALL.md)** • **[🎯 Examples](EXAMPLES.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>