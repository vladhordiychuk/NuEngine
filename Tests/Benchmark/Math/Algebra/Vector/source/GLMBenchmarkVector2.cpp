#include <Tests/Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector2.hpp>

namespace Benchmark
{
    void RegisterVector2Benchmarks_GLM()
    {
        REGISTER_VECTOR_BENCHMARK(glm::vec2, Add_Array100k, BM_Vector_Array_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Add, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, AddAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Sub, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, SubAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Mul, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, MulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, ScalarMul, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, ScalarMulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Div, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a / b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, DivAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a /= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Dot, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::dot(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Distance, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Length, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::length(a); });

        REGISTER_VECTOR_BENCHMARK(glm::vec2, Normalize, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::normalize(a); });
    }
}