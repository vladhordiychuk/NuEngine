#pragma once

#ifndef SIMD_DISPATCH_HPP
#define SIMD_DISPATCH_HPP

#if defined(DISABLE_SIMD)
	#pragma message("SIMD manually disabled. Using scalar fallback")
	#include "SIMDScalar.hpp"
	namespace Simd = Engine::Math::Simd_Scalar;

#elif defined(__SSE4_2__)
	#include "SIMD_SSE.hpp"
	#pragma message("Using SSE4.2 implementation")
	namespace Simd = Engine::Math::Simd_SSE;

#elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
	#include "SIMD_SSE.hpp"
	#pragma message("Using SSE2 implementation")
	namespace Simd = Engine::Math::Simd_SSE;

#else
	#warning "No SIMD support detected. Falling back to scalar implementation"
	#if __has_include("SIMDScalar.hpp")
		#include "SIMDScalar.hpp"
		namespace Simd = Engine::Math::Simd_Scalar;
	#else
		#error "SIMDScalar.hpp not found. Engine requires SIMD (SSE2+) or a scalar fallback."
	#endif
#endif

#endif // SIMD_DISPATCH_HPP