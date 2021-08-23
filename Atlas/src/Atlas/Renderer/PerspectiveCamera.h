#pragma once

#include <glm/glm.hpp>

namespace Atlas
{
	class PerspectiveCamera
	{
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		float m_NearPlane, m_FarPlane, m_Fov, m_AspectRatio;

		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

	public:
		PerspectiveCamera(float nearPlane, float farPlane, float fov, float aspectRatio);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetProjection(float nearPlane, float farPlane, float fov, float aspectRatio);
		void SetFov(float fov) { m_Fov = fov; RecalculateProjectionMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	};
}

