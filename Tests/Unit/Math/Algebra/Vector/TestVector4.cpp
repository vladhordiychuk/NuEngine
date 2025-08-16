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

TEST(Vector4Test, Substraction)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 result = a - b;

	EXPECT_FLOAT_EQ(result.X(), 4.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
	EXPECT_FLOAT_EQ(result.Z(), 4.0f);
	EXPECT_FLOAT_EQ(result.W(), 4.0f);
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

TEST(Vector4Test, FloatMultiplication)
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	float b = 2.0f;
	Vector4 result = a * b;

	EXPECT_FLOAT_EQ(result.X(), 2.0f);
	EXPECT_FLOAT_EQ(result.Y(), 4.0f);
	EXPECT_FLOAT_EQ(result.Z(), 6.0f);
	EXPECT_FLOAT_EQ(result.W(), 8.0f);
}

TEST(Vector4Test, FloatDivision)
{
	Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
	float b = 2.0f;
	Vector4 result = a / b;

	EXPECT_FLOAT_EQ(result.X(), 2.5f);
	EXPECT_FLOAT_EQ(result.Y(), 3.0f);
	EXPECT_FLOAT_EQ(result.Z(), 3.5f);
	EXPECT_FLOAT_EQ(result.W(), 4.0f);
}