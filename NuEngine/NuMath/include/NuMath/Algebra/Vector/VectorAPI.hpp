// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Detail/SIMD/SimdBackend.hpp>

namespace NuMath::VectorAPI
{
	using Impl = Simd::MathBackend;

	// =============================================
	// Type definitions
	// =============================================

	using NuVec4 = Impl::NuVec4;

	// =============================================
	// Load and Store
	// =============================================
	
	/**
	 * @brief Load a 4-component vector from a storage location.
	 *
	 * @param vec The storage location to load the vector from.
	 *
	 * @return The loaded vector.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Load(const NuVecStorage4& vec) noexcept
	{
		return Impl::Load(vec);
	}

	/**
	 * @brief Store a 4-component vector to a storage location.
	 *
	 * @param vec The storage location to store the vector to.
	 * @param value The vector to store.
	 */
	NU_FORCEINLINE void Store(NuVecStorage4& dst, NuVec4 val) noexcept
	{
		Impl::Store(dst, val);
	}

	/**
	 * @brief Stream store a 4-component vector to a storage location.
	 *
	 * @param vec The storage location to store the vector to.
	 * @param value The vector to store.
	 */
	NU_FORCEINLINE void Stream(NuVecStorage4* dst, NuVec4 val) noexcept
	{
		Impl::Stream(reinterpret_cast<float*>(dst), val);
	}

	// =============================================
	// Setters
	// =============================================

	/**
	 * @brief Create a 4-component vector with specified values.
	 *
	 * Initializes a NuVec4 with the given component values.
	 * The w component is optional and defaults to 0.0f.
	 *
	 * @param x The x-component value.
	 * @param y The y-component value.
	 * @param z The z-component value.
	 * @param w The w-component value.
	 *
	 * @return New NuVec4 with the specified component values.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(float x, float y, float z = 0.0f, float w = 0.0f) noexcept
	{
		return Impl::Set(x, y, z, w);
	}

	/**
	 * @brief Create a 4-component vector with identical values.
	 *
	 * Initializes all components (x, y, z, w) to the same scalar value.
	 *
	 * @param scalar Value to set for all components.
	 *
	 * @return New NuVec4 with all components equal to scalar.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetAll(float scalar) noexcept
	{
		return Impl::SetAll(scalar);
	}

	/**
	 * @brief Create a 4-component vector with zero values.
	 *
	 * Initializes all components (x, y, z, w) to zero.
	 *
	 * @return New NuVec4 with all components equal to zero.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZero() noexcept
	{
		return Impl::SetZero();
	}

	/**
	 * @brief Set the X component of 4D vector.
	 *
	 * @param v Input vector.
	 * @param x Value to set for X component.
	 *
	 * @return New NuVec4 with the specified X component value.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetX(NuVec4 v, float x) noexcept
	{
		return Impl::SetX(v, x);
	}

	/**
	 * @brief Set the Y component of 4D vector.
	 *
	 * @param v Input vector.
	 * @param y Value to set for Y component.
	 *
	 * @return New NuVec4 with the specified Y component value.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetY(NuVec4 v, float y) noexcept
	{
		return Impl::SetY(v, y);
	}

	/**
	 * @brief Set the Z component of 4D vector.
	 *
	 * @param v Input vector.
	 * @param z Value to set for Z component.
	 *
	 * @return New NuVec4 with the specified Z component value.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZ(NuVec4 v, float z) noexcept
	{
		return Impl::SetZ(v, z);
	}

	/**
	 * @brief Set the W component of 4D vector.
	 *
	 * @param v Input vector.
	 * @param w Value to set for W component.
	 *
	 * @return New NuVec4 with the specified W component value.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetW(NuVec4 v, float w) noexcept
	{
		return Impl::SetW(v, w);
	}

	// =============================================
	// Getters
	// =============================================

	/**
	 * @brief Get the X component of 4D vector.
	 *
	 * Extracts and returns the first component (X) of the vector.
	 *
	 * @param v Input vector.
	 *
	 * @return float Value of X component.
	 */
	[[nodiscard]] static NU_FORCEINLINE float GetX(NuVec4 v) noexcept
	{
		return Impl::GetX(v);
	}

	/**
	 * @brief Get the Y component of 4D vector.
	 *
	 * Extracts and returns the second component (Y) of the vector.
	 *
	 * @param v Input vector.
	 *
	 * @return float Value of Y component.
	 */
	[[nodiscard]] NU_FORCEINLINE float GetY(NuVec4 v) noexcept
	{
		return Impl::GetY(v);
	}

	/**
	 * @brief Get the Z component of 4D vector.
	 *
	 * Extracts and returns the third component (Z) of the vector.
	 *
	 * @param v Input vector.
	 *
	 * @return float Value of Z component.
	 */
	[[nodiscard]] static NU_FORCEINLINE float GetZ(NuVec4 v) noexcept
	{
		return Impl::GetZ(v);
	}

