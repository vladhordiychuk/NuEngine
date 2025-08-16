#include <gtest/gtest.h>
#include <Math/Math.hpp>

using namespace NuEngine::Math;

TEST(Vector3Test, Addition) 
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = a + b;

    EXPECT_FLOAT_EQ(result.X(), 5.0f);
    EXPECT_FLOAT_EQ(result.Y(), 7.0f);
    EXPECT_FLOAT_EQ(result.Z(), 9.0f);
}

TEST(Vector3Test, Substraction)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 result = a - b;

    EXPECT_FLOAT_EQ(result.X(), 3.0f);
    EXPECT_FLOAT_EQ(result.Y(), 3.0f);
    EXPECT_FLOAT_EQ(result.Z(), 3.0f);
}

TEST(Vector3Test, Multiplication)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = a * b;

    EXPECT_FLOAT_EQ(result.X(), 4.0f);
    EXPECT_FLOAT_EQ(result.Y(), 10.0f);
    EXPECT_FLOAT_EQ(result.Z(), 18.0f);
}

TEST(Vector3Test, FloatMultiplication)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    Vector3 result = a * b;

    EXPECT_FLOAT_EQ(result.X(), 2.0f);
    EXPECT_FLOAT_EQ(result.Y(), 4.0f);
    EXPECT_FLOAT_EQ(result.Z(), 6.0f);
}

TEST(Vector3Test, FloatDivision)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    float b = 2.0f;
    Vector3 result = a / b;

    EXPECT_FLOAT_EQ(result.X(), 2.0f);
    EXPECT_FLOAT_EQ(result.Y(), 2.5f);
    EXPECT_FLOAT_EQ(result.Z(), 3.0f);
}

TEST(Vector3Test, DotProduct) 
{
    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    float dot = a.Dot(b);

    EXPECT_FLOAT_EQ(dot, 0.0f); 
}

TEST(Vector3Test, CrossProduct)
{
    Vector3 a(1.0f, 0.0f, 0.0f);  
    Vector3 b(0.0f, 1.0f, 0.0f);  

    Vector3 cross = a.Cross(b);  

    EXPECT_FLOAT_EQ(cross.X(), 0.0f);
    EXPECT_FLOAT_EQ(cross.Y(), 0.0f);
    EXPECT_FLOAT_EQ(cross.Z(), 1.0f);
}

TEST(Vector3Test, Length) 
{
    Vector3 a(3, 4, 0);
    float length = a.Length();

    EXPECT_FLOAT_EQ(length, 5.0f);
}
