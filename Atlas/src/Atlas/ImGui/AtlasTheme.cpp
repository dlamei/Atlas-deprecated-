#include "atlpch.h"
#include "AtlasTheme.h"


namespace Atlas {

	void Atlas::SetAtlasTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ATL_BLACK_COL;


		// Headers
		colors[ImGuiCol_Header]					= ATL_DARK_GREY_COL;
		colors[ImGuiCol_HeaderHovered]			= ATL_LIGHT_GREY_COL;
		colors[ImGuiCol_HeaderActive]			= ATL_BLACK_COL;

		// Buttons
		colors[ImGuiCol_Button]					= ATL_DARK_GREY_COL;
		colors[ImGuiCol_ButtonHovered]			= ATL_LIGHT_GREY_COL;
		colors[ImGuiCol_ButtonActive]			= ATL_GREY_COL;

		// Frame BG
		colors[ImGuiCol_FrameBg]				= ATL_DARK_GREY_COL;
		colors[ImGuiCol_FrameBgHovered]			= ATL_LIGHT_GREY_COL;
		colors[ImGuiCol_FrameBgActive]			= ATL_DARK_WHITE_COL;

		// Tabs
		colors[ImGuiCol_Tab]					= ATL_DARK_GREY_COL;
		colors[ImGuiCol_TabHovered]				= ATL_DARK_WHITE_COL;
		colors[ImGuiCol_TabActive]				= ATL_BLACK_COL;
		colors[ImGuiCol_TabUnfocused]			= ATL_GREY_COL;
		colors[ImGuiCol_TabUnfocusedActive]		= ATL_DARK_GREY_COL;

		// Title
		colors[ImGuiCol_TitleBg]				= ATL_GREY_COL;
		colors[ImGuiCol_TitleBgActive]			= ATL_LIGHT_GREY_COL;
		colors[ImGuiCol_TitleBgCollapsed]		= ATL_DARK_GREY_COL;
	}
	void PushButtonStyle(const ImVec4& color)
	{
		 ImGui::PushStyleColor(ImGuiCol_Button, color);
		 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color + ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
		 ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	}

	void PopButtonStyle()
	{
		ImGui::PopStyleColor(3);
	}
}