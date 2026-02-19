#include <Renderer/Camera.hpp>

namespace NuEngine::Renderer
{
	Camera::Camera(float fovRadians, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fovRadians), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{
		m_Transform.SetPosition(NuMath::Vector3(0.0f, 0.0f, 3.0f));
		RecalculateProjection();
	}

	void Camera::Move(const NuMath::Vector3& delta)
	{
		NuMath::Quaternion rotation = m_Transform.GetRotation();
		NuMath::Vector3 translation = rotation.RotateVector(delta);
		m_Transform.SetPosition(m_Transform.GetPosition() + translation);
	}

	void Camera::Rotate(const NuMath::Vector3& eulerAngles)
	{
		NuMath::Quaternion deltaRot = NuMath::Quaternion::FromEuler(
			eulerAngles.X(),
			eulerAngles.Y(),
			eulerAngles.Z()
		);
		m_Transform.SetRotation(m_Transform.GetRotation() * deltaRot);
	}

	void Camera::RecalculateProjection()
	{
		m_ProjectionMatrix = NuMath::Matrix4x4::CreatePerspective(m_Fov, m_AspectRatio, m_Near, m_Far);
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (height == 0) return;
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalculateProjection();
	}

	NuMath::Matrix4x4 Camera::GetViewMatrix() const
	{
		return m_Transform.GetInverseMatrix();
	}

	NuMath::Matrix4x4 Camera::GetViewProjectionMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}
}