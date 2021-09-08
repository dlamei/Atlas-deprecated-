#pragma once

#include "Atlas/Core/Layer.h"

#include "Atlas/Renderer/FrameBuffer.h"
#include "Atlas/Events/Event.h"

#include <memory>
#include <glm/glm.hpp>

namespace Atlas {

	class EditorLayer : public Layer
	{
	private:
		Ref<FrameBuffer> m_ViewportFrameBuffer;
		glm::vec2 m_ViewportSize;
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
		inline glm::vec2& GetViewportSize() { return m_ViewportSize; }

	};

}
