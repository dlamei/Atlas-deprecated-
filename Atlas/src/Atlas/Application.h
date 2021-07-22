#pragma once

#include "Core.h"

#include "Window.h"
#include "Atlas/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Atlas {

	class ATLAS_API Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


	};

	//To be defined in Client
	Application* CreateApplication();

}

