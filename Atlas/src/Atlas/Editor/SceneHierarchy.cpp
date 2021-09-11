#include "atlpch.h"
#include "SceneHierarchy.h"

#include <imgui.h>

namespace Atlas {
	SceneHierarchy::SceneHierarchy(const Ref<Scene>& context)
		: m_Context(context)
	{
	}

	void SceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		for (ECS::Entity entity : m_Context->GetEntities())
		{
			TagComponent tag = m_Context->GetComponent<TagComponent>(entity);
			ImGui::Text(tag);
		}

		ImGui::End();
	}
}