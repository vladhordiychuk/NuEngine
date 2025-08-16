#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <windows.h>

#include "Benchmark/Math/Algebra/Vector/include/BenchmarkVector2.hpp"
#include "Benchmark/Math/Algebra/Vector/include/BenchmarkVector3.hpp"
#include "Benchmark/Math/Algebra/Vector/include/BenchmarkVector4.hpp"

#include "Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector2.hpp"
#include "Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector3.hpp"
#include "Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector4.hpp"

void PinToCore(size_t coreId = 0) 
{
    DWORD_PTR mask = (1ull << coreId);
    SetProcessAffinityMask(GetCurrentProcess(), mask);
}

void WarmupCPU()
{
    volatile double x = 1.0;
    for (int i = 0; i < 100000000; ++i) 
    {
        x *= 1.0000001;
        x /= 1.0000001;
    }
}

int main(int argc, char** argv)
{
    PinToCore(0);

    ::testing::InitGoogleTest(&argc, argv);
    int test_result = RUN_ALL_TESTS();
    if (test_result != 0) return test_result;

    WarmupCPU();

    ::benchmark::Initialize(&argc, argv);

    Benchmark::RegisterVector2Benchmarks();
    Benchmark::RegisterVector3Benchmarks();
    Benchmark::RegisterVector4Benchmarks();

    Benchmark::RegisterVector2Benchmarks_GLM();
    Benchmark::RegisterVector3Benchmarks_GLM();
    Benchmark::RegisterVector4Benchmarks_GLM();

    int fake_argc = 3;
    const char* fake_argv[] = 
    {
        argv[0],
        "--benchmark_min_time=2s",      // 2 seconds
        "--benchmark_repetitions=1",   // 3 repeats
    };
    ::benchmark::Initialize(&fake_argc, const_cast<char**>(fake_argv));

    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
