// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Algebra/Vector/VectorAPI.hpp>
#include <Core/Types/Types.hpp>

#include <string>
#include <initializer_list>
#include <sstream>
#include <stdexcept>

namespace NuEngine::Math
{
    using NuFloat = NuEngine::Core::Types::NuFloat;
    using NuInt32 = NuEngine::Core::Types::NuInt32;
    using NuBool = NuEngine::Core::Types::NuBool;

    /*
    * @brief 3D Vector with SIMD support.
    * 
    * This class implements a 3-component vector with support for arithmetic operations, 
    * component access, normalization, length, and distance. It supports SSE optimizations.
    */
    class alignas(16) Vector3
    {
    public:
        static constexpr int Dimensions = 3;

        /*
        * @brief Creates a zero vector (0.0f, 0.0f, 0.0f).
        */
        NU_FORCEINLINE Vector3() noexcept
            : m_data(VectorAPI::SetZero())
        {
        }

        /*
        * @brief Creates a vector with specified components.
        * @param x Value on X.
        * @param y Value on Y.
        * @param z Value on Z.
        */
        NU_FORCEINLINE Vector3(NuFloat x, NuFloat y, NuFloat z) noexcept
            : m_data(VectorAPI::Set(x, y, z, 0.0f))
        {
        }

        /*
        * @brief Initialization with initializer_list.
        * @param Initialization list of 3 elements.
        */
        NU_FORCEINLINE Vector3(std::initializer_list<NuFloat> list) noexcept
        {
            assert(list.size() == 3);
            auto it = list.begin();
            NuFloat x = *it++;
            NuFloat y = *it++;
            NuFloat z = *it++;
            m_data = VectorAPI::Set(x, y, z, 0.0f);
        }

        /*
        * @brief Constructs a copy of another Vector3.
        *
        * Initializes this vector by copying all components from the given vector.
        *
        * @param other The vector to copy.
        */
        NU_FORCEINLINE Vector3(const Vector3& other) noexcept
            : m_data(other.m_data)
        {
        }

        /*
        * @brief Constructs a Vector3 by moving another one.
        *
        * Transfers the internal data from the given vector to this one.
        * The source vector is left in a valid but unspecified state.
        *
        * @param other The vector to move from.
        */
        Vector3(Vector3&& other) noexcept
            : m_data(std::move(other.m_data))
        {
        }

        /*
        * @brief Constructs a Vector3 from a SIMD vector.
        *
        * Initializes the Vector3 using the given SIMD vector without performing
        * any additional computations or conversions.
        *
        * @param simd The SIMD vector (NuVec4) used to initialize this Vector3.
        */
        explicit NU_FORCEINLINE Vector3(VectorAPI::NuVec4 vec) noexcept
            : m_data(vec) 
        {
        }

        /*
        * @brief Adding another vector to this one.
        *
        * Performs component-wise addition: each component of another vector is added to the corresponding component of this vector.
        *
        * @param other The vector to add.
        * @return A new Vector3 that is the result of the addition.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator+(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Add(m_data, other.m_data));
        }

        /*
        * @brief Subtracting another vector from this one.
        *
        * Performs component-wise subtraction: each component of another vector is subtracted to the corresponding component of this vector.
        *
        * @param other The vector to subtract.
        * @return A new Vector3 that is the result of the subtraction.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator-(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Sub(m_data, other.m_data));
        }

        /*
        * @brief Multiplying this vector by another.
        *
        * Performs component-wise multiplication: each component of the other vector
        * is multiplied by the corresponding component of this vector.
        *
        * @param other The vector to multiply with.
        * @return A new Vector3 that is the result of the multiplication.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator*(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Mul(m_data, other.m_data));
        }

        /*
        * 
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator/(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Div(m_data, other.m_data));
        }

        /*
        * @brief Multiplies the vector by a scalar.
        *
        * Scales each component of the vector by the given scalar value.
        *
        * @param scalar The scalar value to multiply by.
        * @return A new Vector3 that is the result of the multiplication.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator*(NuFloat scalar) const noexcept
        {
            return Vector3(VectorAPI::Mul(m_data, Simd::SetAll(scalar)));
        }

        /*
        * @brief Multiplies a scalar by a vector (commutative with vector * scalar).
        *
        * @param scalar The scalar value.
        * @param vec The vector to multiply.
        * @return A new Vector3 that is the result of the multiplication.
        */
        friend [[nodiscard]] NU_FORCEINLINE Vector3 operator*(NuFloat scalar, Vector3 vec) noexcept
        {
            return vec * scalar;
        }

