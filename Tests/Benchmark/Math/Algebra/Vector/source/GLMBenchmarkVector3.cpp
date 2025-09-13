#include <Tests/Benchmark/Math/Algebra/Vector/include/GLMBenchmarkVector3.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector3Benchmarks_GLM()
    {
        REGISTER_VECTOR_BENCHMARK(glm::vec3, Add, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, AddAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Sub, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, SubAssign, BM_Vector_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Mul, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, MulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, ScalarMul, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a * b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, ScalarMulAssign, BM_Vector_AssignScalarOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Div, BM_Vector_ScalarParam,
            [](auto& a, float b) { return a / b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, DivAssign, BM_Vector_AssignScalarOp,
            [](auto& a, const auto& b) { return a /= b; });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Dot, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::dot(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Cross, BM_Vector_BinaryOp,
            [](const auto& a, const auto& b) { return glm::cross(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Distance, BM_Vector_BinaryOp,
            [](auto& a, auto& b) { return glm::distance(a, b); });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Length, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::length(a); });

        REGISTER_VECTOR_BENCHMARK(glm::vec3, Normalize, BM_Vector_UnaryFloat,
            [](auto& a) { return glm::normalize(a); });
    }
}