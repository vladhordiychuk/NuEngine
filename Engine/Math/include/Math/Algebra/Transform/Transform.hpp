// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Math/Algebra/Vector/Vector3.hpp>
#include <Math/Algebra/Quaternion/Quaternion.hpp>
#include <Math/Algebra/Matrix/Matrix4x4.hpp>

#include <string>
#include <sstream>

namespace NuEngine::Math
{
	class Transform
	{
	public:
		Transform() noexcept;
		Transform(const Vector3& position, const Quaternion& rotation, const Vector3 scale) noexcept;
		Transform(const Transform&) noexcept;
		Transform(Transform&&) noexcept;

		void SetPosition(const Vector3& position) noexcept;
		void SetRotation(const Quaternion& rotation) noexcept;
		void SetScale(const Vector3& scale) noexcept;

		const Vector3& GetPosition() const noexcept;
		const Quaternion& GetRotation() const noexcept;
		const Vector3& GetScale() const noexcept;

		[[nodiscard]] Matrix4x4 GetMatrix() const noexcept;

		[[nodiscard]] Matrix4x4 GetInverseMatrix() const noexcept;

		Transform& operator=(const Transform&) noexcept;
		Transform& operator=(Transform&&) noexcept;

		[[nodiscard]] Vector3 GetForward() const noexcept;
		[[nodiscard]] Vector3 GetUp() const noexcept;
		[[nodiscard]] Vector3 GetRight() const noexcept;

		std::string ToString() const;

	private:
		Vector3 m_position;
		Quaternion m_rotation;
		Vector3 m_scale;
	};
}