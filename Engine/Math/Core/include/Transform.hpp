#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

#include <string>
#include <sstream>

namespace Engine::Math
{
	class Transform
	{
	public:
		Transform() noexcept = default;
		Transform(const Vector3& position, const Quaternion& rotation, const Vector3 scale) noexcept;
		Transform(const Transform&) noexcept = default;
		Transform(Transform&&) noexcept = default;

		void SetPosition(const Vector3& position) noexcept;
		void SetRotation(const Quaternion& rotation) noexcept;
		void SetScale(const Vector3& scale) noexcept;

		const Vector3& GetPosition() const noexcept;
		const Quaternion& GetRotation() const noexcept;
		const Vector3& GetScale() const noexcept;

		[[nodiscard]] Matrix4x4 GetMatrix() const noexcept;

		[[nodiscard]] Matrix4x4 GetInverseMatrix() const noexcept;

		Transform& operator=(const Transform&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

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