#include "Sandbox2D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1.0f)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Atlas::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Atlas::RenderCommand::Clear();

	Atlas::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Atlas::Renderer2D::DrawRect({ 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Color);

	Atlas::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::SetWindowFontScale(1.8f);
	ImGui::ColorEdit3("color", glm::value_ptr(m_Color));
	ImGui::End();

}

void Sandbox2D::OnEvent(Atlas::Event& e)
{
	m_CameraController.OnEvent(e);
}
