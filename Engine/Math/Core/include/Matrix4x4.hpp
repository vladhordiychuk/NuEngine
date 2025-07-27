#pragma once

#include "../../SIMD/SIMDBackend.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

#include <string>
#include <sstream>

namespace Engine::Math
{
	class alignas(16) Matrix4x4
	{
	public:
		Matrix4x4() noexcept = default;
		Matrix4x4(const Matrix4x4&) noexcept = default;
		Matrix4x4(Matrix4x4&&) noexcept = default;

		[[nodiscard]] static Matrix4x4 Identity() noexcept;
		[[nodiscard]] static Matrix4x4 FromRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept;
		[[nodiscard]] static Matrix4x4 FromColumns(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3) noexcept;

		[[nodiscard]] Matrix4x4 operator*(const Matrix4x4&) const noexcept;
		[[nodiscard]] Vector4 operator*(const Vector4&) const noexcept;
		[[nodiscard]] Matrix4x4 operator+(const Matrix4x4&) const noexcept;
		[[nodiscard]] Matrix4x4 operator-(const Matrix4x4&) const noexcept;
		Matrix4x4& operator*=(const Matrix4x4&) noexcept;
		Matrix4x4& operator+=(const Matrix4x4&) noexcept;
		Matrix4x4& operator-=(const Matrix4x4&) noexcept;

		[[nodiscard]] Matrix4x4 Transpose() const noexcept;
		[[nodiscard]] float Determinant() const noexcept;
		[[nodiscard]] Matrix4x4 Inverse() const noexcept;

		[[nodiscard]] static Matrix4x4 CreateTranslation(const Vector3& translation) noexcept;
		[[nodiscard]] static Matrix4x4 CreateRotation(const Quaternion& rotation) noexcept;
		[[nodiscard]] static Matrix4x4 CreateScale(const Vector3& scale) noexcept;
		[[nodiscard]] static Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept;
		[[nodiscard]] static Matrix4x4 CreatePerspective(float fovY, float aspect, float nearZ, float farZ) noexcept;
		[[nodiscard]] static Matrix4x4 CreateOrthographic(float left, float right, float bottom, float top, float nearZ, float farZ) noexcept;
		void Decompose(Vector3& traslation, Quaternion& rotation, Vector3& scale) const noexcept;

		[[nodiscard]] Vector4 GetColumn(int index) const noexcept;
		[[nodiscard]] Vector4 GetRow(int index) const noexcept;
		[[nodiscard]] const float* Data() const noexcept;
		[[nodiscard]] float operator()(int row, int col) const noexcept;

		[[nodiscard]] bool IsIdentity(float epsilon = 0.0f) const noexcept;
		[[nodiscard]] std::string ToString() const;

	private:
		Simd::NuMat4 m_data;

		explicit Matrix4x4(const Simd::NuMat4& simdData) noexcept;
	};
}