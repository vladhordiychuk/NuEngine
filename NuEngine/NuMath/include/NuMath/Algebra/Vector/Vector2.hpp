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
	class Vector3;

	/*
	* @brief 2D Vector with SIMD support.
	*
	* This class implements a 2-component vector with support for arithmetic operations,
	* component access, normalization, length, and distance. It supports SSE optimizations.
	*/
	class alignas(16) Vector2
	{
	public:
		/*
		* @brief Creates a zero vector (0.0f, 0.0f).
		*/
		NU_FORCEINLINE Vector2() noexcept
			: m_data(VectorAPI::SetZero())
		{
		}

		/**
		 * @brief Creates a vector with specified components.
		 *
		 * @param x Value on X.
		 * @param y Value on Y.
		 */
		NU_FORCEINLINE Vector2(float x, float y) noexcept
			: m_data(VectorAPI::Set(x, y, 0.0f, 0.0f))
		{
		}

		/**
		 * @brief Initialization with initializer_list.
		 *
		 * @param list An initializer list containing exactly 2 elements (x, y).
		 */
		NU_FORCEINLINE Vector2(std::initializer_list<float> list) noexcept
		{
			NU_MATH_ASSERT(list.size() == 2, "Vector2 initializer_list must contain exactly 2 elements!");
			auto it = list.begin();
			float x = *it++;
			float y = *it++;
			m_data = VectorAPI::Set(x, y, 0.0f, 0.0f);
		}

		/**
		 * @brief Constructs a copy of another Vector2.
		 *
		 * Initializes this vector by copying all components from the given vector.
		 *
		 * @param other The vector to copy.
		 */
		NU_FORCEINLINE Vector2(const Vector2& other) noexcept
			: m_data(other.m_data)
		{
		}

		/*
		* @brief Constructs a Vector2 by moving another one.
		*
		* Transfers the internal data from the given vector to this one.
		* The source vector is left in a valid but unspecified state.
		*
		* @param other The vector to move from.
		*/
		NU_FORCEINLINE Vector2(Vector2&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		/*
		* @brief Constructs a Vector2 from a Vector3.
		*
		* @param vec The Vector3 to convert to Vector2.
		* @return A new Vector2 that is the result of the conversion.
		*/
		explicit NU_FORCEINLINE Vector2(const Vector3& vec) noexcept;

		/*
		* @brief Constructs a Vector2 from a Vector4.
		*
		* @param vec The Vector4 to convert to Vector2.
		* @return A new Vector2 that is the result of the conversion.
		*/
		explicit NU_FORCEINLINE Vector2(const Vector4& vec) noexcept;

		/**
		 * @brief Constructs a Vector2 from a SIMD vector.
		 *
		 * Initializes the Vector2 using the given SIMD vector without performing
		 * any additional computations or conversions.
		 *
		 * @param vec The SIMD vector (NuVec4) used to initialize this Vector2.
		 */
		explicit NU_FORCEINLINE Vector2(VectorAPI::NuVec4 vec) noexcept
			: m_data(vec)
		{
		}

        /**
         * @brief Destructor.
		 *
         * Defaulted since Vector2 does not manage resources.
         */
		NU_FORCEINLINE ~Vector2() = default;

		/**
	 	 * @brief Adding another vector to this one.
	 	 *
	 	 * Performs component-wise addition: each component of another vector is added to the corresponding component of this vector.
		 *
		 * @param other The vector to add.
		 *
		 * @return A new Vector2 that is the result of the addition.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator+(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Add(m_data, other.m_data));
		}

		/**
	 	 * @brief Subtracting another vector from this one.
		 *
		 * Performs component-wise subtraction: each component of another vector is subtracted to the corresponding component of this vector.
		 *
		 * @param other The vector to subtract.
		 *
		 * @return A new Vector2 that is the result of the subtraction.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator-(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Sub(m_data, other.m_data));
		}

		/**
		 * @brief Multiplying this vector by another.
	 	 *
		 * Performs component-wise multiplication: each component of the other vector
		 * is multiplied by the corresponding component of this vector.
		 *
		 * @param other The vector to multiply with.
		 *
		 * @return A new Vector2 that is the result of the multiplication.
		*/
		[[nodiscard]] NU_FORCEINLINE Vector2 operator*(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Mul(m_data, other.m_data));
		}

		/**
		 * @brief Divides this vector by another component-wise.
		 *
		 * Performs component-wise division of this vector by the other vector.
		 *
		 * @param other The vector to divide by.
		 *
		 * @return A new Vector2 that is the result of the division.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator/(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Div(m_data, other.m_data));
		}

		/**
		 * @brief Adds a scalar to each component of the vector.
		 *
		 * @param scalar The scalar value to add.
		 * @return A new Vector2 with the scalar added to each component.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator+(float scalar) const noexcept
		{
			return Vector2(VectorAPI::Add(m_data, VectorAPI::Set(scalar, scalar, 0.0f, 0.0f)));
		}

		/**
		 * @brief Subtracts a scalar from each component of the vector.
		 *
		 * @param scalar The scalar value to subtract.
		 * @return A new Vector2 with the scalar subtracted from each component.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator-(float scalar) const noexcept
		{
			return Vector2(VectorAPI::Sub(m_data, VectorAPI::Set(scalar, scalar, 0.0f, 0.0f)));
		}

		/**
		 * @brief Multiplies the vector by a scalar.
		 *
		 * Scales each component of the vector by the given scalar value.
		 *
		 * @param scalar The scalar value to multiply by.
		 *
		  @return A new Vector2 that is the result of the multiplication.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator*(float scalar) const noexcept
		{
			return Vector2(VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar)));
		}

		/**
		 * @brief Divides the vector by a scalar.
		 *
		 * Scales each component of the vector by the reciprocal of the given scalar value.
		 *
		 * @param scalar The scalar value to divide by.
		 *
		 * @return A new Vector2 that is the result of the division.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 operator/(float scalar) const noexcept
		{
			NU_MATH_ASSERT(std::fabs(scalar) > std::numeric_limits<float>::epsilon(), "Vector2 division by zero or near zero!");
			return Vector2(VectorAPI::Div(m_data, VectorAPI::SetAll(scalar)));
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
		NU_FORCEINLINE Vector2& operator+=(const Vector2& other) noexcept
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
		NU_FORCEINLINE Vector2& operator-=(const Vector2& other) noexcept
		{
			m_data = VectorAPI::Sub(m_data, other.m_data);
			return *this;
		}

		/**
	 	 * @brief Multiplies another vector to this one.
		 
		 * Performs component-wise multiplication and updates the current vector.
		 *
		 * @param other The vector to multiply.
		 *
		 * @return A reference to this vector after the multiplication.
		 */
		NU_FORCEINLINE Vector2& operator*=(const Vector2& other) noexcept
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
		 * @return A new Vector2 that is the result of the division.
		 */
		NU_FORCEINLINE Vector2& operator/=(const Vector2& other) noexcept
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
		NU_FORCEINLINE Vector2& operator+=(float scalar) noexcept
		{
			m_data = VectorAPI::Add(m_data, VectorAPI::Set(scalar, scalar, 0.0f, 0.0f));
			return *this;
		}

		/**
		 * @brief Subtracts a scalar from each component of this vector.
		 *
		 * @param scalar The scalar value to subtract.
		 * @return A reference to this vector after the subtraction.
		 */
		NU_FORCEINLINE Vector2& operator-=(float scalar) noexcept
		{
			m_data = VectorAPI::Sub(m_data, VectorAPI::Set(scalar, scalar, 0.0f, 0.0f));
			return *this;
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * Each component (x, y) will be multiplied by the given scalar.
		 *
		 * @param scalar The scalar value to multiply the vector by.
		 *
		 * @return A reference to this vector after the multiplication.
		 */
		NU_FORCEINLINE Vector2& operator*=(float scalar) noexcept
		{
			m_data = VectorAPI::Mul(m_data, VectorAPI::SetAll(scalar));
			return *this;
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * Each component (x, y) will be divided by the given scalar.
		 *
		 * @param scalar The scalar value to divide the vector by.
		 *
		 * @return A reference to this vector after the division.
		 */
		NU_FORCEINLINE Vector2& operator/=(float scalar) noexcept
		{
			NU_MATH_ASSERT(std::fabs(scalar) > std::numeric_limits<float>::epsilon(), "Vector2 division by zero or near zero!");
			m_data = VectorAPI::Div(m_data, VectorAPI::SetAll(scalar));
			return *this;
		}

		/**
		 * @brief Multiplies a scalar by a vector (commutative with vector * scalar).
		 *
		 * @param scalar The scalar value.
		 * @param vec The vector to multiply.
		 *
		 * @return A new Vector2 that is the result of the multiplication.
		 */
		friend [[nodiscard]] NU_FORCEINLINE Vector2 operator+(float scalar, const Vector2& vec) noexcept
		{
			return Vector2(VectorAPI::Add(VectorAPI::Set(scalar, scalar, 0.0f, 0.0f), vec.m_data));
		}

		/**
		 * @brief Subtracts each component of the vector from the scalar.
		 *
		 * @param scalar The scalar value.
		 * @param vec The vector to subtract from the scalar.
		 *
		 * @return A new Vector2 that is the result of the subtraction.
		 */
		friend [[nodiscard]] NU_FORCEINLINE Vector2 operator-(float scalar, const Vector2& vec) noexcept
		{
			return Vector2(VectorAPI::Sub(VectorAPI::Set(scalar, scalar, 0.0f, 0.0f), vec.m_data));
		}

		/**
		 * @brief Multiplies each component of the vector by the scalar.
		 *
		 * @param scalar The scalar value.
		 * @param vec The vector to multiply.
		 *
		 * @return A new Vector2 that is the result of the multiplication.
		 */
		friend [[nodiscard]] NU_FORCEINLINE Vector2 operator*(float scalar, const Vector2& vec) noexcept
		{
			return Vector2(VectorAPI::Mul(VectorAPI::SetAll(scalar), vec.m_data));
		}

		/**
		 * @brief Multiplies each component of the vector by the scalar.
		 *
		 * @param scalar The scalar value.
		 * @param vec The vector to multiply.
		 *
		 * @return A new Vector2 that is the result of the multiplication.
		 */
		friend [[nodiscard]] NU_FORCEINLINE Vector2 operator/(float scalar, const Vector2& vec) noexcept
		{
			return Vector2(VectorAPI::Div(VectorAPI::Set(scalar, scalar, 0.0f, 0.0f), vec.m_data));
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
		 * @brief Returns a vector with the minimum components between this vector and another.
		 *
		 * @param other The other vector to compare with.
		 *
		 * @return Vector2 A vector containing the minimum values component-wise.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 Min(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Min(m_data, other.m_data));
		}

		/**
		 * @brief Returns a vector with the maximum components between this vector and another.
		 *
		 * @param other The other vector to compare with.
		 *
		 * @return Vector2 A vector containing the maximum values component-wise.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 Max(const Vector2& other) const noexcept
		{
			return Vector2(VectorAPI::Max(m_data, other.m_data));
		}

		/**
		 * @brief Returns a vector with the absolute values of each component.
		 *
		 * @return Vector2 A vector containing the absolute values of this vector's components.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 Abs() const noexcept
		{
			return Vector2(VectorAPI::Abs(m_data));
		}

		/**
		 * @brief Compares two Vector2 instances for equality.
		 *
		 * @param other The vector to compare with.
		 *
		 * @return true if all components are equal, false otherwise.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator==(const Vector2& other) const noexcept
		{
			return VectorAPI::Equal(m_data, other.m_data);
		}

		/**
		 * @brief Checks if two Vector2 instances are nearly equal within a tolerance.
		 *
		 * @param other The vector to compare with.
		 * @param epsilon The maximum allowed difference for components.
		 *
		 * @return true if all components differ no more than epsilon, false otherwise.
		 */
		[[nodiscard]] NU_FORCEINLINE bool NearEqual(const Vector2& other, float epsilon) const noexcept
		{
			return VectorAPI::NearEqual(m_data, other.m_data, epsilon);
		}

		/**
		 * @brief Compares two Vector2 instances for inequality.
		 *
		 * @param other The vector to compare with.
		 *
		 * @result true if any component differs, false otherwise.
		 */
		[[nodiscard]] NU_FORCEINLINE bool operator!=(const Vector2& other) const noexcept
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
		[[nodiscard]] NU_FORCEINLINE Vector2 operator-() const noexcept
		{
			return Vector2(VectorAPI::Neg(m_data));
		}

		/**
		 * @brief Returns a zero vector.
		 *
		 * The resulting vector has components (0, 0).
		 *
		 * @return A vector with all components set to 0.
		 */
		[[nodiscard]] static NU_FORCEINLINE Vector2 Zero() noexcept
		{
			return Vector2(VectorAPI::SetZero());
		}

		/**
		 * @brief Returns a vector with all components set to 1.
		 *
		 * The resulting vector has components (1, 1).
		 *
		 * @return A vector with all components set to 1.
		 */
		[[nodiscard]] static NU_FORCEINLINE Vector2 One() noexcept
		{
			return Vector2(VectorAPI::Set(1.0f, 1.0f, 0.0f, 0.0f));
		}

		/**
		 * @brief Returns a unit vector pointing in the X direction.
		 *
		 * The resulting vector has components (1, 0).
		 *
		 * @return A unit vector with only the X component set to 1.
		 */
		[[nodiscard]] static NU_FORCEINLINE Vector2 UnitX() noexcept
		{
			return Vector2(VectorAPI::Set(1.0f, 0.0f, 0.0f, 0.0f));
		}

		/** 
		 * @brief Returns a unit vector pointing in the Y direction.
		 *
		 * The resulting vector has components (0, 1).
		 *
		 * @return A unit vector with only the Y component set to 1.
		 */
		[[nodiscard]] static NU_FORCEINLINE Vector2 UnitY() noexcept
		{
			return Vector2(VectorAPI::Set(0.0f, 1.0f, 0.0f, 0.0f));
		}

		/**
		 * @brief Provides read-only access to vector components by index.
		 *
		 * @param index Index of the component (0 = x, 1 = y).
         * @return The value of the component at the specified index.
         * @throws Assertion failure if index is out of bounds.
         */
        [[nodiscard]] NU_FORCEINLINE float operator[](int index) const noexcept
        {
            NU_MATH_ASSERT(index >= 0 && index < 2, "Vector2 index out of bounds! Valid range: 0..1");
			return reinterpret_cast<const float*>(&m_data)[index];
        }

		/**
		 * @brief Copy assignment operator.
		 *
		 * @param other The vector to copy from.
		 *
		 * @return Reference to this vector.
		 */
		NU_FORCEINLINE Vector2& operator=(const Vector2& other) noexcept
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
		NU_FORCEINLINE Vector2& operator=(Vector2&& other) noexcept
		{
			if (this != &other)
			{
				m_data = std::move(other.m_data);
			}
			return *this;
		}

		/**
		 * @brief Calculates the length (magnitude) of the vector.
		 *
		 * @return Length of the vector.
		 */
		[[nodiscard]] NU_FORCEINLINE float Length() const noexcept
		{
			return VectorAPI::Length2(m_data);
		}

		/**
		 * @brief Calculates the squared length of the vector.
		 *
		 * @return Squared length.
		 */
		[[nodiscard]] NU_FORCEINLINE float LengthSquared() const noexcept
		{
			return VectorAPI::Dot2(m_data, m_data);
		}

		/**
		 * @brief Computes the dot product with another vector.
		 *
		 * @param other The vector to compute the dot product with.
		 * @return Dot product result.
		 */
		[[nodiscard]] NU_FORCEINLINE float Dot(const Vector2& other) const noexcept
		{
			return VectorAPI::Dot2(m_data, other.m_data);
		}

		/**
		 * @brief Calculates the distance to another vector.
		 *
		 * @param other The vector to calculate the distance to.
		 * @return Distance between the vectors.
		 */
		[[nodiscard]] NU_FORCEINLINE float Distance(const Vector2& other) const noexcept
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
		 * @brief Normalizes the vector using high precision (Newton-Raphson refinement).
		 *
		 * @note Use this for:
		 * - 2D Physics engines (Box2D style solvers).
		 * - Precise steering behaviors (boids, pathfollowing).
		 * - Texture coordinates generation requiring precision.
		 *
		 * @return Normalized vector.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 Normalize() const noexcept
		{
			return Vector2(VectorAPI::Normalize2(m_data));
		}

		/**
		 * @brief Quickly normalizes the vector using a rough approximation.
		 *
		 * @note Use this for:
		 * - UI directions/animations.
		 * - 2D Particle effects (sparks, smoke).
		 * - Joystick/Gamepad input normalization (deadzone handling usually masks the error).
		 *
		 * @return Approximate normalized vector.
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 FastNormalize() const noexcept
		{
			return Vector2(VectorAPI::FastNormalize2(m_data));
		}

		/**
         * @brief Performs linear interpolation towards another 2D vector.
         *
         * Interpolates between this and other using parameter t (t=0 returns this, t=1 returns other).
         *
         * @param other Target vector.
         * @param t Interpolation parameter (typically in [0, 1]).
         *
         * @return Interpolated vector.
         */
		[[nodiscard]] NU_FORCEINLINE static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) noexcept
		{
			return Vector2(VectorAPI::Lerp(a.SimdData(), b.SimdData(), t));
		}

		/**
		 * @brief Returns a Vector2 swizzle (x, x).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 XX() const noexcept
		{
			return Vector2(VectorAPI::Shuffle<0, 0, 3, 3>(m_data));
		}

		/**
		 * @brief Returns a Vector2 swizzle (y, y).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 YY() const noexcept
		{
			return Vector2(VectorAPI::Shuffle<1, 1, 3, 3>(m_data));
		}

		/**
		 * @brief Returns a Vector2 swizzle (y, x).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector2 YX() const noexcept
		{
			return Vector2(VectorAPI::Shuffle<1, 0, 3, 3>(m_data));
		}

		/**
		 * @brief Returns a Vector4 swizzle (x, x, 0.0, 0.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 XXXX() const noexcept;

		/**
		 * @brief Returns a Vector4 swizzle (y, y, 0.0, 0.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 YYYY() const noexcept;

		/**
		 * @brief Returns a Vector4 swizzle (x, y, 0.0, 0.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 XY00() const noexcept;

		/**
		 * @brief Returns a Vector4 swizzle (x, y, 0.0, 1.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector4 XY01() const noexcept;

		/**
		 * @brief Returns a Vector3 swizzle (x, y, 0.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 XY0() const noexcept;

		/**
		 * @brief Returns a Vector3 swizzle (x, y, 1.0).
		 */
		[[nodiscard]] NU_FORCEINLINE Vector3 XY1() const noexcept;

		/**
		 * @brief Converts the vector to a human-readable string.
		 *
		 * @return String representation of the vector.
		 */
		[[nodiscard]] NU_FORCEINLINE std::string ToString() const
		{
			return std::format("({}, {})", X(), Y());
		}

		/**
		 * @brief Writes the vector to the output stream.
		 *
		 * @param os Output stream.
		 * @param vec Vector2 to write.
		 *
		 * @return Reference to the output stream.
		 */
		friend std::ostream& operator<<(std::ostream& os, const Vector2& vec)
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
#include <NuMath/Algebra/Vector/Vector3.hpp>

namespace NuMath
{
	NU_FORCEINLINE Vector2::Vector2(const Vector3& vec) noexcept
		: m_data(VectorAPI::SetZ(vec.SimdData(), 0.0f))
	{
	}

	NU_FORCEINLINE Vector2::Vector2(const Vector4& vec) noexcept
		: m_data(VectorAPI::SetZ(VectorAPI::SetW(vec.SimdData(), 0.0f), 0.0f))
	{
	}

	[[nodiscard]] NU_FORCEINLINE Vector4 Vector2::XXXX() const noexcept
	{
		return Vector4(VectorAPI::Shuffle<0, 0, 0, 0>(m_data));
	}

	[[nodiscard]] NU_FORCEINLINE Vector4 Vector2::YYYY() const noexcept
	{
		return Vector4(VectorAPI::Shuffle<1, 1, 1, 1>(m_data));
	}

	[[nodiscard]] NU_FORCEINLINE Vector4 Vector2::XY00() const noexcept
	{
		return Vector4(m_data);
	}

	[[nodiscard]] NU_FORCEINLINE Vector4 Vector2::XY01() const noexcept
	{
		return Vector4(VectorAPI::SetW(m_data, 1.0f));
	}

	[[nodiscard]] NU_FORCEINLINE Vector3 Vector2::XY0() const noexcept
	{
		return Vector3(m_data);
	}

	[[nodiscard]] NU_FORCEINLINE Vector3 Vector2::XY1() const noexcept
	{
		return Vector3(VectorAPI::SetZ(m_data, 1.0f));
	}
} // namespace NuMath