#pragma once

#include "Atlas/Core/Layer.h"

#include "Atlas/Renderer/FrameBuffer.h"
#include "Atlas/Events/Event.h"
#include "Atlas/Scene/Scene.h"

#include <memory>
#include <glm/glm.hpp>

#include "SceneHierarchy.h"
#include "AtlasLog.h"


namespace Atlas {

	class EditorLayer : public Layer
	{
	private:
		Ref<FrameBuffer> m_ViewportFrameBuffer;
		Ref<Scene> m_ActiveScene;

		SceneHierarchy m_SceneHierarchy;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
		float m_GlobalFontScale = 1.0f;

	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

		void Begin();
		void End();

		inline Ref<FrameBuffer>& GetFrameBuffer() { return m_ViewportFrameBuffer; }
		inline Ref<Scene>& GetActiveScene() { return m_ActiveScene; }
		inline glm::vec2& GetViewportSize() { return m_ViewportSize; }

	};

}
