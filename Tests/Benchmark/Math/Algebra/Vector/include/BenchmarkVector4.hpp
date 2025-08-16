// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>
#include "BenchmarkVectorCommon.hpp"
#define NU_MATH_BACKEND 1
#include <Math/Math.hpp>

namespace Benchmark
{
	void RegisterVector4Benchmarks();
}