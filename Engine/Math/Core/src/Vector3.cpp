#include "../include/Vector3.hpp"

namespace Engine::Math
{
    Vector3::Vector3() 
        : m_data(Simd::SetZero())
    {
    }

    Vector3::Vector3(float x, float y, float z) noexcept
        : m_data(Simd::Set(x, y, z))
    {
    }

    Vector3::Vector3(std::initializer_list<float> list)
    {
        assert(list.size() == 3);
        auto it = list.begin();
        m_data = Simd::Set(it[0], it[1], it[2], 0);
    }

    Vector3::Vector3(const Vector3& other)
        : m_data(other.m_data)
    {
    }

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return Vector3(Simd::Add(m_data, other.m_data));
    }

    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return Vector3(Simd::Sub(m_data, other.m_data));
    }

    Vector3 Vector3::operator*(float scalar) const
    {
        return Vector3(Simd::Mul(m_data, Simd::Set(scalar)));
    }

    Vector3 Vector3::operator/(float scalar) const
    {
        return Vector3(Simd::Div(m_data, Simd::Set(scalar)));
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        m_data = Simd::Add(m_data, other.m_data);
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        m_data = Simd::Sub(m_data, other.m_data);
        return *this;
    }

    Vector3& Vector3::operator*=(float scalar)
    {
        m_data = Simd::Mul(m_data, Simd::Set(scalar));
        return *this;
    }

    Vector3& Vector3::operator/=(float scalar)
    {
        m_data = Simd::Div(m_data, Simd::Set(scalar));
        return *this;
    }

    float Vector3::X() const
    {
        return Simd::GetX(m_data);
    }

    float Vector3::Y() const
    {
        return Simd::GetY(m_data);
    }

    float Vector3::Z() const
    {
        return Simd::GetZ(m_data);
    }

    bool Vector3::operator==(const Vector3& other) const
    {
        return Simd::Equal(m_data, other.m_data);
    }

    bool Vector3::operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    float Vector3::Length() const
    {
        return Simd::SqrtScalar(Simd::HorizontalAdd3(Simd::Mul(m_data, m_data)));
    }

    float Vector3::Dot(const Vector3& other) const
    {
        return Simd::HorizontalAdd3(Simd::Mul(m_data, other.m_data));
    }

    Vector3 Vector3::Normalize() const
    {
        return Vector3(Simd::Normalize(m_data));
    }

    Vector3 Vector3::Cross(const Vector3& other) const
    {
        return Vector3(Simd::Cross(m_data, other.m_data));
    }

    float Vector3::LengthSquared() const
    {
        return Simd::HorizontalAdd3(Simd::Mul(m_data, m_data));
    }

    const float* Vector3::Data() const
    {
        return reinterpret_cast<const float*>(&m_data);
    }

    const Simd::NuVec4& Vector3::SimdData() const
    {
        return m_data;
    }

    std::string Vector3::ToString() const
    {
        std::ostringstream oss;
        oss << "(" << X() << ", " << Y() << ", " << Z() << ")";
        return oss.str();
    }
}