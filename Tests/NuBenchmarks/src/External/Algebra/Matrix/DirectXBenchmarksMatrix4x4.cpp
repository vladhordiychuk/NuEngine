#include <NuBenchmarks/External/Algebra/Matrix/DirectXBenchmarksMatrix4x4.hpp>
#include <NuBenchmarks/Utils/BenchmarksConfig.hpp>

namespace NuEngine::Benchmarks
{
    void RegisterMatrix4x4Benchmarks_DirectX()
    {
#if ENABLE_ARRAY_BENCHMARKS
        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4X4, Mul, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX ma = DirectX::XMLoadFloat4x4(&a);
            DirectX::XMMATRIX mb = DirectX::XMLoadFloat4x4(&b);
            DirectX::XMMATRIX res = DirectX::XMMatrixMultiply(ma, mb);
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, res); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4X4, Add, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX ma = DirectX::XMLoadFloat4x4(&a);
            DirectX::XMMATRIX mb = DirectX::XMLoadFloat4x4(&b);
            ma.r[0] = DirectX::XMVectorAdd(ma.r[0], mb.r[0]);
            ma.r[1] = DirectX::XMVectorAdd(ma.r[1], mb.r[1]);
            ma.r[2] = DirectX::XMVectorAdd(ma.r[2], mb.r[2]);
            ma.r[3] = DirectX::XMVectorAdd(ma.r[3], mb.r[3]);
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, ma); return out;
            });

        REGISTER_ARRAY_BENCHMARK(DirectX::XMFLOAT4X4, Sub, BM_Array_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX ma = DirectX::XMLoadFloat4x4(&a);
            DirectX::XMMATRIX mb = DirectX::XMLoadFloat4x4(&b);
            ma.r[0] = DirectX::XMVectorSubtract(ma.r[0], mb.r[0]);
            ma.r[1] = DirectX::XMVectorSubtract(ma.r[1], mb.r[1]);
            ma.r[2] = DirectX::XMVectorSubtract(ma.r[2], mb.r[2]);
            ma.r[3] = DirectX::XMVectorSubtract(ma.r[3], mb.r[3]);
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, ma); return out;
            });
#endif

#if ENABLE_SINGLE_BENCHMARKS
        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4X4, Mul, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX res = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&a), DirectX::XMLoadFloat4x4(&b));
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, res); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4X4, Add, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX ma = DirectX::XMLoadFloat4x4(&a);
            DirectX::XMMATRIX mb = DirectX::XMLoadFloat4x4(&b);
            ma.r[0] = DirectX::XMVectorAdd(ma.r[0], mb.r[0]);
            ma.r[1] = DirectX::XMVectorAdd(ma.r[1], mb.r[1]);
            ma.r[2] = DirectX::XMVectorAdd(ma.r[2], mb.r[2]);
            ma.r[3] = DirectX::XMVectorAdd(ma.r[3], mb.r[3]);
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, ma); return out;
            });

        REGISTER_SINGLE_BENCHMARK(DirectX::XMFLOAT4X4, Sub, BM_Single_BinaryOp, [](const auto& a, const auto& b) {
            DirectX::XMMATRIX ma = DirectX::XMLoadFloat4x4(&a);
            DirectX::XMMATRIX mb = DirectX::XMLoadFloat4x4(&b);
            ma.r[0] = DirectX::XMVectorSubtract(ma.r[0], mb.r[0]);
            ma.r[1] = DirectX::XMVectorSubtract(ma.r[1], mb.r[1]);
            ma.r[2] = DirectX::XMVectorSubtract(ma.r[2], mb.r[2]);
            ma.r[3] = DirectX::XMVectorSubtract(ma.r[3], mb.r[3]);
            DirectX::XMFLOAT4X4 out; DirectX::XMStoreFloat4x4(&out, ma); return out;
            });
#endif
    }
}