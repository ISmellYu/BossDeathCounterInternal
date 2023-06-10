#pragma once
#include <Windows.h>
namespace Process
{
	inline DWORD ID;
	inline HANDLE Handle;
	inline HWND Hwnd;
	inline HMODULE Module;
	inline WNDPROC WndProc;
	inline int WindowWidth;
	inline int WindowHeight;
	inline LPCSTR Title;
	inline LPCSTR ClassName;
	inline LPCSTR Path;

	inline bool IsExiting;
}