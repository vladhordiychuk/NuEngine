// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <benchmark/benchmark.h>

namespace NuEngine::Benchmarks
{
	template <typename VecType>
	constexpr VecType MakeTestVector()
	{
		if constexpr (VecType::Dimensions == 2)
		{
			return VecType( 1.0f, 2.0f );
		}
		else if constexpr (VecType::Dimensions == 3)
		{
			return VecType( 1.0f, 2.0f, 3.0f );
		}
		else if constexpr (VecType::Dimensions == 4)
		{
			return VecType( 1.0f, 2.0f, 3.0f, 4.0f );
		}
		else
		{
			static_assert(VecType::Dimensions >= 2 && VecType::Dimensions <= 4,
				"Unsupported vector dimension");
		}
	}

	template <typename VecType, typename Func>
	void BM_Vector_Array_BinaryOp(benchmark::State& state, Func func)
	{
		const size_t count = 100000;
		std::vector<VecType> A(count), B(count), C(count);

		for (size_t i = 0; i < count; ++i) 
		{
			A[i] = MakeTestVector<VecType>();
			B[i] = MakeTestVector<VecType>();
		}

		for (auto _ : state) {
			for (size_t i = 0; i < count; ++i) 
			{
				C[i] = func(A[i], B[i]);
			}
			benchmark::DoNotOptimize(C);
		}
	}


	template <typename VecType, typename Func>
	void BM_Vector_BinaryOp(benchmark::State& state, Func func)
	{
		VecType a = MakeTestVector<VecType>();
		VecType b = MakeTestVector<VecType>();
		decltype(func(a, b)) c{};
		for (auto _ : state)
		{
			c = func(a, b);
			benchmark::DoNotOptimize(c);
		}
	}

	template <typename VecType, typename Func>
	void BM_Vector_AssignOp(benchmark::State& state, Func func)
	{
		VecType a = MakeTestVector<VecType>();
		VecType b = MakeTestVector<VecType>();
		for (auto _ : state)
		{
			VecType temp = a;         
			func(temp, b);
			benchmark::DoNotOptimize(temp);
		}
	}


	template <typename VecType, typename Func>
	void BM_Vector_UnaryFloat(benchmark::State& state, Func func)
	{
		VecType a = MakeTestVector<VecType>();
		decltype(func(a)) f{};
		for (auto _ : state)
		{
			f = func(a);
			benchmark::DoNotOptimize(f);
		}
	}

	template <typename VecType, typename Func>
	void BM_Vector_ScalarParam(benchmark::State& state, Func func)
	{
		VecType a = MakeTestVector<VecType>();
		VecType c{};
		float scalar = 2.5f;
		for (auto _ : state)
		{
			c = func(a, scalar);
			benchmark::DoNotOptimize(c);
		}
	}

	template <typename VecType, typename Func>
	void BM_Vector_AssignScalarOp(benchmark::State& state, Func func)
	{
		VecType a = MakeTestVector<VecType>();
		float scalar = 2.5f;
		for (auto _ : state)
		{
			VecType temp = a;
			func(temp, scalar);
			benchmark::DoNotOptimize(temp);
		}
	}
}

#define REGISTER_VECTOR_BENCHMARK(vecType, name, benchFunc, lambda) \
	benchmark::RegisterBenchmark(#vecType "_" #name, \
		[](benchmark::State& state){ benchFunc<vecType>(state, lambda); })