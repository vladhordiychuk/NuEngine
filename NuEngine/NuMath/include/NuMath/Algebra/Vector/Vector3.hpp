// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Algebra/Vector/VectorAPI.hpp>
#include <NuMath/Core/Common.hpp>

#include <string>
#include <initializer_list>
#include <format>
#include <cmath>
#include <limits>
#include <ostream>

namespace NuMath
{
    class Vector4;
    class Vector2;

    /**
     * @brief 3D Vector with SIMD support.
     *
     * This class implements a 3-component vector with support for arithmetic operations,
     * component access, normalization, length, and distance. It supports SSE optimizations.
     */
    class alignas(16) Vector3
    {
    public:
        /**
         * @brief Creates a zero vector (0.0f, 0.0f, 0.0f).
         */
        NU_FORCEINLINE Vector3() noexcept
            : m_data(VectorAPI::SetZero())
        {
        }

        /**
         * @brief Creates a vector with specified components.
         * 
         * @param x Value on X.
         * @param y Value on Y.
         * @param z Value on Z.
         */
        NU_FORCEINLINE Vector3(float x, float y, float z) noexcept
            : m_data(VectorAPI::Set(x, y, z, 0.0f))
        {
        }
        
        /**
         * @brief Initialization with initializer_list.
         * 
         * @param Initialization list of 3 elements.
         */
        NU_FORCEINLINE Vector3(std::initializer_list<float> list) noexcept
        {
            NU_MATH_ASSERT(list.size() == 3, "Vector3 initializer_list must contain exactly 3 elements!");
            auto it = list.begin();
            float x = *it++;
            float y = *it++;
            float z = *it++;
            m_data = VectorAPI::Set(x, y, z, 0.0f);
        }

        /**
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

        /**
         * @brief Constructs a Vector3 by moving another one.
         *
         * Transfers the internal data from the given vector to this one.
         * The source vector is left in a valid but unspecified state.
         *
         * @param other The vector to move from.
         */
        NU_FORCEINLINE Vector3(Vector3&& other) noexcept
            : m_data(std::move(other.m_data))
        {
        }

        /**
         * @brief Constructs a Vector3 from a Vector4.
         *
         * @param vec The Vector4 to convert to Vector3.
         * @return A new Vector3 that is the result of the conversion.
         */
        explicit NU_FORCEINLINE Vector3(const Vector4& vec) noexcept;

        /**
         * @brief Constructs a Vector3 from a Vector2 with custom Z and W components.
         *
         * @param vec The Vector2 to convert to Vector3.
         * @param z The Z component to set.
         * @return A new Vector3 that is the result of the conversion.
         */
        explicit NU_FORCEINLINE Vector3(const Vector2& vec, float z = 0.0f) noexcept;

        /**
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

        /**
         * @brief Destructor.
         *
         * Defaulted since Vector3 does not manage resources.
         */
        NU_FORCEINLINE ~Vector3() = default;

        /**
         * @brief Adding another vector to this one.
         *
         * Performs component-wise addition: each component of another vector is added to the corresponding component of this vector.
         *
         * @param other The vector to add.
         *
         * @return A new Vector3 that is the result of the addition.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator+(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Add(m_data, other.m_data));
        }

        /**
         * @brief Subtracting another vector from this one.
         *
         * Performs component-wise subtraction: each component of another vector is subtracted to the corresponding component of this vector.
         *
         * @param other The vector to subtract.
         *
         * @return A new Vector3 that is the result of the subtraction.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator-(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Sub(m_data, other.m_data));
        }

        /**
         * @brief Multiplying this vector by another.
         *
         * Performs component-wise multiplication: each component of the other vector
         * is multiplied by the corresponding component of this vector.
         *
         * @param other The vector to multiply with.
         *
         * @return A new Vector3 that is the result of the multiplication.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator*(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Mul(m_data, other.m_data));
        }

		/**
		 * @brief Divides this vector by another component-wise.
		 *
		 * Performs component-wise division of this vector by the other vector.
		 *
		 * @param other The vector to divide by.
		 *
		 * @return A new Vector3 that is the result of the division.
		 */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator/(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Div(m_data, other.m_data));
        }

