#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Events/Event.h"

#include "Atlas/Core/Timestep.h"

namespace Atlas {

	class Layer
	{
	protected:
		std::string m_DebugName;

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};

}
