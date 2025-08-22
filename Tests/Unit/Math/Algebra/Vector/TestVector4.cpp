#include <gtest/gtest.h>
#include <Math/Math.hpp>

using namespace NuEngine::Math;

TEST(Vector4Test, Addition)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 result = a + b;

	EXPECT_FLOAT_EQ(result.X(), 6.0f);
	EXPECT_FLOAT_EQ(result.Y(), 8.0f);
	EXPECT_FLOAT_EQ(result.Z(), 10.0f);
	EXPECT_FLOAT_EQ(result.W(), 12.0f);
}

TEST(Vector4Test, EqualAddition)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	a += b;

	EXPECT_FLOAT_EQ(a.X(), 6.0f);
	EXPECT_FLOAT_EQ(a.Y(), 8.0f);
	EXPECT_FLOAT_EQ(a.Z(), 10.0f);
	EXPECT_FLOAT_EQ(a.W(), 12.0f);
}

TEST(Vector4Test, Subtraction)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 result = a - b;

	EXPECT_FLOAT_EQ(result.X(), 4.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
	EXPECT_FLOAT_EQ(result.Z(), 4.0f);
	EXPECT_FLOAT_EQ(result.W(), 4.0f);
}

TEST(Vector4Test, EqualSubtraction)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	a -= b;

	EXPECT_FLOAT_EQ(a.X(), 4.0f);
	EXPECT_FLOAT_EQ(a.Y(), 4.0f);
	EXPECT_FLOAT_EQ(a.Z(), 4.0f);
	EXPECT_FLOAT_EQ(a.W(), 4.0f);
}

TEST(Vector4Test, Multiplication)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 result = a * b;

	EXPECT_FLOAT_EQ(result.X(), 5.0f);
	EXPECT_FLOAT_EQ(result.Y(), 12.0f);
	EXPECT_FLOAT_EQ(result.Z(), 21.0f);
	EXPECT_FLOAT_EQ(result.W(), 32.0f);
}

TEST(Vector4Test, EqualMultiplication)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	a *= b;

	EXPECT_FLOAT_EQ(a.X(), 5.0f);
	EXPECT_FLOAT_EQ(a.Y(), 12.0f);
	EXPECT_FLOAT_EQ(a.Z(), 21.0f);
	EXPECT_FLOAT_EQ(a.W(), 32.0f);
}

TEST(Vector4Test, ScalarMultiplication)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	float b = 2.0f;
	Vector4 result = a * b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
	EXPECT_FLOAT_EQ(result.Z(), 6.0f);
	EXPECT_FLOAT_EQ(result.W(), 8.0f);
}

TEST(Vector4Test, EqualScalarMultiplication)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	float b = 2.0f;
	a *= b;

	EXPECT_FLOAT_EQ(a.X(), 2.0f);
	EXPECT_FLOAT_EQ(a.Y(), 4.0f);
	EXPECT_FLOAT_EQ(a.Z(), 6.0f);
	EXPECT_FLOAT_EQ(a.W(), 8.0f);
}

TEST(Vector4Test, Division)
{
	Vector4 a(5.0f, 6.0f, 9.0f, 8.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 5.0f);
	EXPECT_FLOAT_EQ(result.Y(), 3.0f);
	EXPECT_FLOAT_EQ(result.Z(), 3.0f);
	EXPECT_FLOAT_EQ(result.W(), 2.0f);
}

TEST(Vector4Test, EqualDivision)
{
	Vector4 a(5.0f, 6.0f, 9.0f, 8.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	a /= b;

	EXPECT_FLOAT_EQ(a.X(), 5.0f);
	EXPECT_FLOAT_EQ(a.Y(), 3.0f);
	EXPECT_FLOAT_EQ(a.Z(), 3.0f);
	EXPECT_FLOAT_EQ(a.W(), 2.0f);
}

TEST(Vector4Test, ScalarDivision)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	float b = 2.0f;
	Vector4 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 2.5f);
	EXPECT_FLOAT_EQ(result.Y(), 3.0f);
	EXPECT_FLOAT_EQ(result.Z(), 3.5f);
	EXPECT_FLOAT_EQ(result.W(), 4.0f);
}

TEST(Vector4Test, EqualScalarDivision)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	float b = 2.0f;
	a /= b;

	EXPECT_FLOAT_EQ(a.X(), 2.5f);
	EXPECT_FLOAT_EQ(a.Y(), 3.0f);
	EXPECT_FLOAT_EQ(a.Z(), 3.5f);
	EXPECT_FLOAT_EQ(a.W(), 4.0f);
}

TEST(Vector4Test, Equal)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);

	EXPECT_TRUE(a == b);
}

TEST(Vector4Test, NegativeEqual)
{
	Vector4 a(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 b(-1.0f, -2.0f, -3.0f, -4.0f);

	EXPECT_TRUE(a == b);
}

TEST(Vector4Test, NotEqual)
{
	Vector4 a(1.0f, 3.0f, 2.0f, 4.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);

	EXPECT_TRUE(a != b);

}

TEST(Vector4Test, DotProduct)
{
	Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 b(0.0f, 1.0f, 0.0f, 0.0f);
	float dot = a.Dot(b);

	EXPECT_FLOAT_EQ(dot, 0.0f);
}

TEST(Vector4Test, OppositeVectors)
{
	Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 b(-1.0f, 0.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(a.Dot(b), -1.0f);
}

TEST(Vector4Test, Length)
{
	Vector4 a(3.0f, 4.0f, 0.0f, 0.0f);
	float length = a.Length();

	EXPECT_NEAR(length, 5.0f, 1e-3f);
}

TEST(Vector4Test, Distance)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 3.0f, 4.0f);
	float distance = a.Distance(b);

	EXPECT_NEAR(distance, std::sqrt(32.0f), 1e-2f);
}

TEST(Vector4Test, DistanceSymmetry)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);

	EXPECT_FLOAT_EQ(a.Distance(b), b.Distance(a));
}

TEST(Vector4Test, Normalize)
{
	Vector4 a(3.0f, 0.0f, 0.0f, 0.0f);
	Vector4 result = a.Normalize();

	EXPECT_NEAR(result.X(), 1.0f, 2e-3f);
	EXPECT_NEAR(result.Y(), 0.0f, 2e-3f);
	EXPECT_NEAR(result.Z(), 0.0f, 2e-3f);
	EXPECT_NEAR(result.W(), 0.0f, 2e-3f);
}
