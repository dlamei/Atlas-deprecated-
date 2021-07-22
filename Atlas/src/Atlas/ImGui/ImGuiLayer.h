#pragma once

#include "Atlas/Layer.h"

namespace Atlas {

	class ATLAS_API ImGuiLayer:public Layer
	{

	private:
		float m_Time = 0.0f;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);


	};

}
