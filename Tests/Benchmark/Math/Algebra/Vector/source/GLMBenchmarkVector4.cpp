#include <Tests/Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector4.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector4Benchmarks_GLM()
    {
        REGISTER_VECTOR_BENCHMARK(glm::vec4, Add, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, AddAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Sub, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, SubAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Mul, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, MulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, ScalarMul, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, ScalarMulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Div, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a / b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, DivAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a /= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Dot, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::dot(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Distance, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Length, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::length(a); });

        REGISTER_VECTOR_BENCHMARK(glm::vec4, Normalize, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::normalize(a); });
    }
}