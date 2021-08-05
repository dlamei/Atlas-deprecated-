#include "Sandbox2D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Atlas::Texture2D::Create("assets/Textures/cat.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Atlas::Timestep ts)
{
	ATL_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		ATL_PROFILE_SCOPE("Renderer Prep");
		Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Atlas::RenderCommand::Clear();
	}

	{
		ATL_PROFILE_SCOPE("Renderer Draw");
		Atlas::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Atlas::Renderer2D::DrawRect({ 1.0f, 1.0f }, { 0.8f, 0.6f }, m_Color);
		Atlas::Renderer2D::DrawRect({ -0.4f, -0.5f }, { 1.0f, 1.0f }, m_Texture);
		Atlas::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ATL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::SetWindowFontScale(1.8f);
	ImGui::ColorEdit3("color", glm::value_ptr(m_Color));
	ImGui::End();

}

void Sandbox2D::OnEvent(Atlas::Event& e)
{
	m_CameraController.OnEvent(e);
}
