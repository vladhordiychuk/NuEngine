// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#define NU_FORCEINLINE __forceinline
#else
#define NU_FORCEINLINE inline __attribute__((always_inline))
#endif

namespace NuEngine::Math::Docs::Vector
{
	// =============================================
	// Common types
	// =============================================

	/*
	* @brief 4-component vector type with 16-byte alignment.
	*
	* Represents a SIMD-friendly vector with four float components (x, y, z, w).
	* The structure is aligned to 16 bytes for optimal SIMD operations.
	*/
	struct alignas(16) NuVec4
	{
		float x, y, z, w;
	};

	// =============================================
	// Setters
	// =============================================

	/*
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
	[[nodiscard]] NU_FORCEINLINE NuVec4 Set(float x, float y, float z, float w = 0.0f) noexcept;

	/*
	* @brief Create a 4-component vector with identical values.
	*
	* Initializes all components (x, y, z, w) to the same scalar value.
	*
	* @param scalar Value to set for all components.
	*
	* @return New NuVec4 with all components equal to scalar.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetAll(float scalar) noexcept;

	/*
	* @brief Create a 4-component vector with zero values.
	*
	* Initializes all components (x, y, z, w) to the zero.
	*
	* @return New NuVec4 with all components equal to zero.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 SetZero() noexcept;

	// =============================================
	// Getters
	// =============================================

	/*
	* @brief Get the X component of 4D vector.
	*
	* Extracts and returns the first component (X) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of X component.
	*/
	[[nodiscard]] NU_FORCEINLINE float GetX(const NuVec4& v) noexcept;

	/*
	* @brief Get the Y component of 4D vector.
	*
	* Extracts and returns the second component (Y) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of Y component.
	*/
	[[nodiscard]] NU_FORCEINLINE float GetY(const NuVec4& v) noexcept;

	/*
	* @brief Get the Z component of 4D vector.
	*
	* Extracts and returns the third component (Z) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of Z component.
	*/
	[[nodiscard]] NU_FORCEINLINE float GetZ(const  NuVec4& v) noexcept;

	/*
	* @brief Get the W component of 4D vector.
	*
	* Extracts and returns the fouth component(W) of the vector.
	*
	* @param v Input vector.
	*
	* @return float Value of W component.
	*/
	[[nodiscard]] NU_FORCEINLINE float GetW(const NuVec4& v) noexcept;

	// =============================================
	// Basic Arithmetic
	// =============================================

	/*
	* @brief Component-wise addition of two 4D vectors.
	*
	* Performs scalar addition.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is a[i] + b[i].
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Add(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Component-wise subtaction of two 4D vectors.
	*
	* Perfoem scalar subtraction.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component is a[i] - b[i].
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Sub(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Component-wise multiplication of two 4D vectors.
	*
	* Perform scalar multiplication.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component a[i] * b[i].
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Mul(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Component-wise division of two 4D vectors.
	*
	* Perform scalar division.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return New NuVec4 where each component a[i] / b[i].
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Div(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Negates all components of a 4D vector.
	*
	* Computes component-wise negation: [-x, -y, -z, -w].
	*
	* @param a Input vector.
	*
	* @return New NuVec4 with negated components.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Neg(const NuVec4& a) noexcept;

	/*
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
	[[nodiscard]] NU_FORCEINLINE NuVec4 Min(const NuVec4& a, const NuVec4& b) noexcept;

	/*
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
	[[nodiscard]] NU_FORCEINLINE NuVec4 Max(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Component-wise absolute value.
	*
	* Returns vector with |x|, |y|, |z|, |w| components
	*
	* @param v Input vector.
	*
	* @return New NuVec4 with absolute values.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Abs(const NuVec4& v) noexcept;

	/*
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
	[[nodiscard]] NU_FORCEINLINE bool Equal(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Computes the square root of a scalar floating-point value.
	*
	* Calculates the square root.
	*
	* @param scalar The input value.
	*
	* @return The square root of the input value.
	* @warning Scalar value must be >= 0 for valid result (Input values < 0 will return NaN).
	*/
	[[nodiscard]] NU_FORCEINLINE float SqrtScalar(float scalar) noexcept;

	// =============================================
	// Special Ops
	// =============================================

	/*
	* @brief Computes the horizontal sum of the 4D vector.
	*
	* Efficiently calculates the sum x + y + z + w of a 4D vector.
	*
	* @param v Input vector.
	*
	* @return Sum of x, y, z and w components.
	*/
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd4(const NuVec4& v) noexcept;

	/*
	* @brief Computes the horizontal sum of the first three components of a 4D vector.
	*
	* Efficiently calculates the sum x + y + z of a 4D vector (ignores w component).
	*
	* @param v Input vector (const ref).
	*
	* @return Sum of x, y, and z components.
	*/
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd3(const NuVec4& v) noexcept;

	/*
	* @brief Computes the horizontal sum of the first two components of a 4D vector.
	*
	* Efficiently calculates the sum x + y of a 4D vector (ignores z and w components).
	*
	* @param v Input vector (const ref).
	*
	* @return Sum of x and y components.
	*/
	[[nodiscard]] NU_FORCEINLINE float HorizontalAdd2(const NuVec4& v) noexcept;

	/*
	* @brief NNormalizes first 2 components (x,y) of 4D vector.
	*
	* Computes a unit vector from x/y components while leaving z/w unchanged.
	*
	* @param v Input vector (const ref).
	*
	* @return Normalized vector with original z/w.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize2(const NuVec4& v) noexcept;

	/*
	* @brief Normalizes first 3 components (x,y,z) of 4D vector.
	*
	* Computes unit vector from x,y,z components while preserving w.
	*
	* @param v Input vector (const ref).
	*
	* @return Normalized vector with original w component.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize3(const NuVec4& v) noexcept;

	/*
	* @brief Normalizes full 4D vector.
	*
	* Computes unit vector from all components (x,y,z,w).
	*
	* @param v Input vector (const ref).
	*
	* @return Fully normalized vector.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Normalize4(const NuVec4& v) noexcept;

	// =============================================
	// Extra Math Utilities
	// =============================================

	/*
	* @brief Computes 3D cross product of two vectors (ignores w components).
	*
	* Calculates the cross product for the first three components (x,y,z), setting w component to 0.
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Cross product vector {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0}.
	*/
	[[nodiscard]] NU_FORCEINLINE NuVec4 Cross(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Computes 3D dot product (ignores w components)
	*
	* Calculates the dot product of the first three components (x,y,z).
	*
	* @param a First vector.
	* @param b Second vector.
	*
	* @return Dot product (a.x*b.x + a.y*b.y + a.z*b.z).
	*/
	[[nodiscard]] NU_FORCEINLINE float Dot3(const NuVec4& a, const NuVec4& b) noexcept;

	/*
	* @brief Computes 3D vector length (ignores w component).
	*
	* Calculates the Euclidean length of the first three components (x,y,z).
	*
	* @param v Input vector.
	* @return Length of vector (sqrt(x² + y² + z²)).
	*/
	[[nodiscard]] NU_FORCEINLINE float Length3(const NuVec4& v) noexcept;
}