#include "atlpch.h"
#include "SceneHierarchy.h"

#include "AtlasImGui.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

	template<typename T, typename Function>
	inline void SceneHierarchy::DrawComponent(const char* name, ECS::Entity entity, Function function)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (m_Context->HasComponent<T>(entity))
		{
			T& component = m_Context->GetComponent<T>(entity);
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name);

			if (opened)
			{
				function(component);
				ImGui::TreePop();
			}
		}
	}

	SceneHierarchy::SceneHierarchy(const Ref<Scene>& context)
		: m_Context(context)
	{
	}

	void SceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Inspector");

		auto& entities = m_Context->GetEntities();
		std::for_each(entities.begin(), entities.end(), [&](ECS::Entity entity)
			{
				DrawEntityNode(entity);
			});

		m_Context->RemoveTaggedEntites();

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) m_SelectedEntity = ECS::null;

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty")) m_Context->CreateEntity("Empty");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity != ECS::null)
		{
			DrawComponents(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchy::DrawEntityNode(ECS::Entity entity)
	{
		auto& tag = m_Context->GetComponent<TagComponent>(entity);

		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((void*)(size_t)entity, flags, tag);

		if (ImGui::IsItemClicked()) m_SelectedEntity = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity [X]") || ImGui::IsKeyPressed('X')) entityDeleted = true;

			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (m_SelectedEntity == entity) m_SelectedEntity = ECS::null;
			m_Context->TagToRemove(entity);
		}
	}

	void SceneHierarchy::DrawComponents(ECS::Entity entity)
	{
		if (m_Context->HasComponent<TagComponent>(entity))
		{
			TagComponent& tag = m_Context->GetComponent<TagComponent>(entity);
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag, sizeof(buffer));

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = { buffer };
			}
			ImGui::PopItemWidth();

			DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
				{
					AtlasImGui::DrawVec3("Translation", glm::value_ptr(component.Translation));
					glm::vec3 rotation = glm::degrees(component.Rotation);
					AtlasImGui::DrawVec3("Rotation", glm::value_ptr(rotation));
					component.Rotation = glm::radians(rotation);
					AtlasImGui::DrawVec3("Scale", glm::value_ptr(component.Scale), 1.0f);
				});
		}
	}
}