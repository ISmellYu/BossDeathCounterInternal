#pragma once

#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"

namespace Menu
{
	inline ImFont* overlayFont = nullptr;
	inline ImFont* customFont = nullptr;
	inline ImFont* tahomaFont = nullptr;
	void ShowMenu();
	void ShowOverlay();
}
