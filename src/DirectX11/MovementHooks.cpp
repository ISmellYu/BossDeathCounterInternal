#include "MovementHooks.h"

#include <dinput.h>
#include <iostream>
#include <MinHook.h>

#include "Global.h"
#include "GraphicsHook.h"
#include "Process.h"
#include "VkCodes.h"

#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

namespace MovementHooks
{
	HRESULT APIENTRY MJDIGetDeviceState(IDirectInputDevice8* pDInput8, DWORD cbData, LPVOID lpvData)
	{
		if (GraphicsHook::ShowMenu)
			cbData = 0;
		return oIDInputGetDeviceState(pDInput8, cbData, lpvData);
	}

	HRESULT APIENTRY MJDIGetDeviceData(IDirectInputDevice8* pDInput8, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod,
	                                   LPDWORD pdwInOut, DWORD dwFlags)
	{
		// std::cout << "blocking input!111\n";
		// auto result = oIDInputGetDeviceData(pDInput8, cbObjectData, rgdod, pdwInOut, dwFlags);
		// if (ShowMenu)
		// {
		// 	// std::cout << "blocking input!\n";
		// 	*pdwInOut = 0;
		// }
		std::cout << "Called device data\n";
		return oIDInputGetDeviceData(pDInput8, cbObjectData, rgdod, pdwInOut, dwFlags);
	}

	BOOL WINAPI MJSetCursorPos(int x, int y)
	{
		if (GraphicsHook::ShowMenu)
		{
			return 1;
		}
		return oISetCursorPos(x, y);
	}

	BOOL WINAPI MJGetCursorPos(LPPOINT point)
	{
		if (GraphicsHook::ShowMenu)
		{
			point->x = 0;
			point->y = 0;
			return 1;
		}
		return oIGetCursorPos(point);
	}

	SHORT WINAPI MJGetAsyncKeyState(int vKey)
	{
		return oIGetAsyncKeyState(vKey);
	}

	SHORT WINAPI MJGetKeyState(int vKey)
	{
		return oIGetKeyState(vKey);
	}

	BOOL WINAPI MJGetKeyboardState(PBYTE lpKeyState)
	{
		BOOL result = oIGetKeyboardState(lpKeyState);
		if (GraphicsHook::ShowMenu)
		{
			for (int i = 0; i < 255; i++)
			{
				*(lpKeyState + i) = 0;
			}
		}
		return result;
	}

	UINT WINAPI MJGetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader)
	{
		if (GraphicsHook::ShowMenu)
		{
			*pcbSize = 0;
		}
		return oIGetRawInputData(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);
	}

	uintptr_t GetIndexFunctionAddr(uintptr_t vTable, int index)
	{
		uintptr_t pFunction = vTable + index * sizeof uintptr_t;
		uintptr_t fun = *(uintptr_t*)pFunction;
		return fun;
	}

	bool HookDInput()
	{
		IDirectInput8* pDirectInput = nullptr;
		if (DirectInput8Create(Process::Module, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirectInput,
		                       nullptr) != DI_OK)
		{
			return false;
		}

		LPDIRECTINPUTDEVICE8 lpdiMouse;

		if (pDirectInput->CreateDevice(GUID_SysMouse, &lpdiMouse, nullptr) != DI_OK)
		{
			pDirectInput->Release();
			return false;
		}

		auto objAddr = *(uintptr_t*)&lpdiMouse;
		std::cout << "Lpdi mouse: " << std::hex << objAddr << "\n";
		auto vTable = **(uintptr_t**)(&lpdiMouse);
		uintptr_t addrFunDeviceState = GetIndexFunctionAddr(vTable, 9);

		std::cout << "Vtable addr: " << std::hex << vTable << "\n";
		std::cout << "Function device state addr: " << std::hex << addrFunDeviceState << "\n";

		if (MH_CreateHook((void*)addrFunDeviceState, MJDIGetDeviceState, (void**)&oIDInputGetDeviceState) != MH_OK)
		{
			std::cout << "Create hook not working xdd\n";
			return false;
		}

		if (MH_EnableHook((void*)addrFunDeviceState) != MH_OK)
		{
			std::cout << "Not enabled hook idk why xdd\n";
			return false;
		}


		uintptr_t addrFunDeviceData = GetIndexFunctionAddr(vTable, 10);
		std::cout << "Function device data addr: " << std::hex << addrFunDeviceData << "\n";

		if (MH_CreateHook((void*)addrFunDeviceData, MJDIGetDeviceData, (void**)&oIDInputGetDeviceData) != MH_OK)
		{
			std::cout << "Create hook not working xdd\n";
			return false;
		}

		if (MH_EnableHook((void*)addrFunDeviceData) != MH_OK)
		{
			std::cout << "Not enabled hook idk why xdd\n";
			return false;
		}


		return true;
	}

	bool HookCursor()
	{
		if (MH_CreateHook(SetCursorPos, MJSetCursorPos, (void**)&oISetCursorPos))
		{
			std::cout << "Create hook on set cursor pos not working!\n";
			return false;
		}

		if (MH_EnableHook(SetCursorPos))
		{
			std::cout << "Cant enable set cursor pos!\n";
			return false;
		}

		// if (MH_CreateHook(&GetCursorPos, MJGetCursorPos, (void**)&oIGetCursorPos))
		// {
		// 	std::cout << "Create hook on get cursor pos not working!\n";
		// 	return false;
		// }
		//
		// if (MH_EnableHook(&GetCursorPos))
		// {
		// 	std::cout << "Cant enable get cursor pos!\n";
		// 	return false;
		// }
		return true;
	}

	bool HookKeyboard()
	{
		if (MH_CreateHook(GetAsyncKeyState, MJGetAsyncKeyState, (void**)&oIGetAsyncKeyState))
		{
			std::cout << "Create hook on GetAsyncKeyState not working!\n";
			return false;
		}

		if (MH_EnableHook(GetAsyncKeyState))
		{
			std::cout << "Cant enable GetAsyncKeyState!\n";
			return false;
		}


		if (MH_CreateHook(GetKeyState, MJGetKeyState, (void**)&oIGetKeyState))
		{
			std::cout << "Create hook on GetKeyState not working!\n";
			return false;
		}

		if (MH_EnableHook(GetKeyState))
		{
			std::cout << "Cant enable GetKeyState!\n";
			return false;
		}


		if (MH_CreateHook(GetKeyboardState, MJGetKeyboardState, (void**)&oIGetKeyboardState))
		{
			std::cout << "Create hook on GetKeyboardState not working!\n";
			return false;
		}

		if (MH_EnableHook(GetKeyboardState))
		{
			std::cout << "Cant enable GetKeyBoardState!\n";
			return false;
		}

		return true;
	}

	bool HookXInput()
	{
		return 0;
	}

	bool HookInputDevices()
	{
		if (MH_CreateHook(GetRawInputData, MJGetRawInputData, (void**)&oIGetRawInputData))
		{
			std::cout << "Create hook on GetRawInputData not working!\n";
			return false;
		}

		if (MH_EnableHook(GetRawInputData))
		{
			std::cout << "Create enable GetRawInputData!\n";
			return false;
		}
		return true;
	}
	bool HookAll()
	{
		if (Global::CheckIfUsingDInput())
		{
			HookDInput();
		}
		HookCursor();
		HookKeyboard();
		if (Global::CheckIfUsingXInput())
		{
			HookXInput();
		}
		HookInputDevices();
		return true;
	}

}
