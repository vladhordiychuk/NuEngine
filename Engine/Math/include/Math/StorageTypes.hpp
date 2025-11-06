// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Types.hpp>

namespace NuEngine::Math
{ 
    using NuFloat = Core::Types::NuFloat;

    struct alignas(16) NuVecStorage4 { NuFloat x, y, z, w; };

    struct alignas(16) NuQuatStorage { NuFloat x, y, z, w; };

    struct alignas(16) NuMatStorage4x4 { NuVecStorage4 cols[4]; };
    struct alignas(32) NuVecStorage8 { NuFloat f[8]; };
}