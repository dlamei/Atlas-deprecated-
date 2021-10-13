#pragma once

#include "OrthographicCamera.h"
#include "Atlas/Core/Timestep.h"

#include "Atlas/Events/ApplicationEvent.h"
#include "Atlas/Events/MouseEvent.h"

namespace Atlas {

	class OrthographicCameraController 
	{
	private:
		float m_AspectRatio = 0.5f;
		float m_ZoomLevel = 1.0f;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

		OrthographicCamera m_Camera;

		bool OnMouseScrolled(MouseScrolledEvent& e);

	public:
		OrthographicCameraController(float aspectRatio, bool roation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	};

}
