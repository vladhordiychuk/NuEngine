<div align="center">

# ⚙️ Installation & Setup

**[🏠 Home](../../NuMath/README.md)** • **[📖 API Reference](API.md)** • **[🎯 Examples](EXAMPLES.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>

---

## 📋 Requirements

### Minimum Requirements

| Component | Version | Notes |
|-----------|---------|-------|
| **C++ Standard** | C++20 | Required for concepts, modules |
| **Compiler** | See below | MSVC/GCC/Clang supported |
| **CPU** | SSE4.1+ | Intel 2008+, AMD 2011+ |
| **CMake** | 3.20+ | Optional (for build system) |

### Supported Compilers

✅ **MSVC** 19.29+ (Visual Studio 2019 16.10+)  
✅ **GCC** 11.0+  
✅ **Clang** 13.0+  
✅ **AppleClang** 14.0+ (Xcode 14+)

### Optional (Performance)

🚀 **AVX2** support (Intel Haswell 2013+, AMD Excavator 2015+) - Auto-detected, +30-40% performance  
🚀 **FMA** support (Same as AVX2) - Fused multiply-add instructions

---

## 🚀 Quick Install (CMake)

### Method 1: As a Subdirectory (Recommended)

```cmake
# In your project's CMakeLists.txt

# 1. Add NuEngine as subdirectory
add_subdirectory(external/NuEngine)

# 2. Link NuMath to your target
target_link_libraries(YourGame PRIVATE NuMath)

# 3. Enable C++20
target_compile_features(YourGame PUBLIC cxx_std_20)
```

### Method 2: Manual Include Path

```cmake
# If NuEngine is not in your project tree

# 1. Add include directory
target_include_directories(YourGame PUBLIC
    "${NUENGINE_PATH}/NuMath/include"
)

# 2. Enable C++20
target_compile_features(YourGame PUBLIC cxx_std_20)

# 3. (Optional) Enable AVX2 for best performance
if(MSVC)
    target_compile_options(YourGame PRIVATE /arch:AVX2)
else()
    target_compile_options(YourGame PRIVATE -mavx2 -mfma)
endif()
```

### Complete Example

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyGame CXX)

# Add NuEngine
add_subdirectory(external/NuEngine)

# Create your executable
add_executable(MyGame
    src/main.cpp
    src/Physics.cpp
    src/Renderer.cpp
)

# Link NuMath
target_link_libraries(MyGame PRIVATE NuMath)

# C++20 + AVX2
target_compile_features(MyGame PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(MyGame PRIVATE 
        /W4        # Warning level 4
        /arch:AVX2 # Enable AVX2
    )
else()
    target_compile_options(MyGame PRIVATE
        -Wall -Wextra -Wpedantic
        -mavx2 -mfma
    )
endif()
```

---

## 📦 Manual Installation (No CMake)

### Step 1: Copy Files

```bash
# Copy the include directory to your project
cp -r NuEngine/NuMath/include/Math /path/to/your/project/include/
```

### Step 2: Configure Compiler

**Visual Studio (MSVC):**
```
Project Properties → C/C++ → General → Additional Include Directories
Add: $(ProjectDir)\include

Project Properties → C/C++ → Language → C++ Language Standard
Set: ISO C++20 Standard (/std:c++20)

Project Properties → C/C++ → Code Generation → Enable Enhanced Instruction Set
Set: Advanced Vector Extensions 2 (/arch:AVX2)
```

**GCC/Clang:**
```bash
g++ -std=c++20 -mavx2 -mfma -I./include src/main.cpp -o game
```

---

## ✅ Verify Installation

### Test Program

Create `test_numath.cpp`:

```cpp
#include <Math/NuMath.hpp>
#include <iostream>

int main() {
    using namespace NuMath;
    
    // Test Vector3
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);
    Vector3 result = a + b;
    
    std::cout << "Vector3 Test: " << result << std::endl;
    // Expected: (5, 7, 9)
    
    // Test Matrix4x4
    Matrix4x4 transform = Matrix4x4::CreateTranslation({10, 0, 5});
    Vector4 point(0, 0, 0, 1);
    Vector4 transformed = transform * point;
    
    std::cout << "Matrix4x4 Test: " << transformed << std::endl;
    // Expected: (10, 0, 5, 1)
    
    // Test Color
    Color color = Color::FromSRGB(1.0f, 0.5f, 0.25f);
    uint32_t rgba = color.ToRGBA32();
    
    std::cout << "Color Test: 0x" << std::hex << rgba << std::endl;
    // Expected: 0xFF40BCFF (or similar, depends on gamma)
    
    std::cout << "\n✅ NuMath is working correctly!" << std::endl;
    return 0;
}
```

### Compile & Run

```bash
# CMake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
./test_numath

# Manual (GCC)
g++ -std=c++20 -mavx2 -O3 -I./include test_numath.cpp -o test_numath
./test_numath

