#include "atlpch.h"
#include "Application.h"

#include "Atlas/Events/ApplicationEvent.h"
#include "Atlas/Core/Log.h"

#include "Atlas/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>


namespace Atlas {

	Application* Application::s_Instance = nullptr;

	Application::Application(Application::AppProps props)
		: Application(props.m_Title, props.m_Width, props.m_Height, props.type) 
	{
	}

	Application::Application()
		: Application("Atlas Engine", 1200, 1200, EditorType::ATLAS2D) {}

	Application::Application(std::string title, uint32_t width, uint32_t height, EditorType type)
		: m_EditorType(type)
	{
		ATL_PROFILE_FUNCTION();

		ATL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ title, width, height }));
		m_Window->SetEventCallback(ATL_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init(m_EditorType);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_EditorLayer = new EditorLayer();
		PushOverlay(m_EditorLayer);
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ATL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(ATL_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		ATL_PROFILE_FUNCTION();

		while (m_Running)
		{
			ATL_PROFILE_SCOPE("Run Loop");

			//auto [MouseX, MouseY] = Input::GetMousePos();

			float time = (float)glfwGetTime(); // Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//m_EditorLayer->BindViewportFrameBuffer();
			if (!m_Minimized)
			{
				ATL_PROFILE_SCOPE("LayerStack on Update");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}
			//m_EditorLayer->UnbindViewportFrameBuffer();


			m_ImGuiLayer->Begin();
			m_EditorLayer->Begin();
			{
				ATL_PROFILE_SCOPE("LayerStack on ImGuiRender");
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_EditorLayer->End();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		ATL_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return true;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		ATL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		ATL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
