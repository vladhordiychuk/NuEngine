// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>
#include <cstdlib>
#include <iostream>

namespace NuEngine::Core::Types
{
    // ==========================
    // Integers
    // ==========================
    using NuInt8 = std::int8_t;
    using NuInt16 = std::int16_t;
    using NuInt32 = std::int32_t;
    using NuInt64 = std::int64_t;

    using NuUInt8 = std::uint8_t;
    using NuUInt16 = std::uint16_t;
    using NuUInt32 = std::uint32_t;
    using NuUInt64 = std::uint64_t;

    // Size check
    static_assert(sizeof(NuUInt8) == 1, "NuUInt8 must be 1 byte");
    static_assert(sizeof(NuUInt16) == 2, "NuUInt16 must be 2 bytes");
    static_assert(sizeof(NuUInt32) == 4, "NuUInt32 must be 4 bytes");
    static_assert(sizeof(NuUInt64) == 8, "NuUInt64 must be 8 bytes");

    static_assert(sizeof(NuUInt8) == 1, "NuUInt8 must be 1 byte");
    static_assert(sizeof(NuUInt16) == 2, "NuUInt16 must be 2 bytes");
    static_assert(sizeof(NuUInt32) == 4, "NuUInt32 must be 4 bytes");
    static_assert(sizeof(NuUInt64) == 8, "NuUInt64 must be 8 bytes");

    // ==========================
    // Logical types
    // ==========================
    using NuBool = bool;

    // ==========================
    // Real numbers
    // ==========================
    using NuFloat = float;
    using NuDouble = double;

    static_assert(sizeof(NuFloat) == 4, "NuFloat must be 4 bytes");
    static_assert(sizeof(NuDouble) == 8, "NuDouble must be 8 bytes");

    // Constants for floating point
    constexpr NuFloat NuFloatEPSILON = std::numeric_limits<NuFloat>::epsilon();
    constexpr NuDouble NuDoubleEPSILON = std::numeric_limits<NuDouble>::epsilon();

    // ==========================
    // Other useful types
    // ==========================
    using NuByte = NuUInt8;
    using NuSize = std::size_t;
    using NuChar = char;
    using NuWChar = wchar_t;
    using NuChar8 = char;      // UTF-8
    using NuChar16 = char16_t; // UTF-16
    using NuChar32 = char32_t; // UTF-32

    // ==========================
    // Constants
    // ==========================
    constexpr NuFloat NuPI32 = 3.14159265358979323846f;
    constexpr NuDouble NuPI64 = 3.14159265358979323846;

    // ==========================
    // Macros and inlines (platform-independent)
    // ==========================
#if defined(_MSC_VER)
    #define NU_FORCEINLINE __forceinline
    #define NU_NO_INLINE __declspec(noinline)
    #define NU_DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
    #define NU_FORCEINLINE inline __attribute__((always_inline))
    #define NU_NO_INLINE __attribute__((noinline))
    #define NU_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
    #define NU_FORCEINLINE inline
    #define NU_NO_INLINE
    #define NU_DEPRECATED(msg)
#endif

    // ==========================
    // Assert
    // ==========================

#if !defined(NDEBUG)
    NU_FORCEINLINE void NuAssert(bool expr, const char* msg = nullptr)
    {
        if (!expr)
        {
            if (msg)
                std::cerr << "Assertion failed: " << msg << "\n";

#if defined(_MSC_VER)
            __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
            __builtin_trap();
#else
            std::abort();
#endif
        }
    }
#else
    NU_FORCEINLINE void NuAssert(bool, const char* = nullptr) noexcept {}
#endif
}