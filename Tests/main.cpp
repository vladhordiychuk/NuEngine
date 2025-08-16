#include <gtest/gtest.h>

#ifdef RUN_BENCHMARKS
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
#endif // RUN_BENCHMARKS

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int test_result = RUN_ALL_TESTS();
    if (test_result != 0) return test_result;

#ifdef RUN_BENCHMARKS
    PinToCore(0);
    WarmupCPU();

    ::benchmark::Initialize(&argc, argv);

    Benchmark::RegisterVector2Benchmarks();
    Benchmark::RegisterVector3Benchmarks();
    Benchmark::RegisterVector4Benchmarks();

    Benchmark::RegisterVector2Benchmarks_GLM();
    Benchmark::RegisterVector3Benchmarks_GLM();
    Benchmark::RegisterVector4Benchmarks_GLM();

    ::benchmark::RunSpecifiedBenchmarks();
#endif

    return 0;
}
