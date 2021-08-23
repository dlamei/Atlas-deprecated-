#include "Sandbox2D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Atlas/Renderer/Mesh.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1.0f) {}

void Sandbox2D::OnAttach()
{
	m_Texture1 = Atlas::Texture2D::Create("assets/Textures/cat.png");
	m_Texture2 = Atlas::Texture2D::Create("assets/Textures/colorgrid.png");

	Atlas::FrameBufferSpecs fbSpecs = {
		Atlas::Application::GetWindowWidth(),
		Atlas::Application::GetWindowHeight(),
		{
			Atlas::FBTextureFormat::RGBA8
		}
	};
	m_FrameBuffer = Atlas::FrameBuffer::Create(fbSpecs);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Atlas::Timestep ts)
{
	ATL_PROFILE_FUNCTION();


	//m_CameraController.OnUpdate(ts);

	//Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	//Atlas::RenderCommand::Clear();

	//Atlas::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//Atlas::Renderer2D::ResetStats();
	//int count = 10;
	//for (int i = 0; i < count; i++)
	//{
	//	for (int j = 0; j < count; j++)
	//	{
	//		Atlas::Renderer2D::SetFill({ (float)i / count, (float)j / count, (float)(i + j) / (2 * count) });
	//		Atlas::Renderer2D::DrawRect({ i, j }, { 0.8, 0.8 });
	//	}
	//}
	//Atlas::Renderer2D::DrawRect({ -1.0f, -1.0f }, { 0.8, 0.8 }, m_Texture1);
	//Atlas::Renderer2D::DrawTri({ 0.0f, 0.0f }, { 5.0f, 1.0f }, { 4.0f, -1.0f }, m_Color);

	//Atlas::Renderer2D::EndScene();
	//m_FrameBuffer->Unbind();

	//m_ColorAttachment = m_FrameBuffer->GetColorAttachmentRendererID(0);
	//m_DepthAttachment = m_FrameBuffer->GetDepthAttachmentRendererID();
	//Atlas::Renderer2D::DrawFrameBuffer(m_ColorAttachment);


	//m_FrameBuffer->Bind();
	Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Atlas::RenderCommand::Clear();

	Atlas::Renderer3D::BeginScene(m_Camera);

	Atlas::Renderer3D::EndScene();
	//m_FrameBuffer->Unbind();

	//Atlas::Renderer2D::DrawFrameBuffer(m_FrameBuffer->GetColorAttachmentRendererID(0));
}

void Sandbox2D::OnImGuiRender()
{
	ATL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::SetWindowFontScale(1.8f);
	//ImGui::Image((void*)(size_t)m_ColorAttachment, ImVec2(800, 800));
	//ImGui::Image((void*)(size_t)m_DepthAttachment, ImVec2(800, 800));
	//ImGui::SetWindowFontScale(1.8f);

	//auto stats = Atlas::Renderer2D::GetStats();
	//ImGui::Text("Renderer2D Stats:");
	//ImGui::Text("Draw calls: %d", stats.DrawCalls);
	//ImGui::Text("Quad count: %d", stats.QuadCount);
	//ImGui::Text("Tri count: %d", stats.TriCount);
	//ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
	//ImGui::Text("Index count: %d", stats.GetTotalIndexCount());

	//ImGui::ColorEdit3("color", glm::value_ptr(m_Color));
	ImGui::End();

}

void Sandbox2D::OnEvent(Atlas::Event& e)
{
	m_CameraController.OnEvent(e);

	if (e.GetEventType() == Atlas::EventType::WindowResize)
	{
		Atlas::WindowResizeEvent& resizeEvent = (Atlas::WindowResizeEvent&)e;
		m_FrameBuffer->Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
	}
}
