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
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = glm::normalize(glm::cross(m_Front, m_Up));
		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

		float m_NearPlane, m_FarPlane, m_Fov, m_AspectRatio;

		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

	public:
		PerspectiveCamera(float nearPlane, float farPlane, float fov, float aspectRatio);

		void SetPositionVec(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetFrontVec(const glm::vec3& direction);
		void SetProjection(float nearPlane, float farPlane, float fov, float aspectRatio);
		void SetFov(float fov) { m_Fov = fov; RecalculateProjectionMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const glm::vec3& GetUpVec() const { return m_Up; }
		inline const glm::vec3& GetFrontVec() const { return m_Front; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3 GetRightVec() const { return m_Right; }

	};
}

