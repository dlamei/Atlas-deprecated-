#include "atlpch.h"
#include "Application.h"

#include "Atlas/Events/ApplicationEvent.h"
#include "Atlas/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Atlas {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Window(std::unique_ptr<Window>(Window::Create()))
	{
		ATL_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;
		m_Window->SetEventCallback(ATL_BIND_EVENT_FN(Application::OnEvent));
	}
	
	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ATL_BIND_EVENT_FN(Application::OnWindowClose));

		//ATL_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running) 
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			auto [x, y] = Input::GetMousePos();
			ATL_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

}
