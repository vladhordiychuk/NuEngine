// Copyright(c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/Common.hpp>
#include <NuMath/Core/StorageTypes.hpp>

namespace NuMath::Detail::Batch::SoA
{
	template <typename V1, typename V2>
	constexpr bool AreDimensionsEqual = (V1::Size == V2::Size);

	template <typename Backend, typename ViewR, typename ViewA, typename Op>
	NU_FORCEINLINE void RunUnary(ViewR r, ViewA a, size_t count, Op op) noexcept
	{
		NU_MATH_ASSERT((AreDimensionsEqual<ViewR, ViewA>), "Unary: Input and Output dimensions must match!");

		using Register = typename Backend::Register;
		constexpr size_t N = ViewR::Size;
		constexpr size_t Pack = Backend::Width;

		size_t i = 0;
		const size_t limit = (count / Pack) * Pack;

		for (; i < limit; i += Pack)
		{
			for (size_t d = 0; d < N; ++d)
			{
				Register va = Backend::Load(a.streams[d] + i);
				Register res = op(va);
				Backend::Stream(r.streams[d] + i, res);
			}
		}

		for (; i < count; ++i)
		{
			for (size_t d = 0; d < N; ++d)
			{
				float va = a.streams[d][i];

				r.streams[d][i] = op(va);
			}
		}
	}

	template <typename Backend, typename ViewR, typename ViewA, typename ViewB, typename Op>
	NU_FORCEINLINE void RunBinary(ViewR r, ViewA a, ViewB b, size_t count, Op op) noexcept
	{
		NU_MATH_ASSERT((AreDimensionsEqual<ViewR, ViewA> && AreDimensionsEqual<ViewA, ViewB>), "Binary: All dimensions must match!");

		using Register = typename Backend::Register;
		constexpr size_t N = ViewR::Size;
		constexpr size_t Pack = Backend::Width;

		size_t i = 0;
		const size_t limit = (count / Pack) * Pack;

		for (; i < limit; i += Pack)
		{
			for (size_t d = 0; d < N; ++d)
			{
				Register va = Backend::Load(a.streams[d] + i);
				Register vb = Backend::Load(b.streams[d] + i);
				Register res = op(va, vb);

				Backend::Stream(r.streams[d] + i, res);
			}
		}

		for (; i < count; ++i)
		{
			for (size_t d = 0; d < N; ++d)
			{
				float va = a.streams[d][i];
				float vb = b.streams[d][i];

				r.streams[d][i] = op(va, vb);
			}
		}
	}

	template <typename Backend, typename ViewR, typename ViewA, typename ViewB, typename ViewC, typename Op>
	NU_FORCEINLINE void RunTernary(ViewR r, ViewA a, ViewB b, ViewC c, size_t count, Op op) noexcept
	{
		NU_MATH_ASSERT((AreDimensionsEqual<ViewR, ViewA> && AreDimensionsEqual<ViewA, ViewB> && AreDimensionsEqual<ViewA, ViewC>), "Ternary: All dimensions must match!");

		using Register = typename Backend::Register;
		constexpr size_t Pack = Backend::Width;
		constexpr size_t N = ViewR::Size;

		size_t i = 0;
		const size_t limit = (count / Pack) * Pack;

		for (; i < limit; i += Pack)
		{
			for (size_t d = 0; d < N; ++d)
			{
				Register va = Backend::Load(a.streams[d] + i);
				Register vb = Backend::Load(b.streams[d] + i);
				Register vc = Backend::Load(c.streams[d] + i);
				Register res = op(va, vb, vc);

				Backend::Stream(r.streams[d] + i, res);
			}
		}

		for (; i < count; ++i)
		{
			for (size_t d = 0; d < N; ++d)
			{
				float va = a.streams[d][i];
				float vb = b.streams[d][i];
				float vc = c.streams[d][i];

				r.streams[d][i] = op(va, vb, vc);
			}
		}
	}
}