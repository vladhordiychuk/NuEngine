#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector2.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector2Benchmarks_GLM()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(glm::vec2, Add, BM_Array_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec2, Sub, BM_Array_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec2, Mul, BM_Array_BinaryOp, [](auto& a, auto& b) { return a * b; });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Add, BM_Single_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Sub, BM_Single_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Mul, BM_Single_BinaryOp, [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec2, Dot, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::dot(a, b); });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Distance, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_SINGLE_BENCHMARK(glm::vec2, AddAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a += b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, SubAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a -= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, MulAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec2, ScalarMul, BM_Single_ScalarOp, [](auto& a, float b) { return a * b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Div, BM_Single_ScalarOp, [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec2, ScalarMulAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a *= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, DivAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec2, Length, BM_Single_UnaryOp, [](auto& a) { return glm::length(a); });
        REGISTER_SINGLE_BENCHMARK(glm::vec2, Normalize, BM_Single_UnaryOp, [](auto& a) { return glm::normalize(a); });
#endif
    }
}