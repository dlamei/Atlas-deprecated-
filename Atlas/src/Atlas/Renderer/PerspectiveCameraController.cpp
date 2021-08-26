#include "atlpch.h"
#include "PerspectiveCameraController.h"
#include "Atlas/Core/Input.h"
#include "Atlas/Core/KeyCodes.h"

#include "Atlas/Core/Application.h"

namespace Atlas
{
	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!Atlas::Input::IsMouseButtonPressed(0)) return false;

		if (firstMouseMove)
		{
			m_PMouseX = e.GetX();
			m_PMouseY = e.GetY();
			firstMouseMove = false;
		}

		float xOffset = e.GetX() - m_PMouseX;
		float yOffset = m_PMouseY - e.GetY();
		m_PMouseX = e.GetX();
		m_PMouseY = e.GetY();

		m_Yaw += xOffset * m_CamearSensitivity;
		m_Pitch += yOffset * m_CamearSensitivity;

		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		m_CameraDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraDirection.y = sin(glm::radians(m_Pitch));
		m_CameraDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera.SetFrontVec(glm::normalize(m_CameraDirection));

		return false;
	}

	bool PerspectiveCameraController::OnMousePressed(MouseButtonPressedEvent& e)
	{
		Scope<Window>& window = Application::GetWindowScope();
		window->CaptureMouse(true);
		return false;
	}

	bool PerspectiveCameraController::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		Scope<Window>& window = Application::GetWindowScope();
		window->CaptureMouse(false);
		firstMouseMove = true;
		return false;
	}

	PerspectiveCameraController::PerspectiveCameraController(float aspecRatio)
		: m_Camera(0.1f, 1000.0f, glm::radians(m_Fov), aspecRatio), m_AspectRatio(aspecRatio)
	{
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		bool updated = false;

		if (Input::IsKeyPressed(ATL_KEY_S))
		{
			m_CameraPosition -= m_Camera.GetFrontVec() * (m_CameraMoveSpeed * ts);
			updated = true;
		}
		if (Input::IsKeyPressed(ATL_KEY_W))
		{
			m_CameraPosition += m_Camera.GetFrontVec() * (m_CameraMoveSpeed * ts);
			updated = true;
		}
		if (Input::IsKeyPressed(ATL_KEY_A))
		{
			m_CameraPosition -= m_Camera.GetRightVec() * (m_CameraMoveSpeed * ts);
			updated = true;
		}
		if (Input::IsKeyPressed(ATL_KEY_D))
		{
			m_CameraPosition += m_Camera.GetRightVec() * (m_CameraMoveSpeed * ts);
			updated = true;
		}


		if (Input::IsKeyPressed(ATL_KEY_SPACE))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
			updated = true;
		}
		if (Input::IsKeyPressed(ATL_KEY_LEFT_CONTROL))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
			updated = true;
		}

		if (updated) m_Camera.SetPositionVec(m_CameraPosition);
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(ATL_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ATL_BIND_EVENT_FN(PerspectiveCameraController::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(ATL_BIND_EVENT_FN(PerspectiveCameraController::OnMouseReleased));
	}
}