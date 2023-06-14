#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "Dxguid.lib")


#include <xinput.h>

#include "DirectX11.h"


namespace MovementHooks
{

	// Cursor stuff
	using ISetCursorPos = BOOL(WINAPI*)(int x, int y);
	inline ISetCursorPos oISetCursorPos;

	using IGetCursorPos = BOOL(WINAPI*)(LPPOINT point);
	inline IGetCursorPos oIGetCursorPos;


	// DInput stuff
	using IDInputGetDeviceState = HRESULT(APIENTRY*)(IDirectInputDevice8* pDInput8, DWORD cbData, LPVOID lpvData);
	inline IDInputGetDeviceState oIDInputGetDeviceState;

	using IDInputGetDeviceData = HRESULT(APIENTRY*)(IDirectInputDevice8* pDInput8, DWORD cbObjectData,
	                                                LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
	inline IDInputGetDeviceData oIDInputGetDeviceData;


	// Keyboard stuff
	using IGetAsyncKeyState = SHORT(WINAPI*)(int vKey);
	inline IGetAsyncKeyState oIGetAsyncKeyState;

	using IGetKeyState = SHORT(WINAPI*)(int vKey);
	inline IGetKeyState oIGetKeyState;

	using IGetKeyboardState = BOOL(WINAPI*)(PBYTE lpKeyState);
	inline IGetKeyboardState oIGetKeyboardState;



	// XInput stuff "XInput9_1_0.dll"
	using IXInputGetKeystroke9 = DWORD(WINAPI*)(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke);
	inline IXInputGetKeystroke9 oIXInputGetKeystroke9;

	using IXInputGetState9 = DWORD(WINAPI*)(DWORD dwUserIndex, XINPUT_STATE* pState);
	inline IXInputGetState9 oIXInputGetState9;

	// XInput stuff "xinput1_?.dll"
	using IXInputGetKeystroke_ = DWORD(WINAPI*)(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke);
	inline IXInputGetKeystroke_ oIXInputGetKeystroke_;

	using IXInputGetState_ = DWORD(WINAPI*)(DWORD dwUserIndex, XINPUT_STATE* pState);
	inline IXInputGetState_ oIXInputGetState_;


	// Raw input
	using IGetRawInputData = UINT(WINAPI*)(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbsizeHeader);
	inline IGetRawInputData oIGetRawInputData;


	bool HookAll();
}
