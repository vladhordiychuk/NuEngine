#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector3.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector3Benchmarks_GLM()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(glm::vec3, HeavyMath, BM_Array_BinaryOp, [](const glm::vec3& a, const glm::vec3& b) {
            return glm::normalize(a + b) * glm::dot(a, b);
            });

        REGISTER_ARRAY_BENCHMARK(glm::vec3, Add, BM_Array_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec3, Sub, BM_Array_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec3, Mul, BM_Array_BinaryOp, [](auto& a, auto& b) { return a * b; });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Add, BM_Single_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Sub, BM_Single_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Mul, BM_Single_BinaryOp, [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec3, Dot, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::dot(a, b); });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Cross, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::cross(a, b); });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Distance, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_SINGLE_BENCHMARK(glm::vec3, AddAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a += b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, SubAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a -= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, MulAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec3, ScalarMul, BM_Single_ScalarOp, [](auto& a, float b) { return a * b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Div, BM_Single_ScalarOp, [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec3, ScalarMulAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a *= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, DivAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec3, Length, BM_Single_UnaryOp, [](auto& a) { return glm::length(a); });
        REGISTER_SINGLE_BENCHMARK(glm::vec3, Normalize, BM_Single_UnaryOp, [](auto& a) { return glm::normalize(a); });
#endif
    }
}