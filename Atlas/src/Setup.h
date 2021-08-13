#pragma once
#include <Atlas.h>
#include <Atlas/Core/EntryPoint.h>


namespace Atlas
{
	extern Atlas::Application::AppProps Settings();
	extern void Init();
	extern void Draw();
	extern void DrawGui();
	extern void OnEvent(Atlas::Event& e);
}

class AppLayer : public Atlas::Layer
{
private:
	Atlas::OrthographicCamera m_Camera;

public:
	AppLayer()
		: Layer("Atlas"), m_Camera(0.0f, Atlas::Application::GetWindowWidth(), Atlas::Application::GetWindowHeight(), 0.0f) {};

	virtual void OnAttach() override {
		Atlas::Init(); 
	};

	void OnUpdate(Atlas::Timestep ts) override {
		ATL_PROFILE_FUNCTION();

		Atlas::Renderer2D::BeginScene(m_Camera);
		Atlas::Draw();  
		Atlas::Renderer2D::EndScene();
	}

	virtual void OnImGuiRender() override { Atlas::DrawGui(); }
	void OnEvent(Atlas::Event& e) override { Atlas::OnEvent(e); }

};

class App : public Atlas::Application
{

public:
	App()
		: Application(Atlas::Settings())
	{
		PushLayer(new AppLayer());
	}

	~App()
	{

	}

	void SetSize()
	{

	}
};

Atlas::Application* Atlas::CreateApplication()
{
	return new App();
}

using namespace Atlas;
