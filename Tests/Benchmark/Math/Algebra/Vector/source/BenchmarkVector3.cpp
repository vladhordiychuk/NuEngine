#include <Tests/Benchmark/Math/Algebra/Vector/include/BenchmarkVector3.hpp>

namespace NuEngine::Benchmarks
{
	void RegisterVector3Benchmarks()
	{
		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Add, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a + b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, AddAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a += b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Sub, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a - b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, SubAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a -= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Mul, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, MulAssign, BM_Vector_AssignOp,
			[](auto& a, const auto& b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, ScalarMul, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a * b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, ScalarMulAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a *= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Div, BM_Vector_ScalarParam,
			[](auto& a, float b) { return a / b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, DivAssign, BM_Vector_AssignScalarOp,
			[](auto& a, float b) { return a /= b; });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Dot, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Dot(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Cross, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Cross(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Distance, BM_Vector_BinaryOp,
			[](auto& a, auto& b) { return a.Distance(b); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Length, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Length(); });

		REGISTER_VECTOR_BENCHMARK(NuEngine::Math::Vector3, Normalize, BM_Vector_UnaryFloat,
			[](auto& a) { return a.Normalize(); });
	}
}
