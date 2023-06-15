#pragma once
#include <windows.h>

#include "DarkSouls3.h"
#include "Offsets.h"
#include "State.h"

namespace GameSpecific
{
	inline bool Hook()
	{
		if (GetModuleHandle("DarkSoulsIII.exe") != nullptr)
			DarkSouls3::Hook();
		return true;
	}

	inline void SetDefaultValues()
	{
		if (GetModuleHandle("DarkSoulsIII.exe") == nullptr)
			return;
		uintptr_t g_saveInfo = *(uintptr_t*)((uintptr_t)GetModuleHandle(nullptr) + Offsets::DS3::oSaveInfo);

		while (*(bool*)(g_saveInfo + Offsets::DS3::SaveInfo::isSaveLoaded) == false)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		int deaths = *(int*)(g_saveInfo + Offsets::DS3::SaveInfo::deaths);
		State::currentGame->deaths = deaths;
	}
}
