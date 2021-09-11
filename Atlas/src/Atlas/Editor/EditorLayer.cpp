#include "atlpch.h"
#include "EditorLayer.h"

#include "imgui.h"

#include "Atlas/Core/Core.h"

namespace Atlas {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		ATL_PROFILE_FUNCTION();

		ImGuiStyle& style = ImGui::GetStyle();

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

	void EditorLayer::OnImGuiRender()
	{
		ATL_PROFILE_FUNCTION();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportSize.x, viewportSize.y };
		ImGui::Image((void*)(size_t)m_ViewportFrameBuffer->GetColorAttachmentRendererID(), viewportSize, ImVec2(0.0f, 1.0f) , ImVec2(1.0f, 0.0f));
		ImGui::End();
		ImGui::PopStyleVar();

		m_SceneHierarchy.OnImGuiRender();
	}

	void EditorLayer::OnEvent(Event& event)
	{
	}

	void EditorLayer::Begin()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 1.8f;
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