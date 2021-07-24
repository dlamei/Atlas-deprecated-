#pragma once

#include "Atlas/Layer.h"

#include "Atlas/Events/KeyEvent.h"
#include "Atlas/Events/MouseEvent.h"
#include "Atlas/Events/ApplicationEvent.h"

namespace Atlas {

	class ATLAS_API ImGuiLayer:public Layer
	{

	private:
		float m_Time = 0.0f;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	};

}
