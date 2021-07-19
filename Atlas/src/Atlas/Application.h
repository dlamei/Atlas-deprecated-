#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Atlas {

	class ATLAS_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();


	};

	//To be defined in Client
	Application* CreateApplication();

}

