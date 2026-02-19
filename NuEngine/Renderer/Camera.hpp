// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <NuMath/NuMath.hpp>

namespace NuEngine::Renderer
{
	class Camera
	{
	public:
		Camera(float fovRadians, float aspectRatio, float nearClip, float farClip);
		~Camera() = default;

		void SetPosition(const NuMath::Vector3& position) { m_Transform.SetPosition(position); }
		[[nodiscard]] NuMath::Vector3 GetPosition() const { return m_Transform.GetPosition(); }

		void SetRotation(const NuMath::Quaternion& rotation) { m_Transform.SetRotation(rotation); }
		[[nodiscard]] NuMath::Quaternion GetRotation() const { return m_Transform.GetRotation(); }

		void Move(const NuMath::Vector3& delta);

		void Rotate(const NuMath::Vector3& eulerAngles);

		NuMath::Transform& GetTransform() { return m_Transform; }

		[[nodiscard]] NuMath::Matrix4x4 GetViewMatrix() const;

		[[nodiscard]] const NuMath::Matrix4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		[[nodiscard]] NuMath::Matrix4x4 GetViewProjectionMatrix() const;

		void SetPerspective(float fovRadians, float aspectRatio, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void RecalculateProjection();

		NuMath::Transform m_Transform;
		NuMath::Matrix4x4 m_ProjectionMatrix;

		float m_Fov;
		float m_AspectRatio;
		float m_Near;
		float m_Far;
	};
}