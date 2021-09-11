#include <Atlas.h>
#include <Atlas/Core/EntryPoint.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox3D.h"

//TODO: Stats for 3D

class Sandbox : public Atlas::Application
{
public:
	Sandbox()
		: Application("test", 1200, 1200)
	{
		PushLayer(new Sandbox3D());
		Atlas::Application::GetWindow().SetVSync(false);
	}

	~Sandbox()
	{

	}
};

Atlas::Application* Atlas::CreateApplication()
{
	return new Sandbox();
}
