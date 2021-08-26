#pragma once

#include "Atlas/Core/Layer.h"

#include "Atlas/Events/KeyEvent.h"
#include "Atlas/Events/MouseEvent.h"
#include "Atlas/Events/ApplicationEvent.h"

namespace Atlas {

	class ImGuiLayer : public Layer
	{

	private:
		float m_Time = 0.0f;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	};

}
