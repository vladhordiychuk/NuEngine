#include <NuBenchmarks/External/Algebra/Matrix/GLMBenchmarksMatrix4x4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterMatrix4x4Benchmarks_GLM()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(glm::mat4, Mul, BM_Array_BinaryOp, [](const auto& a, const auto& b) { return a * b; });
        REGISTER_ARRAY_BENCHMARK(glm::mat4, Add, BM_Array_BinaryOp, [](const auto& a, const auto& b) { return a + b; });
        REGISTER_ARRAY_BENCHMARK(glm::mat4, Sub, BM_Array_BinaryOp, [](const auto& a, const auto& b) { return a - b; });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(glm::mat4, Mul, BM_Single_BinaryOp, [](const auto& a, const auto& b) { return a * b; });
        REGISTER_SINGLE_BENCHMARK(glm::mat4, Add, BM_Single_BinaryOp, [](const auto& a, const auto& b) { return a + b; });
        REGISTER_SINGLE_BENCHMARK(glm::mat4, Sub, BM_Single_BinaryOp, [](const auto& a, const auto& b) { return a - b; });
#endif
    }
}