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
        Vector3();
        Vector3(float x, float y, float z) noexcept;
        Vector3(std::initializer_list<float> list);
        Vector3(const Vector3& other);

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(float scalar) const;
        Vector3 operator/(float scalar) const;

        Vector3& operator+=(const Vector3& other);
        Vector3& operator-=(const Vector3& other);
        Vector3& operator*=(float scalar);
        Vector3& operator/=(float scalar);

        float X() const;
        float Y() const;
        float Z() const;

        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;

        float Length() const;
        float LengthSquared() const;
        float Dot(const Vector3& other) const;
        const float* Data() const;
        const Simd::NuVec4& SimdData() const;

        Vector3 Normalize() const;
        Vector3 Cross(const Vector3& other) const;

        std::string ToString() const;

    private:
        Simd::NuVec4 m_data;

        explicit Vector3(Simd::NuVec4 simd) : m_data(simd) {}
    };
}