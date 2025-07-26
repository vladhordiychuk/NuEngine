#pragma once

#include "../../SIMD/SIMDBackend.hpp"
#include "Vector3.hpp"

namespace Engine::Math
{
	class alignas(16) Matrix3x3
	{
	public:
		Matrix3x3() noexcept = default;
	private:
		Simd::NuMat3 m_data;

		explicit Matrix3x3(const Simd::NuMat4&)
	};
}