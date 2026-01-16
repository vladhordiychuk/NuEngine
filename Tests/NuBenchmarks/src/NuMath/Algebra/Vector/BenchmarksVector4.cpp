#include <NuBenchmarks/NuMath/Algebra/Vector/BenchmarksVector4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector4Benchmarks()
    {
#if ENABLE_SOA_BENCHMARKS
        REGISTER_SOA_BENCHMARK(Add,
            [](auto r, auto a, auto b, size_t count) {
                NuMath::Batch::SoA::Add(r, a, b, count);
            });

        REGISTER_SOA_SCALAR_BENCHMARK(Dot,
            [](float* r, auto a, auto b, size_t count) {
                NuMath::Batch::SoA::Dot(r, a, b, count);
            });
#endif

#if ENABLE_BATCH_BENCHMARKS
        REGISTER_BATCH_SCALAR_BENCHMARK(NuMath::NuVecStorage4, Dot,
            [](float* r, const auto* a, const auto* b, size_t count) {
                NuMath::Batch::Dot(r, a, b, count);
            });

        REGISTER_BATCH_BENCHMARK(NuMath::NuVecStorage4, Add,
            [](auto* r, auto* a, auto* b, size_t count) {
                NuMath::Batch::Add(r, a, b, count);
            });

        REGISTER_BATCH_BENCHMARK(NuMath::NuVecStorage4, Sub,
            [](auto* r, auto* a, auto* b, size_t count) {
                NuMath::Batch::Sub(r, a, b, count);
            });

        REGISTER_BATCH_BENCHMARK(NuMath::NuVecStorage4, Mul,
            [](auto* r, auto* a, auto* b, size_t count) {
                NuMath::Batch::Mul(r, a, b, count);
            });

        REGISTER_BATCH_BENCHMARK(NuMath::NuVecStorage4, Div,
            [](auto* r, auto* a, auto* b, size_t count) {
                NuMath::Batch::Div(r, a, b, count);
            });

        REGISTER_BATCH_BENCHMARK(NuMath::NuVecStorage4, HeavyMath,
            [](auto* r, auto* a, auto* b, size_t count)
            {
                for (size_t i = 0; i < count; i += 4)
                {
                    auto va = NuMath::VectorAPI::Load(a[i]);
                    auto vb = NuMath::VectorAPI::Load(b[i]);

                    auto vbNorm = NuMath::VectorAPI::Normalize4(vb);
                    auto sum = NuMath::VectorAPI::Add(va, vbNorm);
                    auto dot = NuMath::VectorAPI::Dot3(va, vb);
                    auto dotVec = NuMath::VectorAPI::SetAll(dot);

                    auto res = NuMath::VectorAPI::Mul(sum, dotVec);

                    NuMath::VectorAPI::Store(r[i], res);
                }
            });
#endif

#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_SCALAR_BENCHMARK(NuMath::Vector4, Dot, BM_Array_ScalarRet_Op,
            [](const auto& a, const auto& b) {
                return a.Dot(b);
            });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector4, Add, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector4, Sub, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector4, Mul, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_ARRAY_BENCHMARK(NuMath::Vector4, Div, BM_Array_BinaryOp,
            [](auto& a, auto& b) { return a / b; });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Add, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a + b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Sub, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a - b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Mul, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Dot, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Dot(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Distance, BM_Single_BinaryOp,
            [](auto& a, auto& b) { return a.Distance(b); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, AddAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a += b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, SubAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a -= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, MulAssign, BM_Single_AssignOp,
            [](auto& a, const auto& b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, ScalarMul, BM_Single_ScalarOp,
            [](auto& a, float b) { return a * b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Div, BM_Single_ScalarOp,
            [](auto& a, float b) { return a / b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, ScalarMulAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a *= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, DivAssign, BM_Single_ScalarAssignOp,
            [](auto& a, float b) { return a /= b; });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Length, BM_Single_UnaryOp,
            [](auto& a) { return a.Length(); });

        REGISTER_SINGLE_BENCHMARK(NuMath::Vector4, Normalize, BM_Single_UnaryOp,
            [](auto& a) { return a.Normalize(); });
#endif
    }
}