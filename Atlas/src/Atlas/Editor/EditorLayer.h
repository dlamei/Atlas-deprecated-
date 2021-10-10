#pragma once

#include "Atlas/Core/Layer.h"

#include "Atlas/Renderer/FrameBuffer.h"
#include "Atlas/Events/Event.h"
#include "Atlas/Scene/Scene.h"

#include <memory>
#include <glm/glm.hpp>

#include "SceneHierarchy.h"
#include "Editor.h"
#include "AtlasLog.h"


namespace Atlas {

	class EditorLayer : public Layer
	{
	private:
		Ref<FrameBuffer> m_ViewportFrameBuffer;
		Ref<FrameBuffer> m_PostProcessingFrameBuffer;
		Ref<Scene> m_ActiveScene;
		bool m_ShowEditor = true;

		Ref<Editor> m_Editor;
		SceneHierarchy m_SceneHierarchy;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
		glm::vec2 m_ViewportBounds[2];
		float m_GlobalFontScale = 1.8f;

		float m_OutlineThickness = 0.08f;
		float m_Gamma = 2.2f;

		ECS::Entity m_HoveredEntity = ECS::null;
		ECS::Entity m_SelectedEntity = ECS::null;

	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		inline void ToggleEditor(bool toggle) { m_ShowEditor = toggle; }

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

		void Begin();
		void End();

		inline Ref<FrameBuffer>& GetFrameBuffer() { return m_ViewportFrameBuffer; }
		inline Ref<Scene>& GetActiveScene() { return m_ActiveScene; }
		inline glm::vec2& GetViewportSize() { return m_Editor->GetViewportSize(); }

		void BindViewportFrameBuffer();
		void UnbindViewportFrameBuffer();

	};

}
