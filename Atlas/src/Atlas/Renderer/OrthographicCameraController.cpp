#include "atlpch.h"
#include "OrthographicCameraController.h";
#include "Atlas/Core/Input.h"
#include "Atlas/Core/KeyCodes.h"

namespace Atlas {
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float) e.GetWidth() / (float) e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Roation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(ATL_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(ATL_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(ATL_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(ATL_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;
		}

		if (m_Roation)
		{
			if (Input::IsKeyPressed(ATL_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			}
			else if (Input::IsKeyPressed(ATL_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			}

			if (m_CameraRotation > 180.0f)
			{
				m_CameraRotation -= 360.0f;
			}
			else if (m_CameraRotation <= -180.0f)
			{
				m_CameraRotation += 360.0f;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 2;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ATL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ATL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized)); }
}