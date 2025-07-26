#pragma once

#include "../../SIMD/SIMDBackend.hpp"

#include <string>
#include <initializer_list>
#include <sstream>

namespace Engine::Math
{
    class alignas(16) Vector3
    {
    public:
        Vector3() noexcept = default;
        Vector3(float x, float y, float z) noexcept;
        Vector3(std::initializer_list<float> list) noexcept;
        Vector3(const Vector3&) noexcept = default;
        Vector3(Vector3&&) noexcept = default;

        [[nodiscard]] Vector3 operator+(const Vector3&) const noexcept;
        [[nodiscard]] Vector3 operator-(const Vector3&) const noexcept;
        [[nodiscard]] Vector3 operator*(float) const noexcept;
        [[nodiscard]] Vector3 operator/(float) const noexcept;

        Vector3& operator+=(const Vector3&) noexcept;
        Vector3& operator-=(const Vector3&) noexcept;
        Vector3& operator*=(float) noexcept;
        Vector3& operator/=(float) noexcept;

        [[nodiscard]] float GetComponent(int) const noexcept;

        [[nodiscard]] bool operator==(const Vector3&) const noexcept;
        [[nodiscard]] bool operator!=(const Vector3&) const noexcept;

        [[nodiscard]] float operator[](int) const noexcept;
        [[nodiscard]] float operator[](int) noexcept
        Vector3& operator=(const Vector3&) noexcept = default;
        Vector3& operator=(Vector&&) noexcept = default;

        [[nodiscard]] static Vector3 Zero() noexcept;
        [[nodiscard]] static Vector3 One() noexcept;
        [[nodiscard]] static Vector3 UnitX() noexcept;
        [[nodiscard]] static Vector3 UnitY() noexcept;
        [[nodiscard]] static Vector3 UnitZ() noexcept;

        [[nodiscard]] float Length() const noexcept;
        [[nodiscard]] float LengthSquared() const noexcept;
        [[nodiscard]] float Dot(const Vector3&) const noexcept;
        [[nodiscard]] const float* Data() const noexcept;
        [[nodiscard]] const Simd::NuVec4& SimdData() const noexcept;

        [[nodiscard]] Vector3 Normalize() const noexcept;
        [[nodiscard]] Vector3 Cross(const Vector3&) const noexcept;

        [[nodiscard]] std::string ToString() const;

    private:
        Simd::NuVec4 m_data;

        explicit Vector3(Simd::NuVec4 simd) : m_data(simd) {}
    };
}