#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector2.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector2Benchmarks()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(NuMath::Vector2, Add, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector2, Sub, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector2, Mul, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_ARRAY_SCALAR_BENCHMARK(NuMath::Vector2, Dot, BM_Array_ScalarRet_Op,
            [](const auto& a, const auto& b) { return a.Dot(b); });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Add, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Sub, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Mul, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Dot, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Dot(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Distance, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Distance(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, AddAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, SubAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, MulAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, ScalarMul, BM_Single_ScalarOp,
            [](auto& a, float b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Div, BM_Single_ScalarOp,
            [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, ScalarMulAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, DivAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Length, BM_Single_UnaryOp,
            [](auto& a) { return a.Length(); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector2, Normalize, BM_Single_UnaryOp,
            [](auto& a) { return a.Normalize(); });
#endif
    }
}