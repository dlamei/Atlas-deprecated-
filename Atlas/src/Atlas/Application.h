#pragma once

#include "Core.h"

#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Atlas/LayerStack.h"
#include "Atlas/ImGui/ImGuiLayer.h"

#include "Atlas/Renderer/Shader.h"
#include "Atlas/Renderer/Buffer.h"
#include "Atlas/Renderer/VertexArray.h"

#include "Atlas/Renderer/OrthographicCamera.h"


namespace Atlas {

	class ATLAS_API Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);

		static Application* s_Instance;


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

