// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>

#include <NuMath/Algebra/Matrix/MatrixAPI.hpp>
#include <NuMath/Batch/BatchLoop.hpp>

namespace NuMath::Batch
{
	namespace MatImpl
	{
		struct LoadOp
		{
			NU_FORCEINLINE auto operator()(const auto& m) { return MatrixAPI::Load(m); }
		};

		struct StoreOp
		{
			NU_FORCEINLINE auto operator()(auto& dst, auto m) { return MatrixAPI::Store(dst, m); }
		};

		struct AddOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) { return MatrixAPI::Add(a, b); }
		};

		struct SubOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) { return MatrixAPI::Sub(a, b); }
		};

		struct MulOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) { return MatrixAPI::Mul(a, b); }
		};
	}

	template <Func func>
	NU_FORCEINLINE void RunBinary(NuMatStorage4x4* NU_RESTRICT r, const NuMatStorage4x4* NU_RESTRICT a, size_t count) noexcept
	{
		Detial::Batch::ProcessLoopBIbary(r, a, count, func, MatImpl::LoadOp{}, MatImpl::StoreOp{});
	}
}
