#include <NuBenchmarks/External/Algebra/Vector/DirectXBenchmarksVector4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterVector4Benchmarks_DirectX()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_SCALAR_BENCHMARK(DirectX::XMFLOAT4, Dot, BM_Array_ScalarRet_Op,
            [](const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) {
                DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
                DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
                DirectX::XMVECTOR res = DirectX::XMVector4Dot(va, vb);
                return DirectX::XMVectorGetX(res);
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4, Add, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
            DirectX::XMVECTOR res = DirectX::XMVectorAdd(va, vb);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4, Sub, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
            DirectX::XMVECTOR res = DirectX::XMVectorSubtract(va, vb);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4, Mul, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
            DirectX::XMVECTOR res = DirectX::XMVectorMultiply(va, vb);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4, Div, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
            DirectX::XMVECTOR res = DirectX::XMVectorDivide(va, vb);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4, HeavyMath, BM_Array_BinaryOp, [](const auto& a, const auto& b)
            {
                DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
                DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
                DirectX::XMVECTOR bNorm = DirectX::XMVector4Normalize(vb);
                DirectX::XMVECTOR sum = DirectX::XMVectorAdd(va, bNorm);
                DirectX::XMVECTOR dotVec = DirectX::XMVector4Dot(va, vb);
                DirectX::XMVECTOR res = DirectX::XMVectorMultiply(sum, dotVec);
                DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Add, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR res = DirectX::XMVectorAdd(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b));
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Sub, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR res = DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b));
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Mul, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR res = DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b));
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Dot, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR res = DirectX::XMVector4Dot(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b));
            return DirectX::XMVectorGetX(res);
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Distance, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMVECTOR dist = DirectX::XMVector4Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b)));
            return DirectX::XMVectorGetX(dist);
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, AddAssign, BM_Single_AssignOp, [](auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            va = DirectX::XMVectorAdd(va, DirectX::XMLoadFloat4(&b));
            DirectX::XMStoreFloat4(&a, va); return a;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, SubAssign, BM_Single_AssignOp, [](auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            va = DirectX::XMVectorSubtract(va, DirectX::XMLoadFloat4(&b));
            DirectX::XMStoreFloat4(&a, va); return a;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, MulAssign, BM_Single_AssignOp, [](auto& a, const auto& b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            va = DirectX::XMVectorMultiply(va, DirectX::XMLoadFloat4(&b));
            DirectX::XMStoreFloat4(&a, va); return a;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, ScalarMul, BM_Single_ScalarOp, [](const auto& a, float b) {
            DirectX::XMVECTOR res = DirectX::XMVectorScale(DirectX::XMLoadFloat4(&a), b);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Div, BM_Single_ScalarOp, [](const auto& a, float b) {
            DirectX::XMVECTOR res = DirectX::XMVectorScale(DirectX::XMLoadFloat4(&a), 1.0f / b);
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, ScalarMulAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            va = DirectX::XMVectorScale(va, b);
            DirectX::XMStoreFloat4(&a, va); return a;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, DivAssign, BM_Single_ScalarAssignOp, [](auto& a, float b) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            va = DirectX::XMVectorScale(va, 1.0f / b);
            DirectX::XMStoreFloat4(&a, va); return a;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Length, BM_Single_UnaryOp, [](const auto& a) {
            return DirectX::XMVectorGetX(DirectX::XMVector4Length(DirectX::XMLoadFloat4(&a)));
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4, Normalize, BM_Single_UnaryOp, [](const auto& a) {
            DirectX::XMVECTOR res = DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&a));
            DirectX::XMFLOAT4 out; DirectX::XMStoreFloat4(&out, res); return out;
            });
#endif
    }
}