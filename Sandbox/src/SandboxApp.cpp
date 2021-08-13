#include <Atlas.h>
#include <Atlas/Core/EntryPoint.h>

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class Sandbox : public Atlas::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
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
