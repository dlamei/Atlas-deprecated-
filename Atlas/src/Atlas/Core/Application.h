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


	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application* s_Instance;
	
		float m_LastFrameTime;


	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }

	};

	//To be defined in Client
	Application* CreateApplication();

}

