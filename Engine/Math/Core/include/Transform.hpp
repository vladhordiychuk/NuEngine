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
		Transform();
		Transform(const Vector3& position, const Quaternion& rotation, const Vector3 scale);

		void SetPosition(const Vector3& position);
		void SetRotation(const Quaternion& rotation);
		void SetScale(const Vector3& scale);

		const Vector3& GetPosition() const;
		const Quaternion& GetRotation() const;
		const Vector3& GetScale() const;

		Matrix4x4 GetMatrix() const;

		Matrix4x4 GetInverseMatrix() const;

		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		std::string ToString() const;

	private:
		Vector3 m_position;
		Quaternion m_rotation;
		Vector3 m_scale;
	};
}