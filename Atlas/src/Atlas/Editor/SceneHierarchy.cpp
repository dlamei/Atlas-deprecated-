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
		ImGui::Begin("Inspector");

		//for (ECS::Entity entity : m_Context->GetEntities())
		//{
		//	if (m_Context->HasComponent<TagComponent>(entity)) DrawEntityNode(entity);
		//}
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

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity [X]") || ImGui::IsKeyPressed('X')) entityDeleted = true;

			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (m_SelectedEntity == entity)
			{
				m_SelectedEntity = ECS::null;
			}
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
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = { buffer };
			}
		}
	}

}