        /**
         * @brief Adds a scalar to each component of the vector.
         *
         * @param scalar The scalar value to add.
         * @return A new Vector3 with the scalar added to each component.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator+(float scalar) const noexcept
        {
            return Vector3(VectorAPI::Add(m_data, VectorAPI::Set(scalar, scalar, scalar, 0.0f)));
        }

        /**
         * @brief Subtracts a scalar from each component of the vector.
         *
         * @param scalar The scalar value to subtract.
         * @return A new Vector3 with the scalar subtracted from each component.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator-(float scalar) const noexcept
        {
            return Vector3(VectorAPI::Sub(m_data, VectorAPI::Set(scalar, scalar, scalar, 0.0f)));
        }

        /**
         * @brief Multiplies the vector by a scalar.
         *
         * Scales each component of the vector by the given scalar value.
         *
         * @param scalar The scalar value to multiply by.
         *
         * @return A new Vector3 that is the result of the multiplication.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator*(float scalar) const noexcept
        {
            return Vector3(VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar)));
        }

        /**
         * @brief Divides the vector by a scalar.
         *
         * Scales each component of the vector by the reciprocal of the given scalar value.
         *
         * @param scalar The scalar value to divide by.
         *
         * @return A new Vector3 that is the result of the division.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 operator/(float scalar) const noexcept
        {
            NU_MATH_ASSERT(std::fabs(scalar) > std::numeric_limits<float>::epsilon(), "Vector3 division by zero or near zero!");
            return Vector3(VectorAPI::Div(m_data, VectorAPI::SetAll(scalar)));
        }

        /**
         * @brief Adds another vector to this one.
         *
         * Performs component-wise addition and updates the current vector.
         *
         * @param other The vector to add.
         *
         * @return A reference to this vector after the addition.
         */
        NU_FORCEINLINE Vector3& operator+=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Add(m_data, other.m_data);
            return *this;
        }
        
        /**
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

        /**
         * @brief Multiplies another vector to this one.
         *
         * Performs component-wise multiplication and updates the current vector.
         *
         * @param other The vector to multiply.
         *
         * @return A reference to this vector after the multiplication.
         */
        NU_FORCEINLINE Vector3& operator*=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Mul(m_data, other.m_data);
            return *this;
        }

        /**
         * @brief Divides this vector by another component-wise.
         *
         * Performs component-wise division of this vector by the other vector.
         *
         * @param other The vector to divide by.
         *
         * @return A new Vector3 that is the result of the division.
         */
        NU_FORCEINLINE Vector3& operator/=(const Vector3& other) noexcept
        {
            m_data = VectorAPI::Div(m_data, other.m_data);
            return *this;
        }

        /**
         * @brief Adds a scalar to each component of this vector.
         *
         * @param scalar The scalar value to add.
         * @return A reference to this vector after the addition.
         */
        NU_FORCEINLINE Vector3& operator+=(float scalar) noexcept
        {
            m_data = VectorAPI::Add(m_data, VectorAPI::Set(scalar, scalar, scalar, 0.0f));
            return *this;
        }

        /**
         * @brief Subtracts a scalar from each component of this vector.
         *
         * @param scalar The scalar value to subtract.
         * @return A reference to this vector after the subtraction.
         */
        NU_FORCEINLINE Vector3& operator-=(float scalar) noexcept
        {
            m_data = VectorAPI::Sub(m_data, VectorAPI::Set(scalar, scalar, scalar, 0.0f));
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar value.
         *
         * Each component (x, y, z) will be multiplied by the given scalar.
         *
         * @param scalar The scalar value to multiply the vector by.
         *
         * @return A reference to this vector after the multiplication.
         */
        NU_FORCEINLINE Vector3& operator*=(float scalar) noexcept
        {
            m_data = VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar));
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar value.
         *
         * Each component (x, y, z) will be divided by the given scalar.
         *
         * @param scalar The scalar value to divide the vector by.
         *
         * @return A reference to this vector after the division.
         */
        NU_FORCEINLINE Vector3& operator/=(float scalar) noexcept
        {
            NU_MATH_ASSERT(std::fabs(scalar) > std::numeric_limits<float>::epsilon(), "Vector3 division by zero or near zero!");
            m_data = VectorAPI::Div(m_data, VectorAPI::SetAll(scalar));
            return *this;
        }

        /**
         * @brief Adds a scalar to each component of the vector (commutative with vector + scalar).
         *
         * @param scalar The scalar value.
         * @param vec The vector to add.
         *
         * @return A new Vector3 that is the result of the addition.
         */
        friend [[nodiscard]] NU_FORCEINLINE Vector3 operator+(float scalar, const Vector3& vec) noexcept
        {
            return Vector3(VectorAPI::Add(VectorAPI::Set(scalar, scalar, scalar, 0.0f), vec.m_data));
        }

        /**
         * @brief Subtracts each component of the vector from the scalar.
         *
         * @param scalar The scalar value.
         * @param vec The vector to subtract from the scalar.
         *
         * @return A new Vector3 that is the result of the subtraction.
         */
        friend [[nodiscard]] NU_FORCEINLINE Vector3 operator-(float scalar, const Vector3& vec) noexcept
        {
            return Vector3(VectorAPI::Sub(VectorAPI::Set(scalar, scalar, scalar, 0.0f), vec.m_data));
        }

        /**
         * @brief Multiplies each component of the vector by the scalar.
         *
         * @param scalar The scalar value.
         * @param vec The vector to multiply.
         *
         * @return A new Vector3 that is the result of the multiplication.
         */
        friend [[nodiscard]] NU_FORCEINLINE Vector3 operator*(float scalar, const Vector3& vec) noexcept
        {
            return Vector3(VectorAPI::Mul(VectorAPI::SetAll(scalar), vec.m_data));
        }

        /**
         * @brief Divides the scalar by each component of the vector.
         *
         * @param scalar The scalar value (numerator).
         * @param vec The vector (denominator).
         *
         * @return A new Vector3 that is the result of the division.
         */
        friend [[nodiscard]] NU_FORCEINLINE Vector3 operator/(float scalar, const Vector3 & vec) noexcept
        {
            return Vector3(VectorAPI::Div(VectorAPI::Set(scalar, scalar, scalar, 0.0f), vec.m_data));
        }

        /**
         * @brief Returns the x-component
         */
        [[nodiscard]] NU_FORCEINLINE float X() const noexcept
        {
            return VectorAPI::GetX(m_data);
        }

        /**
         * @brief Returns the y-component
         */
        [[nodiscard]] NU_FORCEINLINE float Y() const noexcept
        {
            return VectorAPI::GetY(m_data);
        }

        /**
         * @brief Returns the z-component
         */
        [[nodiscard]] NU_FORCEINLINE float Z() const noexcept
        {
            return VectorAPI::GetZ(m_data);
        }

        /**
         * @brief Set the x-component
         */
        NU_FORCEINLINE void SetX(float x) noexcept
        {
            m_data = VectorAPI::SetX(m_data, x);
        }

        /**
         * @brief Set the y-component
         */
        NU_FORCEINLINE void SetY(float y) noexcept
        {
            m_data = VectorAPI::SetY(m_data, y);
        }

        /**
         * @brief Set the z-component
         */
        NU_FORCEINLINE void SetZ(float z) noexcept
        {
            m_data = VectorAPI::SetZ(m_data, z);
        }

        /**
         * @brief Returns a vector with the minimum components between this vector and another.
         *
         * @param other The other vector to compare with.
         *
         * @return Vector3 A vector containing the minimum values component-wise.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 Min(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Min(m_data, other.m_data));
        }

        /**
         * @brief Returns a vector with the maximum components between this vector and another.
         *
         * @param other The other vector to compare with.
         *
         * @return Vector3 A vector containing the maximum values component-wise.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 Max(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Max(m_data, other.m_data));
        }

        /**
         * @brief Returns a vector with the absolute values of each component.
         *
         * @return Vector3 A vector containing the absolute values of this vector's components.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 Abs() const noexcept
        {
            return Vector3(VectorAPI::Abs(m_data));
        }

        /**
         * @brief Compares two Vector3 instances for equality.
         *
         * @param other The vector to compare with.
         *
         * @return true if all components are equal, false otherwise.
         */
        [[nodiscard]] NU_FORCEINLINE bool operator==(const Vector3& other) const noexcept
        {
            return VectorAPI::Equal(m_data, other.m_data);
        }

        /**
         * @brief Checks if two Vector3 instances are nearly equal within a tolerance.
         *
         * @param other The vector to compare with.
         * @param epsilon The maximum allowed difference for components.
         *
         * @return true if all components differ no more than epsilon, false otherwise.
         */
        [[nodiscard]] NU_FORCEINLINE bool NearEqual(const Vector3& other, float epsilon) const noexcept
        {
            return VectorAPI::NearEqual(m_data, other.m_data, epsilon);
        }

        /**
         * @brief Compares two Vector3 instances for inequality.
         *
         * @param other The vector to compare with.
         *
         * @return true if any component differs, false otherwise.
         */
        [[nodiscard]] NU_FORCEINLINE bool operator!=(const Vector3& other) const noexcept
        {
            return !(*this == other);
        }

        /**
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

        /**
         * @brief Provides read-only access to vector components by index.
         *
         * @param index Index of the component (0 = x, 1 = y, 2 = z).
         *
         * @return The value of the component at the specified index.
         * @throws Assertion failure if index is out of bounds.
         */
        [[nodiscard]] NU_FORCEINLINE float operator[](int index) const noexcept
        {
            NU_MATH_ASSERT(index >= 0 && index < 3, "Vector3 index out of bounds! Valid range: 0..2");
            return reinterpret_cast<const float*>(&m_data)[index];
        }

        /**
         * @brief Copy assignment operator.
         *
         * @param other The vector to copy from.
         *
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

        /**
         * @brief Move assignment operator.
         *
         * @param other The vector to move from.
         *
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

        /**
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

        /**
         * @brief Returns a vector with all components set to 1.
         *
         * The resulting vector has components (1, 1, 1).
         *
         * @return A vector with all components set to 1.
         */
        [[nodiscard]] static NU_FORCEINLINE Vector3 One() noexcept
        {
            return Vector3(VectorAPI::Set(1.0f, 1.0f, 1.0f, 0.0f));
        }
        
        /**
         * @brief Returns a unit vector pointing in the X direction.
         *
         * The resulting vector has components (1, 0, 0).
         *
         * @return A unit vector with only the X component set to 1.
         */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitX() noexcept
        {
            return Vector3(VectorAPI::Set(1.0f, 0.0f, 0.0f, 0.0f));
        }

        /**
         * @brief Returns a unit vector pointing in the Y direction.
         *
         * The resulting vector has components (0, 1, 0).
         *
         * @return A unit vector with only the Y component set to 1.
         */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitY() noexcept
        {
            return Vector3(VectorAPI::Set(0.0f, 1.0f, 0.0f, 0.0f));
        }

        /**
         * @brief Returns a unit vector pointing in the Z direction.
         *
         * The resulting vector has components (0, 0, 1).
         *
         * @return A unit vector with only the Z component set to 1.
         */
        [[nodiscard]] static NU_FORCEINLINE Vector3 UnitZ() noexcept
        {
            return Vector3(VectorAPI::Set(0.0f, 0.0f, 1.0f, 0.0f));
        }

        /**
         * @brief Calculates the length (magnitude) of the vector.
         *
         * @return Length of the vector.
         */
        [[nodiscard]] NU_FORCEINLINE float Length() const noexcept
        {
            return VectorAPI::Length3(m_data);
        }

        /**
         * @brief Calculates the squared length of the vector.
         *
         * @return Squared length.
         */
        [[nodiscard]] NU_FORCEINLINE float LengthSquared() const noexcept
        {
            return VectorAPI::Dot3(m_data, m_data);
        }

        /**
         * @brief Computes the dot product with another vector.
         *
         * @param other The vector to compute the dot product with.
         *
         * @return Dot product result.
         */
        [[nodiscard]] NU_FORCEINLINE float Dot(const Vector3& other) const noexcept
        {
            return VectorAPI::Dot3(m_data, other.m_data);
        }

        /**
         * @brief Calculates the distance to another vector.
         *
         * @param other The vector to calculate the distance to.
         *
         * @return Distance between the vectors.
         */
        [[nodiscard]] NU_FORCEINLINE float Distance(const Vector3& other) const noexcept
        {
            return (*this - other).Length();
        }

        /**
         * @brief Provides read-only access to the raw float array of the vector.
         *
         * @return Pointer to the float array.
         */
        [[nodiscard]] NU_FORCEINLINE const float* Data() const noexcept
        {
            return reinterpret_cast<const float*>(&m_data);
        }

        /**
         * @brief Returns the underlying SIMD representation.
         *
         * @return SIMD vector value.
         */
        [[nodiscard]] NU_FORCEINLINE VectorAPI::NuVec4 SimdData() const noexcept
        {
            return m_data;
        }

        /**
         * @brief Normalizes the vector.
         *
         * @return Normalized vector.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 Normalize() const noexcept
        {
            return Vector3(VectorAPI::Normalize3(m_data));
        }

        /**
         * @brief Computes the cross product of this vector with another.
         *
         * @param other The other vector to compute the cross product with.
         *
         * @return The resulting vector that is perpendicular to both.
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 Cross(const Vector3& other) const noexcept
        {
            return Vector3(VectorAPI::Cross(m_data, other.m_data));
        }

        /**
         * @brief Performs linear interpolation between two vectors.
         *
         * @param a The starting vector (result when t = 0).
         * @param b The ending vector (result when t = 1).
         * @param t Interpolation parameter (typically in [0, 1]).
         *
         * @return Interpolated vector.
         */
        [[nodiscard]] NU_FORCEINLINE static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) noexcept
        {
            return Vector3(VectorAPI::Lerp(a.SimdData(), b.SimdData(), t));
        }

        /*
         * @brief Returns a Vector2 swizzle (x, y).
         */
        [[nodiscard]] NU_FORCEINLINE Vector2 XY() const noexcept;

        /**
         * @brief Returns a Vector4 swizzle (x, y, z, 0.0).
         */
        [[nodiscard]] NU_FORCEINLINE Vector4 XYZ0() const noexcept;

        /**
         * @brief Returns a Vector2 swizzle (x, y, z, 1.0).
         */
        [[nodiscard]] NU_FORCEINLINE Vector4 XYZ1() const noexcept;

        /**
         * @brief Returns a Vector2 swizzle (x, x, x).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 XXX() const noexcept
        {
            return Vector3(VectorAPI::Shuffle<0, 0, 0, 3>(m_data));
        }

        /**
         * @brief Returns a Vector4 swizzle (y, y, y).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 YYY() const noexcept
        {
            return Vector3(VectorAPI::Shuffle<1, 1, 1, 3>(m_data));
        }

        /**
         * @brief Returns a Vector4 swizzle (z, z, z).
         */
        [[nodiscard]] NU_FORCEINLINE Vector3 ZZZ() const noexcept
        {
            return Vector3(VectorAPI::Shuffle<2, 2, 2, 3>(m_data));
        }

        /**
         * @brief Converts the vector to a human-readable string.
         *
         * @return String representation of the vector.
         */
        [[nodiscard]] NU_FORCEINLINE std::string ToString() const
        {
            return std::format("({}, {}, {})", X(), Y(), Z());
        }

        /**
         * @brief Writes the vector to the output stream.
         *
         * @param os Output stream.
         * @param other Vector3 to write.
         *
         * @return Reference to the output stream.
        */
        friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
        {
            os << vec.ToString();
            return os;
        }

    private:
        /**
         * @brief Storage for vector components.
         *
         * The vector can be accessed either as a SIMD register (m_data) for optimized operations.
         */
        VectorAPI::NuVec4 m_data;        // SIMD register for optimized operations
    };
} // namespace NuMath


#include <NuMath/Algebra/Vector/Vector4.hpp>
#include <NuMath/Algebra/Vector/Vector2.hpp>

namespace NuMath
{
    NU_FORCEINLINE Vector3::Vector3(const Vector4& vec) noexcept
        : m_data(VectorAPI::SetW(vec.SimdData(), 0.0f))
    {
    }

    NU_FORCEINLINE Vector3::Vector3(const Vector2& vec, float z) noexcept
        : m_data(VectorAPI::SetZ(vec.SimdData(), z))
    {
    }

    [[nodiscard]] NU_FORCEINLINE Vector2 Vector3::XY() const noexcept
    {
        return Vector2(VectorAPI::SetZ(m_data, 0.0f));
    }

    [[nodiscard]] NU_FORCEINLINE Vector4 Vector3::XYZ0() const noexcept
    {
        return Vector4(m_data);
    }

    [[nodiscard]] NU_FORCEINLINE Vector4 Vector3::XYZ1() const noexcept
    {
        return Vector4(VectorAPI::SetW(m_data, 1.0f));
    }
} // namespace NuMath