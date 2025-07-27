#include "../include/Vector3.hpp"

namespace Engine::Math
{
    Vector3::Vector3() noexcept
        : m_data(Simd::SetZero())
    {
    }

    Vector3::Vector3(float x, float y, float z) noexcept
        : m_data(Simd::Set(x, y, z))
    {
    }

    Vector3::Vector3(std::initializer_list<float> list) noexcept
    {
        assert(list.size() == 3);
        auto it = list.begin();
        m_data = Simd::Set(it[0], it[1], it[2], 0);
    }

    Vector3::Vector3(const Vector3& other) noexcept
        : m_data(other.m_data)
    {
    }

    Vector3 Vector3::operator+(const Vector3& other) const noexcept
    {
        return Vector3(Simd::Add(m_data, other.m_data));
    }

    Vector3 Vector3::operator-(const Vector3& other) const noexcept
    {
        return Vector3(Simd::Sub(m_data, other.m_data));
    }

    Vector3 Vector3::operator*(float scalar) const noexcept
    {
        return Vector3(Simd::Mul(m_data, Simd::Set(scalar)));
    }

    Vector3 Vector3::operator/(float scalar) const noexcept
    {
        return Vector3(Simd::Div(m_data, Simd::Set(scalar)));
    }

    Vector3& Vector3::operator+=(const Vector3& other) noexcept
    {
        m_data = Simd::Add(m_data, other.m_data);
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& other) noexcept
    {
        m_data = Simd::Sub(m_data, other.m_data);
        return *this;
    }

    Vector3& Vector3::operator*=(float scalar) noexcept
    {
        m_data = Simd::Mul(m_data, Simd::Set(scalar)); 
        return *this;
    }

    Vector3& Vector3::operator/=(float scalar) noexcept
    {
        m_data = Simd::Div(m_data, Simd::Set(scalar));
        return *this;
    }

    float Vector3::X() const noexcept
    {
        return Simd::GetX(m_data);
    }

    float Vector3::Y() const noexcept
    {
        return Simd::GetY(m_data);
    }

    float Vector3::Z() const noexcept
    {
        return Simd::GetZ(m_data);
    }

    bool Vector3::operator==(const Vector3& other) const noexcept
    {
        return Simd::Equal(m_data, other.m_data);
    }

    bool Vector3::operator!=(const Vector3& other) const noexcept
    {
        return !(*this == other);
    }

    float Vector3::operator[](int index) const noexcept
    {
        return GetComponent(index);
    }

    float& Vector3::operator[](int index) noexcept
    {
        return GetComponent(index);
    }

    Vector3& Vector3::operator=(const Vector3& other) noexcept
    {
        if (this != &other)
        {
            m_data = other.m_data
        }
        return *this;
    }

    Vector3& Vector3::operator=(Vector3&& other) noexcept
    {
        if (this != &other)
        {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    float Vector3::Zero() noexcept
    {
        return Simd::SetZero();
    }

    float Vector3::One() noexcept
    {
        return Simd::Set(1.0f, 1.0f, 1.0f);
    }

    float Vector3::UnitX() noexcept
    {
        return Simd::Set(1.0f, 0.0f, 0.0f);
    }

    float Vector3::UnitY() noexcept
    {
        return Simd::Set(0.0f, 1.0f, 0.0f);
    }

    float Vector3::UnitZ() noexcept
    {
        return Simd::Set(0.0f, 0.0f, 1.0f);
    }

    float Vector3::Length() const noexcept
    {
        return Simd::SqrtScalar(Simd::HorizontalAdd3(Simd::Mul(m_data, m_data)));
    }

    float Vector3::Dot(const Vector3& other) const noexcept
    {
        return Simd::HorizontalAdd3(Simd::Mul(m_data, other.m_data));
    }

    Vector3 Vector3::Normalize() const noexcept
    {
        return Vector3(Simd::Normalize(m_data));
    }

    Vector3 Vector3::Cross(const Vector3& other) const noexcept
    {
        return Vector3(Simd::Cross(m_data, other.m_data));
    }

    float Vector3::LengthSquared() const noexcept
    {
        return Simd::HorizontalAdd3(Simd::Mul(m_data, m_data));
    }

    const float* Vector3::Data() const noexcept
    {
        return reinterpret_cast<const float*>(&m_data);
    }

    const Simd::NuVec4& Vector3::SimdData() const noexcept
    {
        return m_data;
    }

    float Vector3::GetComponent(int index) const noexcept
    {
        switch (index)
        {
        case 0: return Simd::GetX(m_data);
        case 1: return Simd::GetY(m_data);
        case 2: return Simd::GetZ(m_data);
        default: assert(false); return 0.0f;
        }
    }

    std::string Vector3::ToString() const
    {
        std::ostringstream oss;
        oss << "(" << X() << ", " << Y() << ", " << Z() << ")";
        return oss.str();
    }
}