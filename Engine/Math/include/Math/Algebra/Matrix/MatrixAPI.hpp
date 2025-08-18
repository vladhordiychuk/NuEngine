// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#define NU_FORCEINLINE __forceinline
#else
#define NU_FORCEINLINE inline __attribute__((always_inline))
#endif

namespace NuEngine::Math::Docs::Matrix
{

}