// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>

#include <array>

namespace NuMath
{
    struct alignas(16) NuVecStorage4 { float x, y, z, w; };

    struct alignas(16) NuQuatStorage { float x, y, z, w; };

    struct alignas(16) NuColorStorage { float r, g, b, a; };

    struct alignas(16) NuMatStorage4x4 { NuVecStorage4 cols[4]; };

    struct alignas(16) NuMatStorage3x3 { NuVecStorage4 cols[3]; };

    struct alignas(16) NuMatStorage2x2 { NuVecStorage4 cols[2]; };

    struct alignas(32) NuVecStorage8 { float f[8]; };

    template <size_t N>
    struct SoAViewConst
    {
        static constexpr size_t Size = N;

        std::array<const float* NU_RESTRICT, N> streams;

        [[nodiscard]] NU_FORCEINLINE const float* X() const noexcept
        {
            NU_MATH_ASSERT(N >= 1, "Must be...");
            return streams[0];
        }

        [[nodiscard]] NU_FORCEINLINE const float* Y() const noexcept
        {
            NU_MATH_ASSERT(N >= 2, "Must be...");
            return streams[1];
        }

        [[nodiscard]] NU_FORCEINLINE const float* Z() const noexcept
        {
            NU_MATH_ASSERT(N >= 3, "Must be...");
            return streams[2];
        }

        [[nodiscard]] NU_FORCEINLINE const float* W() const noexcept
        {
            NU_MATH_ASSERT(N >= 4, "Must be...");
            return streams[3];
        }

        [[nodiscard]] NU_FORCEINLINE const float* operator[](size_t index) const noexcept
        {
            NU_MATH_ASSERT(index < N, "Index out of bounds!");
            return streams[index];
        }
    };

    template <size_t N>
    struct SoAView
    {
        static constexpr size_t Size = N;

        std::array<float* NU_RESTRICT, N> streams;

        [[nodiscard]] NU_FORCEINLINE float* X() const noexcept
        {
            NU_MATH_ASSERT(N >= 1, "Must be...");
            return streams[0];
        }

        [[nodiscard]] NU_FORCEINLINE float* Y() const noexcept
        {
            NU_MATH_ASSERT(N >= 2, "Must be...");
            return streams[1];
        }

        [[nodiscard]] NU_FORCEINLINE float* Z() const noexcept
        {
            NU_MATH_ASSERT(N >= 3, "Must be...");
            return streams[2];
        }

        [[nodiscard]] NU_FORCEINLINE float* W() const noexcept
        {
            NU_MATH_ASSERT(N >= 4, "Must be...");
            return streams[3];
        }

        [[nodiscard]] NU_FORCEINLINE float* operator[](size_t index) noexcept
        {
            NU_MATH_ASSERT(index < N, "Index out of bounds!");
            return streams[index];
        }

        [[nodiscard]] NU_FORCEINLINE float* operator[](size_t index) const noexcept
        {
            NU_MATH_ASSERT(index < N, "Index out of bounds!");
            return streams[index];
        }

        operator SoAViewConst<N>() const noexcept
        {
            SoAViewConst<N> result;
            for (size_t i = 0; i < N; ++i)
            {
                result.streams[i] = streams[i];
            }
            return result;
        }
    };

    using SoAVec4 = SoAView<4>;
    using SoAVec3 = SoAView<3>;
    using SoAVec2 = SoAView<2>;

    using SoAVec4Const = SoAViewConst<4>;
    using SoAVec3Const = SoAViewConst<3>;
    using SoAVec2Const = SoAViewConst<2>;

    using SoAMat4 = SoAView<16>;
    using SoAMat3 = SoAView<9>;
    using SoAMat2 = SoAView<4>;

    using SoAMat4Const = SoAViewConst<16>;
    using SoAMat3Const = SoAViewConst<9>;
    using SoAMat2Const = SoAViewConst<4>;
} // namespace NuMath