#include "DarkSouls3.h"

#include <iostream>
#include <MinHook.h>

#include "GameFunctionDefinitions.h"
#include "GraphicsHook.h"
#include "State.h"
#include "Offsets.h"
#include "VkCodes.h"

namespace DarkSouls3
{
	__int64 __fastcall MJfnAddDeathCountDS3(void* rcx_t)
	{
		State::currentGame->IncrementDeaths();
		return oIfnAddDeathCountDS3(rcx_t);
	}

	__int64 __fastcall MJfnGetKeyStateDS3(int key)
	{
		__int64 result = ofnGetKeyStateDS3(key);
		return result;
	}

	__int64 __fastcall MJfnSaveRequest()
	{
		std::cout << "loool!\n";
		return ofnSaveRequestDS3();
	}

	void __fastcall MJfnReportBossArea(__int64 a1, __int64 a2, __int64 a3)
	{
		
	}

	void HookDeaths()
	{
		void* funAddr = (void*)(((uintptr_t)GetModuleHandle(nullptr)) + Offsets::DS3::Functions::AddDeathCount);
		if (MH_CreateHook(funAddr, MJfnAddDeathCountDS3, (void**)&oIfnAddDeathCountDS3) == MH_OK)
		{
			MH_EnableHook(funAddr);
		}
	}

	void HookKeyboard()
	{
		void* funAddr = (void*)(((uintptr_t)GetModuleHandle(nullptr)) + Offsets::DS3::Functions::GetKeyState);
		if (MH_CreateHook(funAddr, MJfnGetKeyStateDS3, (void**)&ofnGetKeyStateDS3) == MH_OK)
		{
			MH_EnableHook(funAddr);
		}
	}

	void HookSomething()
	{
		void* funAddr = (void*)(((uintptr_t)GetModuleHandle(nullptr)) + Offsets::DS3::Functions::SaveRequest);
		if (MH_CreateHook(funAddr, MJfnSaveRequest, (void**)&ofnSaveRequestDS3) == MH_OK)
		{
			MH_EnableHook(funAddr);
		}
	}

	bool Hook()
	{
		HookDeaths();
		HookKeyboard();
		HookSomething();
		return true;
	}

}
