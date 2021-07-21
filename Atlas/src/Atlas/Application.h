#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Atlas {

	class ATLAS_API Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	

	};

	//To be defined in Client
	Application* CreateApplication();

}

