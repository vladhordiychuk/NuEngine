// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Types.hpp>

#include <cmath>
#include <cassert>

#include <Math/Detail/SIMD/SIMDBackend.hpp>

namespace NuEngine::Math::MatrixAPI
{
	// =============================================
	// Type definitions
	// =============================================

	using NuMat4 = Simd::NuMat4;

	// =============================================
	// Setters
	// =============================================

	/*
	*
	*/
	using Simd::SetIdentityMatrix;

	/*
	*
	*/
	using Simd::Transpose;

	/*
	*
	*/
	using Simd::Add;

	/*
	*
	*/
	using Simd::Sub;

	/*
	*
	*/
	using Simd::Mul;

	/*
	*
	*/
	using Simd::FromRows;

	/*
	*
	*/
	using Simd::FromColumns;

	/*
	*
	*/
	using Simd::Determinant;

	/*
	*
	*/
	using Simd::CreateTranslation;

	/*
	*
	*/
	using Simd::GetColumn;

	/*
	*
	*/
	using Simd::GetRow;

	/*
	*
	*/
	using Simd::Access;

	/*
	*
	*/
	using Simd::Data;
}