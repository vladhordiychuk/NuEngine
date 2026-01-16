// Copyright(c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/Core/StorageTypes.hpp>
#include <NuMath/Core/Common.hpp>

namespace NuMath::Detail::Batch
{
	// =============================================================
	// UNARY (A) -> Result
	// =============================================================
	template <typename T, typename Op, typename Loader, typename Storer>
	NU_FORCEINLINE void ProcessLoopUnary(T* NU_RESTRICT r,
		const T* NU_RESTRICT a,
		size_t count,
		Op op,
		Loader load,
		Storer store) noexcept
	{
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(a) % 16 == 0), "Input A must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(r) % 16 == 0), "Output result must be aligned!");

		if (count == 0) return;
		size_t i = 0;

		for (; i + 3 < count; i += 4)
		{
			store(r[i], op(load(a[i])));
			store(r[i + 1], op(load(a[i + 1])));
			store(r[i + 2], op(load(a[i + 2])));
			store(r[i + 3], op(load(a[i + 3])));
		}

		for (; i < count; ++i)
		{
			store(r[i], op(load(a[i])));
		}
	}

	// =============================================================
	// BINARY (A, B) -> Result
	// =============================================================
	template <typename T, typename Op, typename Loader, typename Storer>
	NU_FORCEINLINE void ProcessLoopBinary(T* NU_RESTRICT r,
		const T* NU_RESTRICT a,
		const T* NU_RESTRICT b,
		size_t count,
		Op op,
		Loader load,
		Storer store) noexcept
	{
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(a) % 16 == 0), "Input A must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(b) % 16 == 0), "Input B must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(r) % 16 == 0), "Output result must be aligned!");

		if (count == 0) return;
		size_t i = 0;

		for (; i + 3 < count; i += 4)
		{
			auto va0 = load(a[i]);
			auto vb0 = load(b[i]);
			store(r[i], op(va0, vb0));

			auto va1 = load(a[i + 1]);
			auto vb1 = load(b[i + 1]);
			store(r[i + 1], op(va1, vb1));

			auto va2 = load(a[i + 2]);
			auto vb2 = load(b[i + 2]);
			store(r[i + 2], op(va2, vb2));

			auto va3 = load(a[i + 3]);
			auto vb3 = load(b[i + 3]);
			store(r[i + 3], op(va3, vb3));
		}

		for (; i < count; ++i)
		{
			auto va = load(a[i]);
			auto vb = load(b[i]);
			store(r[i], op(va, vb));
		}
	}

	// =============================================================
	// TERNARY (A, B, C) -> Result
	// =============================================================
	template <typename T, typename Op, typename Loader, typename Storer>
	NU_FORCEINLINE void ProcessLoopTernary(T* NU_RESTRICT r, 
		const T* NU_RESTRICT a, 
		const T* NU_RESTRICT b, 
		const T* NU_RESTRICT c, 
		size_t count, 
		Op op, 
		Loader load, 
		Storer store) noexcept
	{
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(a) % 16 == 0), "Input A must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(b) % 16 == 0), "Input B must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(c) % 16 == 0), "Input B must be aligned!");
		NU_MATH_ASSERT((reinterpret_cast<uintptr_t>(r) % 16 == 0), "Output result must be aligned!");

		if (count == 0) return;
		size_t i = 0;

		for (; i + 3 < count; i += 4)
		{
			auto va0 = load(a[i]);
			auto vb0 = load(b[i]);
			auto vc0 = load(c[i]);
			store(r[i], op(va0, vb0, vc0));

			auto va1 = load(a[i+1]);
			auto vb1 = load(b[i+1]);
			auto vc1 = load(c[i+1]);
			store(r[i+1], op(va1, vb1, vc1));

			auto va2 = load(a[i+2]);
			auto vb2 = load(b[i+2]);
			auto vc2 = load(c[i+2]);
			store(r[i+2], op(va2, vb2, vc2));

			auto va3 = load(a[i+3]);
			auto vb3 = load(b[i+3]);
			auto vc3 = load(c[i+3]);
			store(r[i+3], op(va3, vb3, vc3));
		}

		for (; i < count; ++i)
		{
			auto va = load(a[i]);
			auto vb = load(b[i]);
			auto vc = load(c[i]);
			store(r[i], op(va, vb, vc));
		}
	}
} // namespace NuMath::Detail::Batch