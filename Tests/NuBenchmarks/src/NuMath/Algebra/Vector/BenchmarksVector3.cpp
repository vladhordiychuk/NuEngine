#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector3.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector3Benchmarks()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(NuMath::Vector3, Add, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector3, Sub, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector3, Mul, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_ARRAY_SCALAR_BENCHMARK(NuMath::Vector3, Dot, BM_Array_ScalarRet_Op,
            [](const auto& a, const auto& b) { return a.Dot(b); });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Add, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Sub, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Mul, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Dot, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Dot(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Cross, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Cross(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Distance, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Distance(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, AddAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, SubAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, MulAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, ScalarMul, BM_Single_ScalarOp,
            [](auto& a, float b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Div, BM_Single_ScalarOp,
            [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, ScalarMulAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, DivAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Length, BM_Single_UnaryOp,
            [](auto& a) { return a.Length(); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector3, Normalize, BM_Single_UnaryOp,
            [](auto& a) { return a.Normalize(); });
#endif
    }
}