#include <cstdio>
#include <iostream>
#include <thread>

#include "DirectX11Interface.h"
#include "GameSpecific.h"
#include "Global.h"
#include "GraphicsHook.h"
#include "MovementHooks.h"
#include "Process.h"
#include "State.h"
#include "Windows.h"
#include "Offsets.h"
#include <fstream>

#include "SaveHandler.h"

void ReleaseAllAndRestore()
{
	Process::IsExiting = true;
	SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (LONG_PTR)Process::WndProc);
	MH_Uninitialize();
	Global::DisableAll();
	Global::DeleteWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	DebugActiveProcessStop(GetCurrentProcessId());

	// release dx11
	DirectX11Interface::Device->Release();
	DirectX11Interface::DeviceContext->Release();
	DirectX11Interface::RenderTargetView->Release();
}

DWORD WINAPI MainThread(LPVOID lpParameter)
{
	bool WindowFocus = false;
	while (WindowFocus == false)
	{
		auto xd = GetModuleHandleA(nullptr);
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (GetCurrentProcessId() == ForegroundWindowProcessID)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			Process::ID = GetCurrentProcessId();
			Process::Handle = GetCurrentProcess();
			Process::Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process::Hwnd, &TempRect);
			Process::WindowWidth = TempRect.right - TempRect.left;
			Process::WindowHeight = TempRect.bottom - TempRect.top;

			char TempTitle[MAX_PATH];
			GetWindowText(Process::Hwnd, TempTitle, sizeof(TempTitle));
			Process::Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process::Hwnd, TempClassName, sizeof(TempClassName));
			Process::ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process::Handle, nullptr, TempPath, sizeof(TempPath));
			Process::Path = TempPath;

			WindowFocus = true;
		}
	}

	// initialize currentgame by presets
	// AllocConsole();
	// freopen("CONOUT$", "w", stdout);

	// check if save exists

	std::ifstream infile("save.json");
	if (infile.good())
	{
		State::currentGame = SaveHandler::Load("save.json");
	}
	else
	{
		State::currentGame = std::make_unique<Game>("Game");
	}

	GraphicsHook::Hook();
	MovementHooks::HookAll();
	GameSpecific::Hook();
	GameSpecific::SetDefaultValues();

	// release all
	while ((GetAsyncKeyState(VK_F1) & 1) == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// save all
	State::currentGame->PauseCurrentBoss();
	SaveHandler::Save(State::currentGame.get(), "save.json");
	ReleaseAllAndRestore();
	// FreeConsole();
	FreeLibraryAndExitThread(Process::Module, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (Global::ChecktDirectXVersion(Global::DirectXVersion.D3D11) == true)
		{
			Process::Module = hModule;
			CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
		}
		break;
	case DLL_PROCESS_DETACH:
		// FreeLibrary(hModule);
		// FreeLibraryAndExitThread(hModule, TRUE);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
