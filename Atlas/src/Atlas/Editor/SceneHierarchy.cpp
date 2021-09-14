#include "atlpch.h"
#include "SceneHierarchy.h"

#include "Atlas/ImGui/CustomImGui.h"
#include "Atlas/ImGui/AtlasTheme.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

	void DrawVec3(const char* label, float* values, float speed = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f)
	{

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 10, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 1.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };
		buttonSize.x *= io.FontGlobalScale;
		buttonSize.y *= io.FontGlobalScale;

		Atlas::PushButtonStyle(Atlas::ATL_RED_COL);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values[0] = resetValue;
		ImGui::PopFont();
		Atlas::PopButtonStyle();

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::DragFloat("##X", &values[0], speed, 0.0f, 0.0f, "%.2f", 0, ImDrawFlags_RoundCornersTop);
		ImGui::PopStyleVar();

		Atlas::PushButtonStyle(Atlas::ATL_GREEN_COL);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values[1] = resetValue;
		ImGui::PopFont();
		Atlas::PopButtonStyle();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values[1], speed, 0.0f, 0.0f, "%.2f");

		Atlas::PushButtonStyle(Atlas::ATL_BLUE_COL);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values[2] = resetValue;
		ImGui::PopFont();
		Atlas::PopButtonStyle();

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::DragFloat("##Z", &values[2], speed, 0.0f, 0.0f, "%.2f", 0, ImDrawFlags_RoundCornersBottom);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

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
					DrawVec3("Translation", glm::value_ptr(component.Translation), 0.01f);
					glm::vec3 rotation = glm::degrees(component.Rotation);
					DrawVec3("Rotation", glm::value_ptr(rotation));
					component.Rotation = glm::radians(rotation);
					DrawVec3("Scale", glm::value_ptr(component.Scale), 0.01f, 1.0f);
				});
		}
	}


}