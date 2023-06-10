#pragma once

#include "Garamond.h"
#include "imgui_notify.h"
#include "Menu.h"
#include "Roboto.h"
#include "tahoma.h"
#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"

namespace Fonts
{
	void SetupFonts()
	{
		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		Menu::overlayFont = io.Fonts->
		                       AddFontFromMemoryTTF((void*)Roboto_Bold_ttf, sizeof(Roboto_Bold_ttf), 20.f, &font_cfg);
		ImGui::MergeIconsWithLatestFont(16.f, false);
		io.FontDefault = Menu::overlayFont;
		Menu::customFont = io.Fonts->AddFontFromMemoryTTF((void*)EBGaramond_SemiBold_ttf, sizeof(EBGaramond_SemiBold_ttf),
		                                                  30.0f, &font_cfg);
		ImGui::MergeIconsWithLatestFont(16.f, false);

		Menu::tahomaFont = io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg);
		ImGui::MergeIconsWithLatestFont(16.f, false);
	}
}
