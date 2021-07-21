#include "atlpch.h"
#include "Application.h"

#include "Atlas/Events/ApplicationEvent.h"
#include "Atlas/Log.h"

#include <GLFW/glfw3.h>

namespace Atlas {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
		: m_Window(std::unique_ptr<Window>(Window::Create())) 
	{
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	
	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		ATL_CORE_INFO("{0}", e);
	}

	void Application::Run()
	{
		while (m_Running) 
		{
			m_Window->OnUpdate();
		}
	}
}
