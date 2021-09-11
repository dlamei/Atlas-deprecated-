#include "Sandbox3D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Atlas/Renderer/Mesh.h"

Sandbox3D::Sandbox3D()
	: Layer("Sandbox2D"), m_CameraController(1.0f) {}

void Sandbox3D::OnAttach()
{
	m_FrameBuffer = Atlas::FrameBuffer::Create({
			Atlas::Application::GetWindowWidth(),
			Atlas::Application::GetWindowHeight(),
			{ Atlas::FBTextureFormat::RGBA8, Atlas::FBTextureFormat::DEPTH24STENCIL8 }
		});

	Atlas::Ref<Atlas::Scene> scene = Atlas::Application::GetActiveScene();

	scene->SetActiveCamera(Atlas::PerspectiveCameraController(1.0f));

	Atlas::Ref<Atlas::Mesh> mesh = Atlas::Mesh::Create("assets/Models/Box.obj");
	mesh->AddTexture(Atlas::Texture2D::Create("assets/Textures/Box_Diffuse.png"), Atlas::Utils::TextureType::DIFFUSE);
	mesh->AddTexture(Atlas::Texture2D::Create("assets/Textures/Box_Specular.png"), Atlas::Utils::TextureType::SPECULAR);
	scene->AddMesh(mesh);

	mesh = Atlas::Mesh::Create("assets/Models/Hand.obj");
	mesh->SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), 20);
	mesh->SetTranslation(glm::vec3(0.0f, 0.0f, -5.0f));
	scene->AddMesh(mesh);


	scene->SetLight({ 0.0f, 3.0f, -2.0f });

	ECS::Entity entity = scene->CreateEntity("test Entity");
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Atlas::Timestep ts)
{
	ATL_PROFILE_FUNCTION();

	Atlas::Application::GetActiveScene()->GetActiveCamera().OnUpdate(ts);


	//glEnable(GL_DEPTH_TEST);
	//m_FrameBuffer->Bind();

	Atlas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Atlas::RenderCommand::Clear();
	Atlas::Renderer3D::DrawScene(Atlas::Application::GetActiveScene());

	//m_FrameBuffer->Unbind();
	//glDisable(GL_DEPTH_TEST);

	//Atlas::Renderer2D::DrawFrameBuffer(m_FrameBuffer->GetColorAttachmentRendererID());
}

void Sandbox3D::OnImGuiRender()
{
	ATL_PROFILE_FUNCTION();

	//ImGui::ShowDemoWindow();
	//ImGuiIO& io = ImGui::GetIO(); //(void)io;

	//ImGui::Begin("Settings");
	//ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, 1, 10, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
	//ImGui::SetWindowFontScale(1.8f);
	////ImGui::Checkbox("Smooth Shading", &m_Shading);
	//ImGui::Image((void*)(size_t)m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2(800, 800), ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::Image((void*)(size_t)m_FrameBuffer->GetDepthAttachmentRendererID(), ImVec2(800, 800), ImVec2(0, 1), ImVec2(1, 0));
	//m_Scene->GetMesh(0)->SetShading(m_Shading);
	////ImGui::End();
}

void Sandbox3D::OnEvent(Atlas::Event& e)
{
	Atlas::Application::GetActiveScene()->GetActiveCamera().OnEvent(e);

	if (e.GetEventType() == Atlas::EventType::WindowResize)
	{
		Atlas::WindowResizeEvent& resizeEvent = (Atlas::WindowResizeEvent&)e;
		m_FrameBuffer->Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
	}
}
