// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.

#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>

namespace NuEngine::Core::Types
{
    // ==========================
    // Constants
    // ==========================
    constexpr float  EPSILON = std::numeric_limits<float>::epsilon();
    constexpr float  PI = 3.14159265358979323846f;
}

// ==========================
// Macros (Global Namespace)
// ==========================

#if defined(_MSC_VER)
    #define NU_FORCEINLINE __forceinline
    #define NU_NO_INLINE __declspec(noinline)
    #define NU_DEPRECATED(msg) __declspec(deprecated(msg))
    #define NU_DEBUG_BREAK() __debugbreak()
    #define NU_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(__GNUC__) || defined(__clang__)
    #define NU_FORCEINLINE inline __attribute__((always_inline))
    #define NU_NO_INLINE __attribute__((noinline))
    #define NU_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #define NU_DEBUG_BREAK() __builtin_trap()
#else
    #define NU_FORCEINLINE inline
    #define NU_NO_INLINE
    #define NU_DEPRECATED(msg)
    #define NU_DEBUG_BREAK()
    #define NU_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

// ==========================
// Custom Assertion Logic
// ==========================

namespace NuEngine::Core::Internal
{
    void TriggerAssert(const char* condition, const char* file, int line, const char* msg = nullptr);
}

#if !defined(NDEBUG) || defined(NU_ENABLE_ASSERTS)
#define NU_ASSERT(expr, msg) \
        if (!(expr)) { \
            NuEngine::Core::Internal::TriggerAssert(#expr, __FILE__, __LINE__, msg); \
            NU_DEBUG_BREAK(); \
        }
#else
    #define NU_ASSERT(expr, msg) ((void)0)
#endif