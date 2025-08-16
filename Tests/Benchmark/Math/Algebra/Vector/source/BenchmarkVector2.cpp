#include <Tests/Benchmark/Math/Algebra/Vector/include/BenchmarkVector2.hpp>

namespace Benchmark
{
	void RegisterVector2Benchmarks()
	{
		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Add_Array100k, BM_Vector_Array_BinaryOp,
			[](auto& a, auto& b) { return a + b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Add, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a + b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, AddAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a += b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Sub, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a - b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, SubAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a -= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Mul, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, MulAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, ScalarMul, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, ScalarMulAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Div, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a / b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, DivAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a /= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Dot, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Dot(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Distance, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Distance(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Length, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Length(); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector2, Normalize, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Normalize(); });
	}
}

