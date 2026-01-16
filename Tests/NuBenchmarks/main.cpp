#include <benchmark/benchmark.h>
#include <windows.h>

#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>
#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector2.hpp>
#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector3.hpp>
#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector4.hpp>
#include <NuBenchmarks/NuMath/Algebra/Matrix/BenchmarksMatrix4x4.hpp>
#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector2.hpp>
#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector3.hpp>
#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector4.hpp>
#include <NuBenchmarks/External/Algebra/Matrix/GLMBenchmarksMatrix4x4.hpp>
#include <NuBenchmarks/External/Algebra/Vector/DirectXBenchmarksVector4.hpp>
#include <NuBenchmarks/External/Algebra/Matrix/DirectXBenchmarksMatrix4x4.hpp>

void PinToCore(size_t coreId = 0)
{
    DWORD_PTR mask = (1ull << coreId);
    SetProcessAffinityMask(GetCurrentProcess(), mask);
}

void WarmupCPU()
{
    volatile double x = 1.0;
    for (int i = 0; i < 100000000; ++i) {
        x *= 1.0000001;
        x /= 1.0000001;
    }
}

int main(int argc, char** argv)
{
#ifdef CI_BUILD
    return 0;
#endif
    PinToCore(0);
    WarmupCPU();

    NuEngine::Benchmarks::RegisterVector2Benchmarks_GLM();
    NuEngine::Benchmarks::RegisterVector3Benchmarks_GLM();
    NuEngine::Benchmarks::RegisterVector4Benchmarks_GLM();
    NuEngine::Benchmarks::RegisterMatrix4x4Benchmarks_GLM();

    NuEngine::Benchmarks::RegisterVector2Benchmarks();
    NuEngine::Benchmarks::RegisterVector3Benchmarks();
    NuEngine::Benchmarks::RegisterVector4Benchmarks();
    NuEngine::Benchmarks::RegisterMatrix4x4Benchmarks();

    NuEngine::Benchmarks::RegisterVector4Benchmarks_DirectX();
    NuEngine::Benchmarks::RegisterMatrix4x4Benchmarks_DirectX();

    int fake_argc = 3;
    const char* fake_argv[] =
    {
        argv[0],
        "--benchmark_min_time=" IN_TIME_STR,
        "--benchmark_repetitions=1",
    };

    ::benchmark::Initialize(&fake_argc, const_cast<char**>(fake_argv));
    ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}