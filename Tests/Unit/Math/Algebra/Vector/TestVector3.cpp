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

TEST(Vector3Test, EqualAddition)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    a += b;

    EXPECT_FLOAT_EQ(a.X(), 5.0f);
    EXPECT_FLOAT_EQ(a.Y(), 7.0f);
    EXPECT_FLOAT_EQ(a.Z(), 9.0f);
}

TEST(Vector3Test, Subtraction)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 result = a - b;

    EXPECT_FLOAT_EQ(result.X(), 3.0f);
    EXPECT_FLOAT_EQ(result.Y(), 3.0f);
    EXPECT_FLOAT_EQ(result.Z(), 3.0f);
}

TEST(Vector3Test, EqualSubtraction)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    a -= b;

    EXPECT_FLOAT_EQ(a.X(), 3.0f);
    EXPECT_FLOAT_EQ(a.Y(), 3.0f);
    EXPECT_FLOAT_EQ(a.Z(), 3.0f);
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

TEST(Vector3Test, EqualMultiplication)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    a *= b;

    EXPECT_FLOAT_EQ(a.X(), 4.0f);
    EXPECT_FLOAT_EQ(a.Y(), 10.0f);
    EXPECT_FLOAT_EQ(a.Z(), 18.0f);
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

TEST(Vector3Test, EqualFloatMultiplication)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    float b = 2.0f;
    a *= b;
    
    EXPECT_FLOAT_EQ(a.X(), 2.0f);
    EXPECT_FLOAT_EQ(a.Y(), 4.0f);
    EXPECT_FLOAT_EQ(a.Z(), 6.0f);
}

TEST(Vector3Test, Division)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 result = a / b;

    EXPECT_FLOAT_EQ(result.X(), 4.0f);
    EXPECT_FLOAT_EQ(result.Y(), 2.5f);
    EXPECT_FLOAT_EQ(result.Z(), 2.0f);
}

TEST(Vector3Test, EqualDivision)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    a /= b;

    EXPECT_FLOAT_EQ(a.X(), 4.0f);
    EXPECT_FLOAT_EQ(a.Y(), 2.5f);
    EXPECT_FLOAT_EQ(a.Z(), 2.0f);
}

TEST(Vector3Test, ScalarDivision)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    float b = 2.0f;
    Vector3 result = a / b;

    EXPECT_FLOAT_EQ(result.X(), 2.0f);
    EXPECT_FLOAT_EQ(result.Y(), 2.5f);
    EXPECT_FLOAT_EQ(result.Z(), 3.0f);
}

TEST(Vector3Test, EqualScalarDivision)
{
    Vector3 a(4.0f, 5.0f, 6.0f);
    float b = 2.0f;
    a /= b;

    EXPECT_FLOAT_EQ(a.X(), 2.0f);
    EXPECT_FLOAT_EQ(a.Y(), 2.5f);
    EXPECT_FLOAT_EQ(a.Z(), 3.0f);
}

TEST(Vector3Test, Equal)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);

    EXPECT_TRUE(a == b);
}

TEST(Vector3Test, NegativeEqual)
{
    Vector3 a(-1.0f, -2.0f, -3.0f);
    Vector3 b(-1.0f, -2.0f, -3.0f);

    EXPECT_TRUE(a == b);
}

TEST(Vector3Test, NotEqual)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 1.0f);

    EXPECT_TRUE(a != b);
}

TEST(Vector3Test, DotProduct) 
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    float dot = a.Dot(b);

    EXPECT_FLOAT_EQ(dot, 0.0f); 
}

TEST(Vector3Test, OppositeVectors) 
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(-1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(a.Dot(b), -1.0f);
    EXPECT_TRUE(a.Cross(b) == Vector3(0.0f, 0.0f, 0.0f));
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

TEST(Vector3Test, CrossWithSelf) 
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 cross = a.Cross(a);

    EXPECT_FLOAT_EQ(cross.X(), 0.0f);
    EXPECT_FLOAT_EQ(cross.Y(), 0.0f);
    EXPECT_FLOAT_EQ(cross.Z(), 0.0f);
}

TEST(Vector3Test, Length) 
{
    Vector3 a(3.0f, 4.0f, 0.0f);
    float length = a.Length();

    EXPECT_NEAR(length, 5.0f, 1e-3f);
}

TEST(Vector3Test, Distance)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 6.0f, 3.0f);
    float distance = a.Distance(b);

    EXPECT_NEAR(distance, 5.0f, 1e-3f);
}

TEST(Vector3Test, DistanceSymmetry)
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    EXPECT_FLOAT_EQ(a.Distance(b), b.Distance(a));
}

TEST(Vector3Test, Normalize)
{
    Vector3 a(3.0f, 0.0f, 0.0f);
    Vector3 result = a.Normalize();

    EXPECT_NEAR(result.X(), 1.0f, 2e-3f); 
    EXPECT_NEAR(result.Y(), 0.0f, 2e-3f);
    EXPECT_NEAR(result.Z(), 0.0f, 2e-3f);
}
