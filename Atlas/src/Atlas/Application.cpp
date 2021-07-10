#include "Application.h"

#include "Atlas/Events/ApplicationEvent.h"

#include "Atlas/Log.h"

namespace Atlas {

	Application::Application()
	{

	}
	
	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent event(1280, 720);

		if (event.IsInCategory(EventCategoryApplication))
		{
			ATL_ERROR(event);
		}
		if (event.IsInCategory(EventCategoryInput))
		{
			ATL_TRACE(event.ToString());
		}

		while (true);
	}
}