	/**
	 * @brief Get the W component of 4D vector.
	 *
	 * Extracts and returns the fourth component (W) of the vector.
	 *
	 * @param v Input vector.
	 *
	 * @return float Value of W component.
	 */
	[[nodiscard]] static NU_FORCEINLINE float GetW(NuVec4 v) noexcept
	{
		return Impl::GetW(v);
	}

	// =============================================
	// Basic Arithmetic
	// =============================================

	/**
	* @brief Component-wise addition of two 4D vectors.
	*
	* Performs component-wise addition.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is a[i] + b[i].
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Add(NuVec4 a, NuVec4 b) noexcept 
	{
		return Impl::Add(a, b);
	}

	/**
	 * @brief Component-wise subtraction of two 4D vectors.
	 *
	 * Performs component-wise subtraction.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return New NuVec4 where each component is a[i] - b[i].
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Sub(NuVec4 a, NuVec4 b) noexcept 
	{
		return Impl::Sub(a, b);
	}

	/**
	 * @brief Component-wise multiplication of two 4D vectors.
	 *
	 * Performs component-wise multiplication.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return New NuVec4 where each component a[i] * b[i].
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(NuVec4 a, NuVec4 b) noexcept 
	{
		return Impl::Mul(a, b);
	}

	/**
	 * @brief Component-wise division of two 4D vectors.
	 *
	 * Performs component-wise division.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return New NuVec4 where each component a[i] / b[i].
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Div(NuVec4 a, NuVec4 b) noexcept {
		return Impl::Div(a, b);
	}

	/**
	 * @brief Negates all components of a 4D vector.
	 *
	 * Computes component-wise negation: [-x, -y, -z, -w].
	 *
	 * @param a Input vector.
	 *
	 * @return New NuVec4 with negated components.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Neg(NuVec4 v) noexcept
	{
		return Impl::Neg(v);
	}

	/**
	 * @brief Component-wise minimum of two vectors.
	 *
	 * Computes a new vector where each component is the minimum
	 * of corresponding components from input vectors.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return New NuVec4 where each component is min(a[i], b[i]).
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Min(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Min(a, b);
	}

	/**
	 * @brief Component-wise maximum of two vectors.
	 *
	 * Computes a new vector where each component is the maximum
	 * of corresponding components from input vectors.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return New NuVec4 where each component is max(a[i], b[i]).
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Max(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Max(a, b);
	}

	/**
	 * @brief Component-wise absolute value.
	 *
	 * Returns vector with |x|, |y|, |z|, |w| components
	 *
	 * @param v Input vector.
	 *
	 * @return New NuVec4 with absolute values.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Abs(NuVec4 v) noexcept
	{
		return Impl::Abs(v);
	}

	/**
	 * @brief Exact floating-point vector equality comparison.
	 *
	 * Performs component-wise == comparison. For approximate equality,
	 * consider using a threshold-based comparison instead.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return true if all components are exactly equal.
	 * @warning Floating-point imprecision may affect results.
	 */
	[[nodiscard]] NU_FORCEINLINE bool Equal(NuVec4 a, NuVec4 b)noexcept
	{
		return Impl::Equal(a, b);
	}

	/**
	 * @brief Approximate floating-point vector equality comparison with epsilon.
	 *
	 * Performs component-wise comparison with given epsilon tolerance.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 * @param epsilon The tolerance value.
	 *
	 * @return true if all components differ by no more than epsilon.
	 */
	[[nodiscard]] NU_FORCEINLINE bool NearEqual(NuVec4 a, NuVec4 b, float epsilon) noexcept
	{
		return Impl::NearEqual(a, b, epsilon);
	}

	/**
	 * @brief Computes the square root of a scalar floating-point value.
	 *
	 * Calculates the square root.
	 *
	 * @param scalar The input value.
	 *
	 * @return The square root of the input value.
	 * @warning Scalar value must be >= 0 for valid result (Input values < 0 will return NaN).
	 */
	[[nodiscard]] NU_FORCEINLINE float SqrtScalar(float value) noexcept
	{
		return Impl::SqrtScalar(value);
	}

	// =============================================
	// Special Ops
	// =============================================

	/**
	 * @brief Computes the horizontal sum of the 4D vector.
	 *
	 * Efficiently calculates the sum x + y + z + w of a 4D vector.
	 *
	 * @param v Input vector.
	 *
	 * @return Sum of x, y, z and w components.
	 */
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd4(NuVec4 v) noexcept
	{
		return Impl::HorizontalAdd4(v);
	}

	/**
	 * @brief Computes the horizontal sum of the first three components of a 4D vector.
	 *
	 * Efficiently calculates the sum x + y + z of a 4D vector (ignores w component).
	 *
	 * @param v Input vector.
	 *
	 * @return Sum of x, y, and z components.
	 */
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd3(NuVec4 v) noexcept
	{
		return Impl::HorizontalAdd3(v);
	}

	/**
	 * @brief Computes the horizontal sum of the first two components of a 4D vector.
	 *
	 * Efficiently calculates the sum x + y of a 4D vector (ignores z and w components).
	 *
	 * @param v Input vector.
	 *
	 * @return Sum of x and y components.
	 */
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd2(NuVec4 v) noexcept
	{
		return Impl::HorizontalAdd2(v);
	}

