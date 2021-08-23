#include "atlpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Atlas
{
	void PerspectiveCamera::RecalculateViewMatrix()
	{
		ATL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::PerspectiveCamera(float nearPlane, float farPlane, float fov, float aspectRatio)
		: m_NearPlane(nearPlane), m_FarPlane(farPlane), m_Fov(fov), m_AspectRatio(aspectRatio),
		m_ProjectionMatrix(glm::perspective(fov, aspectRatio, nearPlane, farPlane)), m_ViewMatrix(1.0f)
	{
		ATL_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float nearPlane, float farPlane, float fov, float aspectRatio)
	{
		ATL_PROFILE_FUNCTION();

		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
	}
}
