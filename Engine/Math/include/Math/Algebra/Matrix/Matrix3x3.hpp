// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Detail/SIMD/SIMDBackend.hpp>
#include <Math/Algebra/Vector/Vector3.hpp>

namespace NuEngine::Math
{
	class alignas(16) Matrix3x3
	{
	public:
		Matrix3x3() noexcept = default;
	private:
		Simd::NuMat3 m_data;

		explicit Matrix3x3(const Simd::NuMat4&);
	};
}