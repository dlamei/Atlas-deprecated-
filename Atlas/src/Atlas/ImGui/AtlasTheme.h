#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace Atlas {

	static ImVec4 ATL_BLACK_COL		 = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
	static ImVec4 ATL_DARK_GREY_COL	 = ImVec4( 0.1f,  0.1f,  0.1f, 1.0f);
	static ImVec4 ATL_GREY_COL		 = ImVec4( 0.2f,  0.2f,  0.2f, 1.0f);
	static ImVec4 ATL_LIGHT_GREY_COL = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);
	static ImVec4 ATL_DARK_WHITE_COL = ImVec4( 0.3f,  0.3f,  0.3f, 1.0f);
	static ImVec4 ATL_WHITE_COL		 = ImVec4( 0.4f,  0.4f,  0.4f, 1.0f);

	static ImVec4 ATL_RED_COL		 = ImVec4( 0.7f, 0.08f, 0.14f, 1.0f);
	static ImVec4 ATL_GREEN_COL		 = ImVec4(0.0f, 0.58f,  0.0f, 1.0f);
	static ImVec4 ATL_YELLOW_COL	 = ImVec4(0.78f, 0.75f, 0.25f, 1.0f);
	static ImVec4 ATL_BLUE_COL		 = ImVec4( 0.0f, 0.27f,  0.8f, 1.0f);

	static ImVec4 ATL_LIGHT_RED_COL		 = ImVec4( 0.7f, 0.08f, 0.14f, 1.0f) + ImVec4(0.2f, 0.2f, 0.2f, 0.0f);
	static ImVec4 ATL_LIGHT_GREEN_COL	 = ImVec4(0.0f, 0.58f,  0.0f, 1.0f)  + ImVec4(0.2f, 0.2f, 0.2f, 0.0f);
	static ImVec4 ATL_LIGHT_YELLOW_COL	 = ImVec4(0.78f, 0.75f, 0.25f, 1.0f) + ImVec4(0.2f, 0.2f, 0.2f, 0.0f);
	static ImVec4 ATL_LIGHT_BLUE_COL	 = ImVec4( 0.0f, 0.27f,  0.8f, 1.0f) + ImVec4(0.2f, 0.2f, 0.2f, 0.0f);

	void SetAtlasTheme();
	void PushButtonStyle(const ImVec4& color);
	void PopButtonStyle();
}
