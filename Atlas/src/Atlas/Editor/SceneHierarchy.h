#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Scene/Scene.h"

namespace Atlas {

	class SceneHierarchy
	{
	private:
		Ref<Scene> m_Context;

	public:
		SceneHierarchy() = default;
		SceneHierarchy(const Ref<Scene>& context);

		inline void SetContext(const Ref<Scene>& context) { m_Context = context; }

		void OnImGuiRender();

	};

}
