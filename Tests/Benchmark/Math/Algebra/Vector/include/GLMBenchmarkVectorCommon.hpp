// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "BenchmarkVectorCommon.hpp"

namespace Benchmark
{
    template<>
    inline glm::vec3 MakeTestVector<glm::vec3>()
    {
        return glm::vec3( 1.0f, 2.0f, 3.0f );
    }

    template<>
    inline glm::vec2 MakeTestVector<glm::vec2>()
    {
        return glm::vec2( 1.0f, 2.0f );
    }

    template<>
    inline glm::vec4 MakeTestVector<glm::vec4>()
    {
        return glm::vec4( 1.0f, 2.0f, 3.0f, 4.0f );
    }
}



