// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SimdBackend.hpp>

namespace NuMath::TransformAPI
{
    using Impl = Simd::MathBackend;

    /**
     * @brief Transform storage structure.
     */
    using NuTransform = Impl::NuTransform;

    [[nodiscard]] NU_FORCEINLINE NuTransform Identity() noexcept
    {
        return Impl::TransformIdentity();
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Create(Impl::NuVec4 position, Impl::NuVec4 rotation, Impl::NuVec4 scale) noexcept
    {
        return Impl::TransformCreate(position, rotation, scale);
    }

    NU_FORCEINLINE void SetPosition(NuTransform& t, Impl::NuVec4 position) noexcept
    {
        Impl::TransformSetPosition(t, position);
    }

    NU_FORCEINLINE void SetRotation(NuTransform& t, Impl::NuVec4 rotation) noexcept
    {
        Impl::TransformSetRotation(t, rotation);
    }

    NU_FORCEINLINE void SetScale(NuTransform& t, Impl::NuVec4 scale) noexcept
    {
        Impl::TransformSetScale(t, scale);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetPosition(const NuTransform& t) noexcept
    {
        return Impl::TransformGetPosition(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetRotation(const NuTransform& t) noexcept
    {
        return Impl::TransformGetRotation(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetScale(const NuTransform& t) noexcept
    {
        return Impl::TransformGetScale(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuMat4 ToMatrix(const NuTransform& t) noexcept
    {
        return Impl::TransformToMatrix(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuMat4 ToInverseMatrix(const NuTransform& t) noexcept
    {
        return Impl::TransformToInverseMatrix(t);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Inverse(const NuTransform& t) noexcept
    {
        return Impl::TransformInverse(t);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Combine(const NuTransform& parent, const NuTransform& child) noexcept
    {
        return Impl::TransformCombine(parent, child);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 TransformPoint(const NuTransform& t, Impl::NuVec4 point) noexcept
    {
        return Impl::TransformPoint(t, point);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 TransformDirection(const NuTransform& t, Impl::NuVec4 direction) noexcept
    {
        return Impl::TransformDirection(t, direction);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetForward(const NuTransform& t) noexcept
    {
        return Impl::TransformGetForward(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetUp(const NuTransform& t) noexcept
    {
        return Impl::TransformGetUp(t);
    }

    [[nodiscard]] NU_FORCEINLINE Impl::NuVec4 GetRight(const NuTransform& t) noexcept
    {
        return Impl::TransformGetRight(t);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Lerp(const NuTransform& a, const NuTransform& b, float t) noexcept
    {
        return Impl::TransformLerp(a, b, t);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Translation(Impl::NuVec4 position) noexcept
    {
        return Impl::TransformTranslation(position);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Rotation(Impl::NuVec4 rotation) noexcept
    {
        return Impl::TransformRotation(rotation);
    }

    [[nodiscard]] NU_FORCEINLINE NuTransform Scale(Impl::NuVec4 scale) noexcept
    {
        return Impl::TransformScale(scale);
    }

    [[nodiscard]] NU_FORCEINLINE bool Equal(const NuTransform& a, const NuTransform& b) noexcept
    {
        return Impl::TransformEqual(a, b);
    }
} // namespace NuMath::TransformAPI