#include "atlpch.h"
#include "SceneHierarchy.h"

#include "Atlas/ImGui/CustomImGui.h"
#include "Atlas/ImGui/AtlasTheme.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <charconv>

//TODO:: TEMP
#include <Atlas/Core/Application.h>

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
	void SceneHierarchy::DrawComponent(const char* name, ECS::Entity entity, Function function)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed;
		if (m_Context->HasComponent<T>(entity))
		{
			T& component = m_Context->GetComponent<T>(entity);

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 10, 10 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::PopStyleVar();

			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name);


			ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("Component settings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("Component settings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (opened)
			{
				function(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_Context->RemoveComponent<T>(entity);
			}

			ImGui::NewLine();
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = ECS::null;
			auto& callBackFn = Application::GetEventCallback();
			callBackFn(EntitySelectedEvent(m_SelectedEntity));
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty")) m_Context->CreateEntity("Empty");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectedEntity != ECS::null)
		{
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");
			}

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::Selectable("Transform Component")) m_Context->CreateComponentEx<TransformComponent>(m_SelectedEntity);
				if (ImGui::Selectable("Mesh Component")) m_Context->CreateComponentEx<MeshComponent>(m_SelectedEntity);
				if (ImGui::Selectable("Point light Component")) m_Context->CreateComponentEx<PointLightComponent>(m_SelectedEntity);
				if (ImGui::Selectable("Directional light Component")) m_Context->CreateComponentEx<DirLightComponent>(m_SelectedEntity);
				ImGui::EndPopup();
			}


			DrawComponents(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchy::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::EntitySelected)
		{
			m_SelectedEntity = ((EntitySelectedEvent&)e).GetEntity();
		}
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
			auto& callBackFn = Application::GetEventCallback();
			callBackFn(EntitySelectedEvent(m_SelectedEntity));
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
				auto& callBackFn = Application::GetEventCallback();
				callBackFn(EntitySelectedEvent(m_SelectedEntity));
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

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = { buffer };
			}
			ImGui::PopItemWidth();

		}

		DrawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& component)
			{
				DrawVec3("Translation", glm::value_ptr(component.Translation), 0.01f);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3("Rotation", glm::value_ptr(rotation));
				component.Rotation = glm::radians(rotation);
				DrawVec3("Scale", glm::value_ptr(component.Scale), 0.01f, 1.0f);
				auto& camera = m_Context->GetActiveCamera();
			});

		DrawComponent<PointLightComponent>("Point light", entity, [](PointLightComponent& component)
			{
				ImGui::ColorEdit4("Ambient", glm::value_ptr(component.Ambient), ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Diffuse", glm::value_ptr(component.Diffuse), ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Specular", glm::value_ptr(component.Specular), ImGuiColorEditFlags_NoInputs);
				ImGui::Text("Falloff");
				ImGui::DragFloat("Constant", &component.Constant, 0.1f);
				ImGui::DragFloat("Linear", &component.Linear, 0.1f);
				ImGui::DragFloat("Quadratic", &component.Quadratic, 0.1f);
			});

		DrawComponent<DirLightComponent>("Directional light", entity, [](DirLightComponent& component)
			{
				ImGui::DragFloat3("Direction", glm::value_ptr(component.Direction), 0.05f, -1.0f, 1.0f);
				ImGui::ColorEdit4("Ambient", glm::value_ptr(component.Ambient), ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Diffuse", glm::value_ptr(component.Diffuse), ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Specular", glm::value_ptr(component.Specular), ImGuiColorEditFlags_NoInputs);
			});

		DrawComponent<MeshComponent>("Mesh", entity, [](MeshComponent& component)
			{
				ImGui::Text(component.Mesh->GetFilePath().c_str());
				char buffer[128];
				memset(&buffer[0], 0, 128);

				std::to_chars(&buffer[0], &buffer[0] + 128, component.Mesh->GetTriangleCount());
				ImGui::Text("Triangles: ");
				ImGui::SameLine();
				ImGui::Text(buffer);

				std::to_chars(&buffer[0], &buffer[0] + 128, 3 * component.Mesh->GetTriangleCount());
				ImGui::Text("Vertecies: ");
				ImGui::SameLine();
				ImGui::Text(buffer);

				ImGui::DragFloat("Shininess", &component.Shininess, 0.1f, 0.0f, 32.0f);

				bool shading = component.Mesh->GetShading();
				std::string shadingButton = "Shading: " + (std::string) (shading ? "Smooth" : "Flat");
				if (ImGui::Button(shadingButton.c_str()))
				{
					ImGui::OpenPopup("Shading Settings");
				}

				if (ImGui::BeginPopup("Shading Settings"))
				{
					if (ImGui::MenuItem("Smooth", "", shading)) { shading = true; }
					if (ImGui::MenuItem("Flat", "", !shading)) { shading = false; }
					component.Mesh->SetShading(shading);
					ImGui::EndPopup();
				}

				Utils::DisplayMode displayMode = component.Mesh->GetDisplayMode();
				std::string displayButton = "Display: " + Utils::DisplayEnumToString(displayMode);
				if (ImGui::Button(displayButton.c_str()))
				{
					ImGui::OpenPopup("Display Settings");
				}

				if (ImGui::BeginPopup("Display Settings"))
				{
					for (int i = 0; i < (int)Utils::DisplayMode::NONE; i++)
					{
						Utils::DisplayMode type = static_cast<Utils::DisplayMode>(i);
						if (ImGui::MenuItem(Utils::DisplayEnumToString(type).c_str(), "", displayMode == type)) { displayMode = type; }
					}

					component.Mesh->SetDisplayMode(displayMode);
					ImGui::EndPopup();
				}

				ImGui::Checkbox("Hide", &component.Hide);

			});
	}


}