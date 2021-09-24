#include "atlpch.h"
#include "EditorLayer.h"

#include "Atlas/ImGui/AtlasTheme.h"
#include "Atlas/Core/Core.h"
#include "Atlas/Math/Math.h"

#include "imgui.h"
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Atlas {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		ATL_PROFILE_FUNCTION();

		Atlas::SetAtlasTheme();

		m_ViewportFrameBuffer = FrameBuffer::Create({
				(uint32_t)m_ViewportSize.x,
				(uint32_t)m_ViewportSize.y,
				{ FBTextureFormat::RGBA8, FBTextureFormat::DEPTH24STENCIL8 }
			});

		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchy.SetContext(m_ActiveScene);

	}

	void EditorLayer::OnDetach()
	{
	}

	ImGuizmo::OPERATION AtlOpToImGuizmoOp(Utils::Transform transform)
	{
		switch (transform)
		{
		case Utils::Transform::TRANSLATE:
			return ImGuizmo::TRANSLATE;

		case Utils::Transform::ROTATE:
			return ImGuizmo::ROTATE;

		case Utils::Transform::SCALE:
			return ImGuizmo::SCALE;
		}

		ATL_CORE_ASSERT(false, "Unknown Tranform");
		return ImGuizmo::BOUNDS;
	}


	void EditorLayer::OnImGuiRender()
	{
		ATL_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportSize.x, viewportSize.y };
		ImGui::Image((void*)(size_t)m_ViewportFrameBuffer->GetColorAttachmentRendererID(), viewportSize, ImVec2(0.0f, 1.0f) , ImVec2(1.0f, 0.0f));

		ImGuiWindow* window = GImGui->CurrentWindow;
		ImGuizmo::SetRect(window->Pos.x, window->Pos.y, window->Size.x, window->Size.y);
		ImGuizmo::SetDrawlist(window->DrawList);

		auto& camera = m_ActiveScene->GetActiveCamera();
		ImGuizmo::DrawGrid(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()), glm::value_ptr(glm::mat4(0.0f)), 1.0f);

		ECS::Entity selectedEntity = m_SceneHierarchy.GetSelectedEntity();
		if (selectedEntity != ECS::null)
		{
			if (m_ActiveScene->HasComponent<TransformComponent>(selectedEntity))
			{

				TransformComponent& component = m_ActiveScene->GetComponent<TransformComponent>(selectedEntity);
				glm::mat4 transform = component.GetTransform();
				ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()), AtlOpToImGuizmoOp(component.TransformOperation), ImGuizmo::LOCAL, glm::value_ptr(transform));

				Math::DecomposeTransform(transform, component.Translation, component.Rotation, component.Scale);

				if (ImGui::IsKeyPressed('1'))
				{
					component.TransformOperation = Utils::Transform::TRANSLATE;
				}
				else if (ImGui::IsKeyPressed('2'))
				{
					component.TransformOperation = Utils::Transform::ROTATE;
				}
				else if (ImGui::IsKeyPressed('3'))
				{
					component.TransformOperation = Utils::Transform::SCALE;
				}
			}

		}

		ImGui::End();

		ImGui::PopStyleVar();

		m_SceneHierarchy.OnImGuiRender();

		Log::GetAtlasLogger().Draw("Atlas Log");

		m_ActiveScene->OnUpdateEditor();

	}

	void EditorLayer::OnEvent(Event& event)
	{
	}

	void EditorLayer::Begin()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 2.0f;
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		FrameBufferSpecs specs = m_ViewportFrameBuffer->GetSpecs();

		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (specs.Width != (uint32_t)m_ViewportSize.x || specs.Height != (uint32_t)m_ViewportSize.y))
		{
			m_ViewportFrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

	}

	void EditorLayer::End()
	{

	}

}