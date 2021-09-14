#pragma once

namespace ImGui {
	typedef int ImGuiSliderFlags;       // -> enum ImGuiSliderFlags_     // Flags: for DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.
	typedef int ImDrawFlags;            // -> enum ImDrawFlags_          // Flags: for ImDrawList functions

	bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags, ImDrawFlags drawFlags);
}
