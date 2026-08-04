// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

// ==========================
// Compiler / Platform Macros (Global Namespace)
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
	#define NU_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
	#define NU_FORCEINLINE inline
	#define NU_NO_INLINE
	#define NU_DEPRECATED(msg)
	#define NU_DEBUG_BREAK()
	#define NU_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
