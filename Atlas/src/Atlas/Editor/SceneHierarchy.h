#pragma once

#include "Atlas/Core/Core.h"
#include "Atlas/Scene/Scene.h"

namespace Atlas {

	class SceneHierarchy
	{
	private:
		Ref<Scene> m_Context;
		ECS::Entity m_SelectedEntity = ECS::null;

		template<typename T, typename Function>
		void DrawComponent(const char* name, ECS::Entity entity, Function function);

	public:
		SceneHierarchy() = default;
		SceneHierarchy(const Ref<Scene>& context);

		inline void SetContext(const Ref<Scene>& context) { m_Context = context; }

		void OnImGuiRender();
		void DrawEntityNode(ECS::Entity entity);
		void DrawComponents(ECS::Entity entity);

		void SetSelectedEntity(ECS::Entity entity) { m_SelectedEntity = entity; }

		ECS::Entity GetSelectedEntity() { return m_SelectedEntity; }

	};


}
