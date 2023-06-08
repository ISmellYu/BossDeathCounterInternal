#pragma once

#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"

namespace Menu
{
	inline ImFont* overlayFont = NULL;
	inline ImFont* customFont = NULL;
	void ShowMenu();
	void ShowOverlay();
}