        /*
        * @brief Divides the vector by a scalar.
        *
        * Scales each component of the vector by the reciprocal of the given scalar value.
        *
        * @param scalar The scalar value to divide by.
        * @return A new Vector3 that is the result of the division.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator/(NuFloat scalar) const noexcept
        {
            NuEngine::Core::Types::NuAssert(std::fabs(scalar) > std::numeric_limits<NuFloat>::epsilon() && "Division by zero or near zero!");
            return Vector3(VectorAPI::Div(m_data, VectorAPI::SetAll(scalar)));
        }

        /*
        * @brief Adds another vector to this one.
        *
        * Performs component-wise addition and updates the current vector.
        *
        * @param other The vector to add.
        * @return A reference to this vector after the addition.
        */
        NU_FORCEINLINE Vector3& operator+=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Add(m_data, other.m_data);
            return *this;
        }

        /*
        * @brief Subtracts another vector to this one.
        *
        * Performs component-wise subtraction and updates the current vector.
        *
        * @param other The vector to subtract.
        */
        NU_FORCEINLINE Vector3& operator-=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Sub(m_data, other.m_data);
            return *this;
        }

        /*
        * @brief Multiplies another vector to this one.
        *
        * Performs component-wise multiplication and updates the current vector.
        *
        * @param other The vector to multiply.
        * @return A reference to this vector after the multiplication.
        */
        NU_FORCEINLINE Vector3& operator*=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Mul(m_data, other.m_data);
            return *this;
        }

        /*
        * 
        */
        NU_FORCEINLINE Vector3& operator/=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Div(m_data, other.m_data);
            return *this;
        }

        /*
        * @brief Multiplies this vector by a scalar value.
        *
        * Each component (x, y, z) will be multiplied by the given scalar.
        *
        * @param scalar The scalar value to multiply the vector by.
        * @return A reference to this vector after the multiplication.
        */
        NU_FORCEINLINE Vector3& operator*=(NuFloat scalar) noexcept
        {
            m_data = VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar));
            return *this;
        }

        /*
        * @brief Divides this vector by a scalar value.
        *
        * Each component (x, y, z) will be divided by the given scalar.
        *
        * @param scalar The scalar value to divide the vector by.
        * @return A reference to this vector after the division.
        */
        NU_FORCEINLINE Vector3& operator/=(NuFloat scalar) noexcept
        {
            NuEngine::Core::Types::NuAssert(std::fabs(scalar) > std::numeric_limits<NuFloat>::epsilon() && "Division by zero or near zero!");
            m_data = VectorAPI::Div(m_data, VectorAPI::SetAll(scalar));
            return *this;
        }

        /*
        * @brief Returns the x-component
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat X() const noexcept
        {
            return VectorAPI::GetX(m_data);
        }

        /*
        * @brief Returns the y-component
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat Y() const noexcept
        {
            return VectorAPI::GetY(m_data);
        }

        /*
        * @brief Returns the z-component
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat Z() const noexcept
        {
            return VectorAPI::GetZ(m_data);
        }

        /*
        * @brief Returns a vector with the minimum components between this vector and another.
        *
        * @param other The other vector to compare with.
        *
        * @return Vector3 A vector containing the minimum values component-wise.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 Min(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Min(m_data, other.m_data));
        }

        /*
        * @brief Returns a vector with the maximum components between this vector and another.
        *
        * @param other The other vector to compare with.
        *
        * @return Vector3 A vector containing the maximum values component-wise.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 Max(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Max(m_data, other.m_data));
        }

        /*
        * @brief Returns a vector with the absolute values of each component.
        *
        * @return Vector3 A vector containing the absolute values of this vector's components.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 Abs() const noexcept
        {
            return Vector3(VectorAPI::Abs(m_data));
        }

        /*
        * @brief Compares two Vector3 instances for equality.
        *
        * @param other The vector to compare with.
        * @result true if all components are equal, false otherwise.
        */
        [[nodiscard]] NU_FORCEINLINE NuBool operator==(Vector3 other) const noexcept
        {
            return VectorAPI::Equal(m_data, other.m_data);
        }

        /*
        * @brief Checks if two Vector3 instances are nearly equal within a tolerance.
        *
        * @param other The vector to compare with.
        * @param epsilon The maximum allowed difference for components.
        *
        * @return true if all components differ no more than epsilon, false otherwise.
        */
        [[nodiscard]] NU_FORCEINLINE NuBool NearEqual(Vector3 other, NuFloat epsilon) const noexcept
        {
            return VectorAPI::NearEqual(m_data, other.m_data, epsilon);
        }

        /*
        * @brief Compares two Vector3 instances for inequality.
        *
        * @param other The vector to compare with.
        * @result true if any component differs, false otherwise.
        */
        [[nodiscard]] NU_FORCEINLINE NuBool operator!=(Vector3 other) const noexcept
        {
            return !(*this == other);
        }

        /*
        * @brief Returns a negated copy of the vector.
        *
        * Each component of the resulting vector is the negation of the corresponding component in this vector.
        *
        * @return A negated vector.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator-() const noexcept
        {
            return Vector3(VectorAPI::Neg(m_data));
        }

        /*
        * @brief Provides read-only access to vector components by index.
        *
        * @param index Index of the component (0 = x, 1 = y, 2 = z).
        * @return The value of the component at the specified index.
        * @throws Assertion failure if index is out of bounds.
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat operator[](NuInt32 index) const
        {
            NuEngine::Core::Types::NuAssert(index >= 0 && index < 3);
            return m_components[index];
        }

        /*
        * @brief Provides read-write access to vector components by index.
        *
        * @param index Index of the component (0 = x, 1 = y, 2 = z).
        * @return Reference to the component at the specified index.
        * @throws Assertion failure if index is out of bounds.
        */
        NU_FORCEINLINE NuFloat& operator[](NuInt32 index)
        {
            NuEngine::Core::Types::NuAssert(index >= 0 && index < 3);
            return m_components[index];
        }

        /*
        * @brief Copy assignment operator.
        *
        * @param other The vector to copy from.
        * @return Reference to this vector.
        */
        NU_FORCEINLINE Vector3& operator=(const Vector3& other) noexcept
        {
            if (this != &other)
            {
                m_data = other.m_data;
            }
            return *this;
        }

        /*
        * @brief Move assignment operator.
        *
        * @param other The vector to move from.
        * @return Reference to this vector.
        */
        NU_FORCEINLINE Vector3& operator=(Vector3&& other) noexcept
        {
            if (this != &other)
            {
                m_data = std::move(other.m_data);
            }
            return *this;
        }

        /*
        * @brief Returns a zero vector.
        *
        * The resulting vector has components (0, 0, 0).
        *
        * @return A vector with all components set to 0.
        */
        [[nodiscard]] static NU_FORCEINLINE Vector3 Zero() noexcept
        {
            return Vector3(VectorAPI::SetZero());
        }

        /*
        * @brief Returns a vector with all components set to 1.
        *
        * The resulting vector has components (1, 1, 1).
        *
        * @return A vector with all components set to 1.
        */
        [[nodiscard]] static NU_FORCEINLINE Vector3 One() noexcept
        {
            return Vector3(VectorAPI::Set(1.0f, 1.0f, 1.0f));
        }

        /*
        * @brief Returns a unit vector pointing in the X direction.
        *
        * The resulting vector has components (1, 0, 0).
        *
        * @return A unit vector with only the X component set to 1.
        */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitX() noexcept
        {
            return Vector3(VectorAPI::Set(1.0f, 0.0f, 0.0f));
        }

        /*
        * @brief Returns a unit vector pointing in the Y direction.
        *
        * The resulting vector has components (0, 1, 0).
        *
        * @return A unit vector with only the Y component set to 1.
        */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitY() noexcept
        {
            return Vector3(VectorAPI::Set(0.0f, 1.0f, 0.0f));
        }

        /*
        * @brief Returns a unit vector pointing in the Z direction.
        *
        * The resulting vector has components (0, 0, 1).
        *
        * @return A unit vector with only the Z component set to 1.
        */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitZ() noexcept
        {
            return Vector3(VectorAPI::Set(0.0f, 0.0f, 1.0f));
        }

        /*
        * @brief Calculates the length (magnitude) of the vector.
        *
        * @return Length of the vector.
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat Length() const noexcept
        {
            return VectorAPI::Length3(m_data);
        }

        /*
        * @brief Calculates the squared length of the vector.
        * 
        * @return Squared length.
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat LengthSquared() const noexcept
        {
            return VectorAPI::Dot3(m_data, m_data);
        }

        /*
        * @brief Computes the dot product with another vector.
        * 
        * @param other The vector to compute the dot product with.
        * @return Dot product result.
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat Dot(Vector3 other) const noexcept
        {
            return VectorAPI::Dot3(m_data, other.m_data);
        }

        /*
        * @brief Calculates the distance to another vector.
        * 
        * @param other The vector to calculate the distance to.
        * @return Distance between the vectors.
        */
        [[nodiscard]] NU_FORCEINLINE NuFloat Distance(Vector3 other) const noexcept
        {
            return (*this - other).Length();
        }

        /*
        * @brief Provides read-only access to the raw float array of the vector.
        * 
        * @return Pointer to the float array.
        */
        [[nodiscard]] NU_FORCEINLINE const NuFloat* Data() const noexcept
        {
            return m_components;
        }

        /*
        * @brief Provides write access to the raw float array of the vector.
        * 
        * @return Pointer to the float array.
        */
        NU_FORCEINLINE NuFloat* Data() noexcept
        {
            return m_components;
        }

        /*
        * @brief Returns the underlying SIMD representation.
        * 
        * @return Reference to the SIMD vector.
        */
        [[nodiscard]] NU_FORCEINLINE const VectorAPI::NuVec4 SimdData() const noexcept
        {
            return m_data;
        }

        /*
        * @brief Normalizes the vector.
        * 
        * @return Normalized vector.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 Normalize() const noexcept
        {
            return Vector3(VectorAPI::Normalize3(m_data));
        }

        /*
        * @brief Computes the cross product of this vector with another.
        * 
        * @param other The other vector to compute the cross product with.
        * @return The resulting vector that is perpendicular to both.
        */
        [[nodiscard]] NU_FORCEINLINE Vector3 Cross(Vector3 other) const noexcept
        {
            return Vector3(VectorAPI::Cross(m_data, other.m_data));
        }

        /*
        * @brief Converts the vector to a human-readable string.
        * 
        * @return String representation of the vector.
        */
        [[nodiscard]] std::string ToString() const
        {
            std::ostringstream oss;
            oss << "(" << X() << ", " << Y() << ", " << Z() << ")";
            return oss.str();
        }

        /*
        * @brief Writes the vector to the output stream.
        * 
        * @param os Output stream.
        * @param other Vector3 to write.
        * @return Reference to the output stream.
        */
        friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
        {
            os << vec.ToString();
            return os;
        }

    private:
        /*
        * @brief Storage for vector components.
        *
        * The vector can be accessed either as a SIMD register (m_data) for optimized operations
        * or as an array of floats (m_components) for individual component access.
        * The last 1 component is unused but maintained for SIMD alignment.
        */
        union 
        {
            VectorAPI::NuVec4 m_data; // SIMD-vector
            NuFloat m_components[4]; // float-array
        };
    };
}