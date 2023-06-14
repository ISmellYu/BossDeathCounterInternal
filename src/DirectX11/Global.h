#pragma once
#include <windows.h>
#include <stdint.h>
#include <assert.h>
#include <psapi.h>
#include <MinHook.h>
#include <thread>

#if defined _M_X64
using uintx_t = uint64_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

namespace Global
{
	static uintx_t* MethodsTable = nullptr;

	inline struct _DirectXVersion
	{
		int Unknown = 0;
		int D3D9 = 1;
		int D3D10 = 2;
		int D3D11 = 3;
		int D3D12 = 4;
	} DirectXVersion;

	inline bool ChecktDirectXVersion(int _DirectXVersion)
	{
		if (_DirectXVersion == DirectXVersion.D3D12)
		{
			if (GetModuleHandle("d3d12.dll") != nullptr)
			{
				return true;
			}
		}

		if (_DirectXVersion == DirectXVersion.D3D11)
		{
			if (GetModuleHandle("d3d11.dll") != nullptr)
			{
				return true;
			}
		}

		if (_DirectXVersion == DirectXVersion.D3D10)
		{
			if (GetModuleHandle("d3d10.dll") != nullptr)
			{
				return true;
			}
		}

		if (_DirectXVersion == DirectXVersion.D3D9)
		{
			if (GetModuleHandle("d3d9.dll") != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	inline bool CheckIfUsingDInput()
	{
		if (GetModuleHandle("dinput8.dll") != nullptr)
			return true;
		return false;
	}

	inline bool CheckIfUsingXInput()
	{
		if (GetModuleHandle("xinput1_4.dll") || GetModuleHandle("xinput1_3.dll") || GetModuleHandle("xinput1_2.dll") || GetModuleHandle("xinput1_1.dll") || 
			GetModuleHandle("XInput9_1_0.dll"))
		{
			return true;
		}
		return false;
	}

	inline WNDCLASSEX WindowClass;
	inline HWND WindowHwnd;

	inline bool InitWindow()
	{
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(nullptr);
		WindowClass.hIcon = nullptr;
		WindowClass.hCursor = nullptr;
		WindowClass.hbrBackground = nullptr;
		WindowClass.lpszMenuName = nullptr;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = nullptr;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL,
		                          NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == nullptr)
		{
			return false;
		}
		return true;
	}

	inline bool DeleteWindow()
	{
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != nullptr)
		{
			return false;
		}
		return true;
	}

	inline bool CreateHook(uint16_t Index, void** Original, void* Function)
	{
		assert(Index >= 0 && Original != NULL && Function != NULL);
		auto target = (void*)MethodsTable[Index];
		if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK)
		{
			return false;
		}
		return true;
	}


	inline void DisableHook(uint16_t Index)
	{
		assert(Index >= 0);
		MH_DisableHook((void*)MethodsTable[Index]);
	}

	inline void DisableAll()
	{
		MH_DisableHook(nullptr);
		free(MethodsTable);
		MethodsTable = nullptr;
	}
}


