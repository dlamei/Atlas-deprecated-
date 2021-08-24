#pragma once

#include "PerspectiveCamera.h"
#include "Atlas/Core/Timestep.h"

#include "Atlas/Events/ApplicationEvent.h"
#include "Atlas/Events/MouseEvent.h"

namespace Atlas
{
	class PerspectiveCameraController
	{
	private:
		float m_AspectRatio;
		float m_CameraMoveSpeed = 5.0f, m_CamearSensitivity = 0.2f;
		float m_Yaw = -90.0f, m_Pitch = 0.0f;
		float m_Fov = 90;

		float m_PMouseX = 0.0f, m_PMouseY = 0.0f;
		bool firstMouseMove = true;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraDirection = { 0.0f, 0.0f, 0.0f };

		PerspectiveCamera m_Camera;

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);

	public:
		PerspectiveCameraController(float aspecRatio);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline void SetFov(float fov) { m_Fov = fov; m_Camera.SetFov(fov); }
		inline float GetFov() { return m_Fov; }

		inline const PerspectiveCamera& GetCamera() { return m_Camera; }
	};
}

