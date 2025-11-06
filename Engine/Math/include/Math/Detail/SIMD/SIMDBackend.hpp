// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#define NU_MATH_BACKEND 0

#ifndef SIMD_DISPATCH_HPP
#define SIMD_DISPATCH_HPP

// ============================================================
// NU_MATH_BACKEND:
// 0 = Scalar (forcibly)
// 1 = SIMD (forcibly)
// undefined = Auto selection by CPU flags
// ============================================================

// If you need to disable SIMD via define
#if defined(DISABLE_SIMD)
    #pragma message("SIMD manually disabled. Using scalar fallback")
    #include "SimdScalar.hpp"
    namespace Simd = Math::Scalar;
#else
    #if defined(NU_MATH_BACKEND)
        #if NU_MATH_BACKEND == 1
            #if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
                #include "SimdSSE.hpp"
                #pragma message("Using SIMD backend (forced)")
                namespace Simd = Math::SSE;
            #else
                #include "SimdScalar.hpp"
                #pragma message("⚠ SIMD forced but SSE2 not available. Falling back to scalar")
                namespace Simd = Math::Scalar;
            #endif
        #elif NU_MATH_BACKEND == 0
            #include "SimdScalar.hpp"
            #pragma message("Using scalar backend (forced)")
            namespace Simd = Math::Scalar;
        #else
            #error "Invalid NU_MATH_BACKEND value. Use 0 (scalar), 1 (SIMD) or leave undefined for auto."
        #endif
    #else
        #if defined(__SSE4_2__)
            #include "SimdSSE.hpp"
            #pragma message("Using SSE4.2 implementation")
            namespace Simd = Math::SSE;
        #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
            #include "SimdSSE.hpp"
            #pragma message("Using SSE2 implementation")
            namespace Simd = Math::SSE;
        #else
            #pragma message("⚠ No SIMD support detected. Using scalar backend")
            #include "SimdScalar.hpp"
            namespace Simd = Math::Scalar;
        #endif
    #endif
#endif

#endif
