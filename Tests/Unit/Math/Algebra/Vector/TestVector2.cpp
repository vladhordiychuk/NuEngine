#include <gtest/gtest.h>
#include <Math/Math.hpp>

using namespace NuEngine::Math;

TEST(Vector2Test, Addition)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(3.0f, 4.0f);
	Vector2 result = a + b;

	EXPECT_FLOAT_EQ(result.X(), 4.0f);
	EXPECT_FLOAT_EQ(result.Y(), 6.0f);
}

TEST(Vector2Test, EqualAddition)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(3.0f, 4.0f);
	a += b;

	EXPECT_FLOAT_EQ(a.X(), 4.0f);
	EXPECT_FLOAT_EQ(a.Y(), 6.0f);
}

TEST(Vector2Test, Subtraction)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 b(1.0f, 2.0f);
	Vector2 result = a - b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 2.0f);
}

TEST(Vector2Test, EqualSubtraction)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 b(1.0f, 2.0f);
	a -= b;

	EXPECT_FLOAT_EQ(a.X(), 2.0f);
	EXPECT_FLOAT_EQ(a.Y(), 2.0f);
}

TEST(Vector2Test, Multiplication)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(3.0f, 4.0f);
	Vector2 result = a* b;

	EXPECT_FLOAT_EQ(result.X(), 3.0f);
	EXPECT_FLOAT_EQ(result.Y(), 8.0f);
}

TEST(Vector2Test, EqualMultiplication)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(3.0f, 4.0f);
	a *= b;

	EXPECT_FLOAT_EQ(a.X(), 3.0f);
	EXPECT_FLOAT_EQ(a.Y(), 8.0f);
}

TEST(Vector2Test, Division)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 b(1.0f, 2.0f);
	Vector2 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 3.0f);
	EXPECT_FLOAT_EQ(result.Y(), 2.0f);
}

TEST(Vecto2Test, EqualDivision)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 b(1.0f, 2.0f);
	a /= b;

	EXPECT_FLOAT_EQ(a.X(), 3.0f);
	EXPECT_FLOAT_EQ(a.Y(), 2.0f);
}

TEST(Vector2Test, ScalarMultiplication)
{
	Vector2 a(1.0f, 2.0f);
	float b = 2.0f;
	Vector2 result = a * b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
}

TEST(Vector2Test, EqualScalarMultiplication)
{
	Vector2 a(1.0f, 2.0f);
	float b = 2.0f;
	a *= b;

	EXPECT_FLOAT_EQ(a.X(), 2.0f);
	EXPECT_FLOAT_EQ(a.Y(), 4.0f);
}

TEST(Vector2Test, ScalarDivision)
{
	Vector2 a(2.0f, 4.0f);
	float b = 2.0f;
	Vector2 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 1.0f);
	EXPECT_FLOAT_EQ(result.Y(), 2.0f);
}

TEST(Vector2Test, EqualScalarDivision)
{
	Vector2 a(2.0f, 4.0f);
	float b = 2.0f;
	a /= b;

	EXPECT_FLOAT_EQ(a.X(), 1.0f);
	EXPECT_FLOAT_EQ(a.Y(), 2.0f);
}

TEST(Vector2Test, Equal)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(1.0f, 2.0f);

	EXPECT_TRUE(a == b);
}

TEST(Vector2Test, NegativeEqual)
{
	Vector2 a(-1.0f, -2.0f);
	Vector2 b(-1.0f, -2.0f);

	EXPECT_TRUE(a == b);
}

TEST(Vector2Test, NotEqual)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(2.0f, 1.0f);

	EXPECT_TRUE(a != b);
}

TEST(Vector2Test, DotProduct)
{
	Vector2 a(1.0f, 0.0f);
	Vector2 b(0.0f, 1.0f);
	float dot = a.Dot(b);

	EXPECT_FLOAT_EQ(dot, 0.0f);
}

TEST(Vector2Test, OppositeVectors)
{
	Vector2 a(1.0f, 0.0f);
	Vector2 b(-1.0f, 0.0f);

	EXPECT_FLOAT_EQ(a.Dot(b), -1.0f);
}

TEST(Vector2Test, Length)
{
	Vector2 a(3.0f, 4.0f);
	float length = a.Length();

	EXPECT_NEAR(length, 5.0f, 1e-3f);
}

TEST(Vector2Test, Distance)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(4.0f, 6.0f);
	float dist = a.Distance(b);

	EXPECT_NEAR(dist, 5.0f, 1e-3f);
}

TEST(Vector2Test, DistanceSymmetry)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(4.0f, 5.0f);
	EXPECT_FLOAT_EQ(a.Distance(b), b.Distance(a));
}

TEST(Vector2Test, Normalize)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 result = a.Normalize();

	EXPECT_NEAR(result.X(), 0.6f, 2e-3f);
	EXPECT_NEAR(result.Y(), 0.8f, 2e-3f);
}