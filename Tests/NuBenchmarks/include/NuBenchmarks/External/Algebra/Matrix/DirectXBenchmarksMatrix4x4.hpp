// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <DirectXMath.h>
#include <benchmark/benchmark.h>
#include <NuBenchmarks/Utils/BenchmarksCommon.hpp>

namespace NuEngine::Benchmarks
{
    template <>
    inline DirectX::XMFLOAT4X4 MakeTestValue<DirectX::XMFLOAT4X4>(FastRNG& rng)
    {
        DirectX::XMFLOAT4X4 m;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                m.m[i][j] = rng.NextFloat();
            }
        }
        return m;
    }

    void RegisterMatrix4x4Benchmarks_DirectX();
}