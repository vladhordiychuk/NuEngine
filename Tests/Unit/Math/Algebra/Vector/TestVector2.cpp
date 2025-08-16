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

TEST(Vector2Test, Substraction)
{
	Vector2 a(3.0f, 4.0f);
	Vector2 b(1.0f, 2.0f);
	Vector2 result = a - b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 2.0f);
}

TEST(Vector2Test, Multiplication)
{
	Vector2 a(1.0f, 2.0f);
	Vector2 b(3.0f, 4.0f);
	Vector2 result = a* b;

	EXPECT_FLOAT_EQ(result.X(), 3.0f);
	EXPECT_FLOAT_EQ(result.Y(), 8.0f);
}

TEST(Vector2Test, FloatMultiplication)
{
	Vector2 a(1.0f, 2.0f);
	float b = 2.0f;
	Vector2 result = a * b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
}

TEST(Vector2Test, FloatDivision)
{
	Vector2 a(2.0f, 4.0f);
	float b = 2.0f;
	Vector2 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 1.0f);
	EXPECT_FLOAT_EQ(result.Y(), 2.0f);
}

