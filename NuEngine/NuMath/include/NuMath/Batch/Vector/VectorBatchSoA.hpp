// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Detail/SIMD/SimdBackend.hpp>
#include <NuMath/Batch/Common/BatchLoopSoA.hpp>

namespace NuMath::Batch::SoA
{
    using Backend = NuMath::Simd::BatchBackend;

    // =============================================================
    // OPERATIONS
    // =============================================================
    namespace Ops
    {
        struct Add
        {
            template <typename T>
            NU_FORCEINLINE T operator()(T a, T b) const noexcept { return Backend::Add(a, b); }
            NU_FORCEINLINE float operator()(float a, float b) const noexcept { return a + b; }
        };

        struct Sub
        {
            template <typename T>
            NU_FORCEINLINE T operator()(T a, T b) const noexcept { return Backend::Sub(a, b); }
            NU_FORCEINLINE float operator()(float a, float b) const noexcept { return a - b; }
        };

        struct Mul
        {
            template <typename T>
            NU_FORCEINLINE T operator()(T a, T b) const noexcept { return Backend::Mul(a, b); }
            NU_FORCEINLINE float operator()(float a, float b) const noexcept { return a * b; }
        };

        struct Div
        {
            template <typename T>
            NU_FORCEINLINE T operator()(T a, T b) const noexcept { return Backend::Div(a, b); }
            NU_FORCEINLINE float operator()(float a, float b) const noexcept { return a / b; }
        };
    } // namespace Ops

    // =============================================================
    // PUBLIC API
    // =============================================================

    template <typename ViewR, typename ViewA, typename ViewB>
    NU_FORCEINLINE void Add(ViewR r, ViewA a, ViewB b, size_t count) noexcept
    {
        Detail::Batch::SoA::RunBinary<Backend>(r, a, b, count, Ops::Add{});
    }

    template <typename ViewR, typename ViewA, typename ViewB>
    NU_FORCEINLINE void Sub(ViewR r, ViewA a, ViewB b, size_t count) noexcept
    {
        Detail::Batch::SoA::RunBinary<Backend>(r, a, b, count, Ops::Sub{});
    }

    template <typename ViewR, typename ViewA, typename ViewB>
    NU_FORCEINLINE void Mul(ViewR r, ViewA a, ViewB b, size_t count) noexcept
    {
        Detail::Batch::SoA::RunBinary<Backend>(r, a, b, count, Ops::Mul{});
    }

    template <typename ViewR, typename ViewA, typename ViewB>
    NU_FORCEINLINE void Div(ViewR r, ViewA a, ViewB b, size_t count) noexcept
    {
        Detail::Batch::SoA::RunBinary<Backend>(r, a, b, count, Ops::Div{});
    }

    template <typename ViewR, typename ViewA, typename ViewB>
    NU_FORCEINLINE void Dot(ViewR r, ViewA a, ViewB b, size_t count) noexcept
    {
        static_assert(ViewA::Size == ViewB::Size, "Dot: Inputs must have same dimensions");

        float* outPtr;
        if constexpr (std::is_pointer_v<ViewR>)
        {
            outPtr = r;
        }
        else
        {
            static_assert(ViewR::Size == 1, "Dot: Output View must be scalar (Size 1)");
            outPtr = r.streams[0];
        }
        
        using Register = typename Backend::Register;
        constexpr size_t N = ViewA::Size;
        constexpr size_t Pack = Backend::Width;

        size_t i = 0;
        const size_t limit = (count / Pack) * Pack;

        for (; i < limit; i += Pack)
        {
            Register sum = Backend::SetZero();
            for (size_t d = 0; d < N; ++d)
            {
                Register va = Backend::Load(a.streams[d] + i);
                Register vb = Backend::Load(b.streams[d] + i);

#if defined(__FMA__)
                sum = _mm256_fmadd_ps(va, vb, sum);
#else
                sum = Backend::Add(sum, Backend::Mul(va, vb));
#endif
            }
            Backend::Stream(outPtr + i, sum);
        }

        for (; i < count; ++i)
        {
            float sum = 0.0f;
            for (size_t d = 0; d < N; ++d)
            {
                sum += a.streams[d][i] * b.streams[d][i];
            }
            outPtr[i] = sum;
        }
    }
} // namespace NuMath::Batch::SoA