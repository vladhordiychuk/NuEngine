// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>
#include <cstdlib>
#include <cstdio>

#if defined(_MSC_VER)
    #define NU_FORCEINLINE __forceinline
    #define NU_NO_INLINE __declspec(noinline)
    #define NU_RESTRICT __restrict
    #define NU_DEPRECATED(msg) __declspec(deprecated(msg))
    #define NU_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #define NU_FORCEINLINE inline __attribute__((always_inline))
    #define NU_NO_INLINE __attribute__((noinline))
    #define NU_RESTRICT __restrict__
    #define NU_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #define NU_DEBUG_BREAK() __builtin_trap()
#else
    #define NU_FORCEINLINE inline
    #define NU_NO_INLINE
    #define NU_RESTRICT
    #define NU_DEPRECATED(msg)
    #define NU_DEBUG_BREAK()
#endif

#if defined(__ARM_NEON) || defined(__aarch64__)
    #include <arm_neon.h>
    #define NU_USE_NEON 1
#elif defined(__SSE__) || defined(_M_X64)
    #include <immintrin.h>
    #define NU_USE_SSE 1
#endif

#if defined(__AVX__)
    #include <immintrin.h>
    #define NU_USE_AVX 1
#endif

namespace NuMath::Detail
{
    NU_NO_INLINE inline void AssertHandler(const char* expression, const char* message, const char* file, int line)
    {
        std::fprintf(stderr,
            "=========================================\n"
            "       NuMath ASSERTION FAILED\n"
            "=========================================\n"
            "Expr:    %s\n"
            "Message: %s\n"
            "File:    %s\n"
            "Line:    %d\n"
            "=========================================\n",
            expression, message ? message : "N/A", file, line);
        NU_DEBUG_BREAK();
        std::abort();
    }
}

#if !defined(NDEBUG) || defined(NU_ENABLE_ASSERTS)
    #define NU_MATH_ASSERT(expr, msg) \
        do { \
            if ( !(expr) ) { \
                NuMath::Detail::AssertHandler(#expr, msg, __FILE__, __LINE__); \
            } \
        } while (0)
#else
    #define NU_MATH_ASSERT(expr, msg) do { (void)sizeof(expr); } while(0)
#endif