	/**
	 * @brief Normalizes first 2 components (x,y) of 4D vector.
	 *
	 * Computes a unit vector from x/y components while leaving z/w unchanged.
	 *
	 * @param v Input vector.
	 *
	 * @return Normalized vector with original z/w.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize2(NuVec4 v) noexcept
	{
		return Impl::Normalize2(v);
	}

	/**
	 * @brief Normalizes first 3 components (x,y,z) of 4D vector.
	 *
	 * Computes unit vector from x,y,z components while preserving w.
	 *
	 * @param v Input vector.
	 *
	 * @return Normalized vector with original w component.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize3(NuVec4 v) noexcept
	{
		return Impl::Normalize3(v);
	}

	/**
	 * @brief Normalizes full 4D vector.
	 *
	 * Computes unit vector from all components (x,y,z,w).
	 *
	 * @param v Input vector.
	 *
	 * @return Fully normalized vector.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize4(NuVec4 v) noexcept
	{
		return Impl::Normalize4(v);
	}

	// =============================================
	// Extra Math Utilities
	// =============================================

	/**
	 * @brief Computes 3D cross product of two vectors (ignores w components).
	 *
	 * Calculates the cross product for the first three components (x,y,z), setting w component to 0.
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return Cross product vector {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0}.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Cross(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Cross(a, b);
	}

	/**
	 * @brief Computes 2D dot product (ignores z and w components).
	 *
	 * Calculates the dot product of the first two components (x,y).
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return Dot product (a.x*b.x + a.y*b.y).
	 */
	[[nodiscard]] NU_FORCEINLINE float Dot2(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Dot2(a, b);
	}

	/**
	 * @brief Computes 3D dot product (ignores w component).
	 *
	 * Calculates the dot product of the first three components (x,y,z).
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z).
	 */
	[[nodiscard]] NU_FORCEINLINE float Dot3(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Dot3(a, b);
	}

	/**
	 * @brief Computes 4D dot product.
	 *
	 * Calculates the dot product of all four components (x,y,z,w).
	 *
	 * @param a First vector.
	 * @param b Second vector.
	 *
	 * @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w).
	 */
	[[nodiscard]] NU_FORCEINLINE float Dot4(NuVec4 a, NuVec4 b) noexcept
	{
		return Impl::Dot4(a, b);
	}

	/**
	 * @brief Computes 2D vector length (ignores z and w components).
	 *
	 * Calculates the Euclidean length of the first two components (x,y).
	 *
	 * @param v Input vector.
	 *
	 * @return Length of vector (sqrt(x² + y²)).
	 */
	[[nodiscard]] NU_FORCEINLINE float Length2(NuVec4 v) noexcept
	{
		return Impl::Length2(v);
	}

	/**
	 * @brief Computes 3D vector length (ignores w component).
	 *
	 * Calculates the Euclidean length of the first three components (x,y,z).
	 *
	 * @param v Input vector.
	 *
	 * @return Length of vector (sqrt(x² + y² + z²)).
	 */
	[[nodiscard]] NU_FORCEINLINE float Length3(NuVec4 v) noexcept
	{
		return Impl::Length3(v);
	}

	/**
	 * @brief Computes 4D vector length.
	 *
	 * Calculates the Euclidean length of all four components (x,y,z,w).
	 *
	 * @param v Input vector.
	 *
	 * @return Length of vector (sqrt(x² + y² + z² + w²)).
	 */
	[[nodiscard]] NU_FORCEINLINE float Length4(NuVec4 v) noexcept
	{
		return Impl::Length4(v);
	}

	/**
	 * @brief Performs component-wise linear interpolation between two vectors.
	 *
	 * Computes the interpolation: a + (b - a) * t
	 *
	 * @param a The starting vector (result when t = 0).
	 * @param b The ending vector (result when t = 1).
	 * @param t A vector of interpolation factors, one for each component.
	 *
	 * @return The interpolated vector.
	 */
	[[nodiscard]] NU_FORCEINLINE NuVec4 Lerp(NuVec4 a, NuVec4 b, float t) noexcept
	{
		return Impl::Lerp(a, b, t);
	}

	/**
	 * @brief Swizzle/shuffles components of a vector.
	 *
	 * Creates a new vector by arbitrary selection and reordering of
	 * components from the input vector.
	 *
	 * @tparam I0 Index for the new X component (0-3).
	 * @tparam I1 Index for the new Y component (0-3).
	 * @tparam I2 Index for the new Z component (0-3).
	 * @tparam I3 Index for the new W component (0-3).
	 * @param v The vector to shuffle.
	 * 
	 * @return A new NuVec4 composed of { v[I0], v[I1], v[I2], v[I3] }.
	 */
	template <int I0, int I1, int I2, int I3>
	[[nodiscard]] NU_FORCEINLINE NuVec4 Shuffle(NuVec4 v) noexcept
	{
		return Impl::Shuffle<I0, I1, I2, I3>(v);
	}
} // namespace NuMath::VectorAPI