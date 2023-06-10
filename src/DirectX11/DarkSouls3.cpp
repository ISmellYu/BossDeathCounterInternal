#include "DarkSouls3.h"

#include <iostream>
#include <MinHook.h>

#include "GameFunctionDefinitions.h"
#include "State.h"
#include "Offsets.h"

namespace DarkSouls3
{
	__int64 __fastcall MJfnAddDeathCountDS3(void* rcx_t)
	{
		State::currentGame->IncrementDeaths();
		return oIfnAddDeathCountDS3(rcx_t);
	}

	void HookDeaths()
	{
		void* funAddr = (void*)(((uintptr_t)GetModuleHandle(nullptr)) + Offsets::DS3::Functions::AddDeathCount);
		std::cout << std::hex << funAddr << "\n";
		if (MH_CreateHook(funAddr, MJfnAddDeathCountDS3, (void**)&oIfnAddDeathCountDS3) == MH_OK)
		{
			MH_EnableHook(funAddr);
			std::cout << "Success hooking funAddr\n";
		}
	}

	bool Hook()
	{
		HookDeaths();
		return true;
	}

}
