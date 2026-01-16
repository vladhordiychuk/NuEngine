#include <NuBenchmarks/External/Algebra/Vector/GLMBenchmarksVector4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector4Benchmarks_GLM()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_SCALAR_BENCHMARK(glm::vec4, Dot, BM_Array_ScalarRet_Op,
            [](const auto& a, const auto& b) { return glm::dot(a, b); });

        REGISTER_ARRAY_BENCHMARK(glm::vec4, Add, BM_Array_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec4, Sub, BM_Array_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec4, Mul, BM_Array_BinaryOp, [](auto& a, auto& b) { return a * b; });
        REGISTER_ARRAY_BENCHMARK(glm::vec4, Div, BM_Array_BinaryOp, [](auto& a, auto& b) { return a / b; });

        REGISTER_ARRAY_BENCHMARK(glm::vec4, HeavyMath, BM_Array_BinaryOp, [](const glm::vec4& a, const glm::vec4& b)
            {
                glm::vec4 bNorm = glm::normalize(b);
                glm::vec4 sum = a + bNorm;
                float dot = glm::dot(a, b);
                return sum * dot;
            });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Add, BM_Single_BinaryOp, [](auto& a, auto& b) { return a + b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Sub, BM_Single_BinaryOp, [](auto& a, auto& b) { return a - b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Mul, BM_Single_BinaryOp, [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec4, Dot, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::dot(a, b); });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Distance, BM_Single_BinaryOp, [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_SINGLE_BENCHMARK(glm::vec4, AddAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a += b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, SubAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a -= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, MulAssign, BM_Single_AssignOp, [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec4, ScalarMul, BM_Single_ScalarOp, [](auto& a, float b) { return a * b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Div, BM_Single_ScalarOp, [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec4, ScalarMulAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a *= b; });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, DivAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(glm::vec4, Length, BM_Single_UnaryOp, [](auto& a) { return glm::length(a); });
        REGISTER_SINGLE_BENCHMARK(glm::vec4, Normalize, BM_Single_UnaryOp, [](auto& a) { return glm::normalize(a); });
#endif
    }
}