# Manual (MSVC)
cl /std:c++20 /arch:AVX2 /O2 /EHsc /I.\include test_numath.cpp
test_numath.exe
```

**Expected Output:**
```
Vector3 Test: (5, 7, 9)
Matrix4x4 Test: (10, 0, 5, 1)
Color Test: 0xff40bcff

✅ NuMath is working correctly!
```

---

## 🛠️ Platform-Specific Notes

### Windows (Visual Studio)

1. **Enable Natvis Debugging:**
   - Copy `NuMath/NuMath.natvis` to your project root
   - Or: `Tools → Options → Debugging → Native → Add` and select the `.natvis` file

2. **Precompiled Headers (Optional):**
   ```cpp
   // pch.h
   #include <Math/NuMath.hpp>
   ```

3. **Recommended Project Settings:**
   - Configuration: `Release`
   - Optimization: `/O2` (Maximum Speed)
   - Intrinsics: `Yes (/Oi)`
   - Whole Program Optimization: `Yes`

### Linux (GCC/Clang)

```bash
# Install build tools
sudo apt-get install build-essential cmake

# Verify CPU supports AVX2
grep -o 'avx2' /proc/cpuinfo | head -1
# Output: avx2 (if supported)

# Build with optimizations
g++ -std=c++20 -O3 -march=native -flto \
    -I./include src/main.cpp -o game
```

### macOS (AppleClang)

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Build with optimizations
clang++ -std=c++20 -O3 -march=native \
    -I./include src/main.cpp -o game
    
# Note: Apple Silicon (M1/M2) uses ARM NEON, not AVX2
# NuMath auto-detects and uses appropriate SIMD backend
```

---

## 🎯 Advanced Configuration

### Custom SIMD Backend Selection

```cpp
// Force SSE backend (for testing or legacy CPUs)
#define NUMATH_FORCE_SSE
#include <Math/NuMath.hpp>

// Force Scalar backend (no SIMD)
#define NUMATH_FORCE_SCALAR
#include <Math/NuMath.hpp>

// Auto-detect (default, recommended)
#include <Math/NuMath.hpp>
```

### Performance Profiling Macros

```cpp
// Enable performance counters
#define NUMATH_ENABLE_PROFILING
#include <Math/NuMath.hpp>

// Usage:
NuMath::Profiler::Begin("PhysicsUpdate");
// ... your code ...
NuMath::Profiler::End("PhysicsUpdate");
NuMath::Profiler::Report();
```

### Custom Allocators (for Batch Processing)

```cpp
#include <NuMath/Batch/BatchAllocator.hpp>

// Use custom aligned allocator for SoA data
auto allocator = NuMath::Batch::AlignedAllocator<float, 32>();
std::vector<float, decltype(allocator)> data(10000, allocator);
```

---

## 🐛 Troubleshooting

### Error: "cannot open source file Math/NuMath.hpp"

**Solution:** Include path not configured correctly.

```cmake
# CMake: Add this line
target_include_directories(YourTarget PUBLIC "${NUENGINE_PATH}/NuMath/include")

# Manual: Add -I flag
g++ -I./NuEngine/NuMath/include ...
```

### Error: "ISO C++20 does not allow ..."

**Solution:** Enable C++20 standard.

```cmake
# CMake
target_compile_features(YourTarget PUBLIC cxx_std_20)

# GCC/Clang
g++ -std=c++20 ...

# MSVC
cl /std:c++20 ...
```

### Warning: "AVX2 not supported on this CPU"

**Solution:** NuMath auto-falls back to SSE or scalar. No action needed, but performance will be reduced (~30%).

```cpp
// Check detected SIMD level at runtime
#include <Math/Core/SimdDetect.hpp>

if (NuMath::SIMD::HasAVX2()) {
    std::cout << "Using AVX2 backend\n";
} else if (NuMath::SIMD::HasSSE41()) {
    std::cout << "Using SSE4.1 backend\n";
} else {
    std::cout << "Using scalar backend\n";
}
```

### Performance is slower than expected

**Checklist:**
1. ✅ Release build? (`-O3` or `/O2`)
2. ✅ AVX2 enabled? (`-mavx2` or `/arch:AVX2`)
3. ✅ Link-Time Optimization? (`-flto` or `/GL`)
4. ✅ Using Batch API for large datasets?
5. ✅ Data is aligned? (`alignas(32)` for AVX2)

---

## 📚 Next Steps

- **[📖 Read API Documentation](API.md)** - Learn how to use Vectors, Matrices, and Colors
- **[🎯 See Code Examples](EXAMPLES.md)** - Game loops, particle systems, ray casting
- **[📊 View Benchmarks](benchmarks.html)** - Understand performance characteristics
- **[🏠 Back to Main README](../../NuMath/README.md)** - Overview and features

---

<div align="center">

**Need Help?** [Open an Issue](https://github.com/VladHordiichuk/NuEngine/issues) • [Email Support](mailto:vladyslav.hordiychuk@example.com)

**[🏠 Home](../../NuMath/README.md)** • **[📖 API](API.md)** • **[🎯 Examples](EXAMPLES.md)** • **[📊 Benchmarks](benchmarks.html)**

</div>