#pragma once

#include "Core.h"

#include "Window.h"
#include "Atlas/Events/Event.h"
#include "Atlas/Events/ApplicationEvent.h"

#include "Atlas/Core/LayerStack.h"
#include "Atlas/ImGui/ImGuiLayer.h"

#include "Atlas/Core/Timestep.h"

namespace Atlas {


	class Application
	{

	public:
		struct AppProps
		{
			int m_Width, m_Height;
			std::string m_Title;
		};


	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application* s_Instance;
	
		float m_LastFrameTime;

	public:
		float MouseX, MouseY;

		Application();
		Application(AppProps props);
		Application(std::string title, uint32_t width, uint32_t height);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		static inline uint32_t GetWindowHeight() { return s_Instance->m_Window->GetHeight(); }
		static inline uint32_t GetWindowWidth() { return s_Instance->m_Window->GetWidth(); }
		static inline Application& Get() { return *s_Instance; }

	};

	//To be defined in Client
	Application* CreateApplication();

}

