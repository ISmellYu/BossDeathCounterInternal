#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "Dxguid.lib")
#include "DirectX11.h"

namespace MovementHooks
{
	using ISetCursorPos = BOOL(WINAPI*)(int x, int y);
	inline ISetCursorPos oISetCursorPos;

	using IGetCursorPos = BOOL(WINAPI*)(LPPOINT point);
	inline IGetCursorPos oIGetCursorPos;

	using IDInputGetDeviceState = HRESULT(APIENTRY*)(IDirectInputDevice8* pDInput8, DWORD cbData, LPVOID lpvData);
	inline IDInputGetDeviceState oIDInputGetDeviceState;

	using IDInputGetDeviceData = HRESULT(APIENTRY*)(IDirectInputDevice8* pDInput8, DWORD cbObjectData,
	                                                LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
	inline IDInputGetDeviceData oIDInputGetDeviceData;

	bool HookAll();
}
