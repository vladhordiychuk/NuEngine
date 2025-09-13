#include <Tests/Benchmark/Math/Algebra/Vector/include/BenchmarkVector4.hpp>

namespace NuEngine::Benchmarks
{
	void RegisterVector4Benchmarks()
	{
		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Add, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a + b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, AddAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a += b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Sub, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a - b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, SubAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a -= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Mul, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, MulAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, ScalarMul, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, ScalarMulAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Div, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a / b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, DivAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a /= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Dot, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Dot(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Distance, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Distance(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Length, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Length(); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector4, Normalize, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Normalize(); });
	}
}
