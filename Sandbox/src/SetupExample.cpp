/*
#include <Setup.h>
using namespace Atlas;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

glm::vec4 color = { 1.0f, 1.0f, 0.0f, 1.0f };

Application::AppProps Atlas::Settings()
{
	return { 1200, 1200, "test" };
}

void Atlas::Init()
{
	std::cout << "Init" << std::endl;
}


void Atlas::Draw()
{
	RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	RenderCommand::Clear();

	Renderer2D::DrawRect({ 0, 0 }, { 200, 200 }, color);

}


void Atlas::DrawGui()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("color:", glm::value_ptr(color));
	ImGui::End();
}

void Atlas::OnEvent(Atlas::Event& e)
{
	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		
	}
	
}
*/
