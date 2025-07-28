#pragma once

#include "../../SIMD/SIMDBackend.hpp"

#include <string>
#include <sstream>
#include <initializer_list>

namespace Engine::Math
{
    class alignas(16) Vector4
    {
    public:
        Vector4() noexcept = default;
        Vector4(float x, float y, float z, float w) noexcept;
        Vector4(std::initializer_list<float> list) noexcept;
        Vector4(const Vector4&) noexcept = default;
        Vector4(Vector4&&) noexcept = default;

        [[nodiscard]] Vector4 operator+(const Vector4&) const noexcept;
        [[nodiscard]] Vector4 operator-(const Vector4&) const noexcept;
        [[nodiscard]] Vector4 operator*(const Vector4&) const noexcept;
        [[nodiscard]] Vector4 operator*(float) const noexcept;
        [[nodiscard]] Vector4 operator/(float) const noexcept;

        Vector4& operator+=(const Vector4&) noexcept;
        Vector4& operator-=(const Vector4&) noexcept;
        Vector4& operator*=(float) noexcept;
        Vector4& operator/=(float) noexcept;

        [[nodiscard]] float GetComponent(int) const noexcept;
        [[nodiscard]] float X() const noexcept;
        [[nodiscard]] float Y() const noexcept;
        [[nodiscard]] float Z() const noexcept;
        [[nodiscard]] float W() const noexcept;

        [[nodiscard]] bool operator==(const Vector4&) const noexcept;
        [[nodiscard]] bool operator!=(const Vector4&) const noexcept;

        [[nodiscard]] Vector4 operator-() const noexcept;

        [[nodiscard]] float operator[](int) const noexcept;
        Vector4& operator=(const Vector4&) noexcept = default;
        Vector4& operator=(Vector4&&) noexcept = default;

        [[nodiscard]] static Vector4 Zero() noexcept;
        [[nodiscard]] static Vector4 One() noexcept;
        [[nodiscard]] static Vector4 UnitX() noexcept;
        [[nodiscard]] static Vector4 UnitY() noexcept;
        [[nodiscard]] static Vector4 UnitZ() noexcept;
        [[nodiscard]] static Vector4 UnitW() noexcept;

        [[nodiscard]] float Length() const noexcept;
        [[nodiscard]] float Dot(const Vector4&) const noexcept;
        [[nodiscard]] float LengthSquared() const noexcept;
        [[nodiscard]] float Distance(const Vector4&) const noexcept;

        [[nodiscard]] const float* Data() const noexcept;
        [[nodiscard]] const Simd::NuVec4& SimdData() const noexcept;

        [[nodiscard]] Vector4 Normalize() const noexcept;

        [[nodiscard]] std::string ToString() const;
        friend std::ostream& operator<<(std::ostream&, const Vector4&);

    private:
        Simd::NuVec4 m_data;

        explicit Vector4(Simd::NuVec4 simd) noexcept : m_data(simd) {}
    };
}