// Copyright(c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#ifdef NU_PROFILING_ENABLED
	#include <tracy/Tracy.hpp>
	#define NU_PROFILE_SCOPE(name)  ZoneScopedN(name)
	#define NU_PROFILE_FRAME()      FrameMark
	#define NU_PROFILE_ALLOC(ptr, size) TracyAlloc(ptr, size)
	#define NU_PROFILE_FREE(ptr)    TracyFree(ptr)
#else
	#define NU_PROFILE_SCOPE(name)
	#define NU_PROFILE_FRAME()
	#define NU_PROFILE_ALLOC(ptr, size)
	#define NU_PROFILE_FREE(ptr)
#endif