// Copyright (c) 2025 Vladyslav Hordiychuk


#pragma once

#if defined(__SSE__) || defined(_M_X64)
    #include <NuMath/Detail/SIMD/SimdSSE.hpp>
#else
    #include <NuMath/Detail/SIMD/SimdScalar.hpp>
#endif

#if defined(__AVX__) || defined(__AVX2__)
    #include <NuMath/Detail/SIMD/SimdAVX.hpp>
#endif

namespace NuMath::Simd
{
    // ============================================================
    // MATH BACKEND (AoS - Array of Structures)
    // ============================================================

#if defined(NU_MATH_FORCE_SCALAR)
    using MathBackend = NuMath::Detail::Scalar_Traits;
#elif defined(__SSE__) || defined(_M_X64)
    using MathBackend = NuMath::Detail::SSE_Traits;
#else
    using MathBackend = NuMath::Detail::Scalar_Traits;
#endif

    // ============================================================
    // BATCH BACKEND (SoA - Structure of Arrays)
    // ============================================================

#if defined(NU_MATH_FORCE_SCALAR)
    using BatchBackend = NuMath::Detail::Scalar_Traits;
#elif defined(__AVX__)
    using BatchBackend = NuMath::Detail::AVX_Traits;
#elif defined(__SSE__) || defined(_M_X64)
    using BatchBackend = NuMath::Detail::SSE_Traits;
#else
    using BatchBackend = NuMath::Detail::Scalar_Traits;
#endif
}