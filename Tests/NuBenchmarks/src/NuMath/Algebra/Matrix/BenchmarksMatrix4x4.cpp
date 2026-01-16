#include <NuBenchmarks/NuMath/Algebra/Matrix/BenchmarksMatrix4x4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterMatrix4x4Benchmarks()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(NuMath::Matrix4x4, Mul, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Matrix4x4, Add, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Matrix4x4, Sub, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a - b; });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(NuMath::Matrix4x4, Mul, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Matrix4x4, Add, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Matrix4x4, Sub, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a - b; });
#endif
    }
}