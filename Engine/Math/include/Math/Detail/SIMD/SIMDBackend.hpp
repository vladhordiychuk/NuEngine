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
    #include "SIMDScalar.hpp"
    namespace Simd = NuEngine::Math::Simd_Scalar;
#else
    #if defined(NU_MATH_BACKEND)
        #if NU_MATH_BACKEND == 1
            #if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
                #include "SIMD_SSE.hpp"
                #pragma message("Using SIMD backend (forced)")
                namespace Simd = NuEngine::Math::Simd_SSE;
            #else
                #include "SIMDScalar.hpp"
                #pragma message("⚠ SIMD forced but SSE2 not available. Falling back to scalar")
                namespace Simd = NuEngine::Math::Simd_Scalar;
            #endif
        #elif NU_MATH_BACKEND == 0
            #include "SIMDScalar.hpp"
            #pragma message("Using scalar backend (forced)")
            namespace Simd = NuEngine::Math::Simd_Scalar;
        #else
            #error "Invalid NU_MATH_BACKEND value. Use 0 (scalar), 1 (SIMD) or leave undefined for auto."
        #endif
    #else
        #if defined(__SSE4_2__)
            #include "SIMD_SSE.hpp"
            #pragma message("Using SSE4.2 implementation")
            namespace Simd = NuEngine::Math::Simd_SSE;
        #elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
            #include "SIMD_SSE.hpp"
            #pragma message("Using SSE2 implementation")
            namespace Simd = NuEngine::Math::Simd_SSE;
        #else
            #pragma message("⚠ No SIMD support detected. Using scalar backend")
            #include "SIMDScalar.hpp"
            namespace Simd = NuEngine::Math::Simd_Scalar;
        #endif
    #endif
#endif

#endif
