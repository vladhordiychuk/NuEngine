// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Algebra/Vector/VectorAPI.hpp>
#include <NuMath/Batch/Common/BatchLoop.hpp>

#include <algorithm>
#include <cmath>

namespace NuMath::Batch
{
	namespace VecImpl
	{
		struct LoadOp
		{
			NU_FORCEINLINE auto operator()(const auto& v) noexcept { return VectorAPI::Load(v); }
		};

		struct StoreOp
		{
			NU_FORCEINLINE auto operator()(auto& dst, auto v) noexcept { return VectorAPI::Store(dst, v); }
		};

		struct StreamOp
		{
			NU_FORCEINLINE auto operator()(auto& dst, auto v) noexcept { return VectorAPI::Stream(&dst, v); }
		};

		struct AddOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) noexcept { return VectorAPI::Add(a, b); }
			NU_FORCEINLINE float operator()(float a, float b) noexcept { return a + b; }
		};

		struct SubOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) noexcept { return VectorAPI::Sub(a, b); }
			NU_FORCEINLINE float operator()(float a, float b) noexcept { return a - b; }
		};

		struct MulOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) noexcept { return VectorAPI::Mul(a, b); }
			NU_FORCEINLINE float operator()(float a, float b) noexcept { return a * b; }
		};

		struct DivOp
		{
			NU_FORCEINLINE auto operator()(auto a, auto b) noexcept { return VectorAPI::Div(a, b); }
			NU_FORCEINLINE float operator()(float a, float b) noexcept { return a / b; }
		};
	} // namespace VecImpl

	template <typename Func>
	NU_FORCEINLINE void RunBinary(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count, Func func) noexcept
	{
		Detail::Batch::ProcessLoopBinary(r, a, b, count, func, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	template <typename Func>
	NU_FORCEINLINE void RunUnary(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, size_t count, Func func) noexcept
	{
		Detail::Batch::ProcessLoopUnary(r, a, count, func, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	template <typename Func>
	NU_FORCEINLINE void RunTernary(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, const NuVecStorage4* c, size_t count, Func func) noexcept
	{
		Detail::Batch::ProcessLoopTernary(r, a, b, c, count, func, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	NU_FORCEINLINE void Add(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count) noexcept
	{
		Detail::Batch::ProcessLoopBinary(r, a, b, count, VecImpl::AddOp{}, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	NU_FORCEINLINE void Sub(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count) noexcept
	{
		Detail::Batch::ProcessLoopBinary(r, a, b, count, VecImpl::SubOp{}, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	NU_FORCEINLINE void Mul(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count) noexcept
	{
		Detail::Batch::ProcessLoopBinary(r, a, b, count, VecImpl::MulOp{}, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	NU_FORCEINLINE void Div(NuVecStorage4* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count) noexcept
	{
		Detail::Batch::ProcessLoopBinary(r, a, b, count, VecImpl::DivOp{}, VecImpl::LoadOp{}, VecImpl::StreamOp{});
	}

	NU_FORCEINLINE void Dot(float* NU_RESTRICT r, const NuVecStorage4* NU_RESTRICT a, const NuVecStorage4* NU_RESTRICT b, size_t count) noexcept
	{
		for (size_t i = 0; i < count; ++i)
		{
			auto va = VectorAPI::Load(a[i]);
			auto vb = VectorAPI::Load(b[i]);

			float res = VectorAPI::Dot4(va, vb);

			r[i] = res;
		}
	}
} // namespace NuMath::Batch