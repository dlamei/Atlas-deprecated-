#include <Atlas.h>
#include <Atlas/Core/EntryPoint.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox3D.h"
#include "Sandbox2D.h"

//TODO: Stats for 3D

class Sandbox : public Atlas::Application
{
public:
	Sandbox()
		: Application(AppProps({ "Atlas Engine", 2100, 1200, Atlas::EditorType::ATLAS2D }))
	{
		PushLayer(new Sandbox2D());
		Atlas::Application::GetWindow().SetVSync(true);
	}

	~Sandbox()
	{

	}
};

Atlas::Application* Atlas::CreateApplication()
{
	return new Sandbox();
}
