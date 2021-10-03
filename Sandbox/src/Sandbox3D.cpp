#include "Sandbox3D.h"

#include "ImGui/imgui.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Atlas/Renderer/Mesh.h"

using namespace Atlas;

Sandbox3D::Sandbox3D()
	: Layer("Sandbox2D"), m_CameraController(1.0f) {}

void Sandbox3D::OnAttach()
{
	Ref<Scene> scene = Application::GetActiveScene();

	scene->SetActiveCamera(PerspectiveCameraController(1.0f));

	//scene->LoadMesh("assets/Models/Dragon.obj");
	Mesh& mesh = scene->LoadMesh("assets/Models/Box.obj");
	mesh.AddTexture(Texture2D::Create("assets/Textures/Box_Diffuse.png"), Utils::TextureType::DIFFUSE);
	mesh.AddTexture(Texture2D::Create("assets/Textures/Box_Specular.png"), Utils::TextureType::SPECULAR);

	//Mesh& mesh = scene->LoadMesh("assets/Models/Backpack.obj");
	//mesh.AddTexture(Texture2D::Create("assets/Textures/Backpack_Diffuse.jpg", false), Utils::TextureType::DIFFUSE);
	//mesh.AddTexture(Texture2D::Create("assets/Textures/Backpack_Specular.jpg", false), Utils::TextureType::SPECULAR);

	ECS::Entity lightEntity = scene->CreateEntity("Point light");
	scene->CreateComponent<PointLightComponent>(lightEntity);
	scene->CreateComponent<TransformComponent>(lightEntity);
	
	ECS::Entity dirLight = scene->CreateEntity("Dir Light");
	scene->CreateComponent<DirLightComponent>(dirLight);
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Timestep ts)
{
	ATL_PROFILE_FUNCTION();

	//Application::GetActiveScene()->GetActiveCamera().OnUpdate(ts);

	//Renderer3D::DrawScene(Application::GetActiveScene());
}

void Sandbox3D::OnImGuiRender()
{
	ATL_PROFILE_FUNCTION();
}

void Sandbox3D::OnEvent(Event& e)
{
	//Application::GetActiveScene()->GetActiveCamera().OnEvent(e);

}
