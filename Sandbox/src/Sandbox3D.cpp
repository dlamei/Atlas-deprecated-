#include "Sandbox3D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Atlas/Renderer/Mesh.h"

using namespace Atlas;

Sandbox3D::Sandbox3D()
	: Layer("Sandbox2D"), m_CameraController(1.0f) {}

void Sandbox3D::OnAttach()
{
	m_FrameBuffer = FrameBuffer::Create({
			Application::GetWindowWidth(),
			Application::GetWindowHeight(),
			{ FBTextureFormat::RGBA8, FBTextureFormat::DEPTH24STENCIL8 }
		});

	Ref<Scene> scene = Application::GetActiveScene();

	scene->SetActiveCamera(PerspectiveCameraController(1.0f));

	ECS::Entity entity = scene->CreateEntity();
	auto& boxComponent = scene->CreateComponent<MeshComponent>(entity, "assets/Models/Box.obj");
	scene->CreateComponent<int>(entity, 34);
	boxComponent.Mesh->AddTexture(Texture2D::Create("assets/Textures/Box_Diffuse.png"), Utils::TextureType::DIFFUSE);
	boxComponent.Mesh->AddTexture(Texture2D::Create("assets/Textures/Box_Specular.png"), Utils::TextureType::SPECULAR);

	entity = scene->CreateEntity();
	auto& handComponent = scene->CreateComponent<MeshComponent>(entity, "assets/Models/Hand.obj");
	handComponent.Mesh->SetTranslation(glm::vec3(0.0f, 0.0f, -5.0f));

	scene->CreateEntity("Test");

	scene->SetLight({ 0.0f, 3.0f, -2.0f });

}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Timestep ts)
{
	ATL_PROFILE_FUNCTION();

	Application::GetActiveScene()->GetActiveCamera().OnUpdate(ts);


	//glEnable(GL_DEPTH_TEST);
	//m_FrameBuffer->Bind();

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();
	Renderer3D::DrawScene(Application::GetActiveScene());

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

void Sandbox3D::OnEvent(Event& e)
{
	Application::GetActiveScene()->GetActiveCamera().OnEvent(e);

	if (e.GetEventType() == EventType::WindowResize)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		m_FrameBuffer->Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
	}
}
