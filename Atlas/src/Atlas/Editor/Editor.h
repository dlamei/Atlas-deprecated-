#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Scene/Scene.h"

#include "SceneHierarchy.h"

namespace Atlas
{
	enum class EditorType
	{
		ATLAS2D = 0,
		ATLAS3D
	};

	class Editor
	{
	protected:
		Ref<FrameBuffer> m_ViewportFrameBuffer;
		Ref<FrameBuffer> m_PostProcessingFrameBuffer;
		glm::vec2 m_ViewportBounds[2] = { glm::vec2(0) };
		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		bool m_ShowViewport = true;

	public:
		Editor(Ref<FrameBuffer> viewportFB, Ref<FrameBuffer> postprocessingFB)
			: m_ViewportFrameBuffer(viewportFB), m_PostProcessingFrameBuffer(postprocessingFB) {}

		glm::vec2& GetViewportSize() { return m_ViewportSize; }

		void ToggleViewport(bool toggle) { m_ShowViewport = toggle; }

		virtual void Render() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;
	};

	class Editor3D : public Editor
	{
	private:
		Ref<Scene> m_ActiveScene;

		float m_OutlineThickness = 0.08f;
		float m_Gamma = 2.2f;

		ECS::Entity m_HoveredEntity = ECS::null;
		ECS::Entity m_SelectedEntity = ECS::null;

		SceneHierarchy m_SceneHierarchy;

	public:
		Editor3D(Ref<FrameBuffer> viewportFB, Ref<FrameBuffer> postprocessingFB, Ref<Scene> scene);

		virtual void Render() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

	};

	class Editor2D : public Editor
	{
	public:

		Editor2D(Ref<FrameBuffer> viewportFB, Ref<FrameBuffer> postprocessingFB);

		virtual void Render() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

	};
}
