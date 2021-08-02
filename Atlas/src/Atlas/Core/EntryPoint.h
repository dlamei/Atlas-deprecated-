#pragma once

#ifdef ATL_PLATFORM_WINDOWS

extern Atlas::Application* Atlas::CreateApplication();

int main(int argc, char** argv)
{
	Atlas::Log::Init();
	ATL_CORE_INFO("Welcome to Atlas engine!");

	auto app = Atlas::CreateApplication();
	app->Run();
	delete app;
}

#endif
