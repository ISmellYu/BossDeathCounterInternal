#include "DirectX11.h"
#include "DirectX11Demo.h"

#include <cstdio>
#include <iostream>
#include <thread>
#include "Roboto.h"
#include "Garamond.h"
#include "tahoma.h"
#include "imgui_helpers.h"
#include "imgui_notify.h"
#include "Menu.h"
#include "State.h"
#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"

bool ShowMenu = false;
bool ImGui_Initialised = false;

namespace Process
{
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	HMODULE Module;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;

	bool IsExiting;
}

namespace DirectX11Interface
{
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* RenderTargetView;
	IDirectInput8* DirectInput;
	LPDIRECTINPUTDEVICE8 LpdiMouse;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT APIENTRY MJDIGetDeviceState(IDirectInputDevice8* pDInput8, DWORD cbData, LPVOID lpvData)
{
	if (ShowMenu)
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
	if (ShowMenu)
	{
		return 1;
	}
	return oISetCursorPos(x, y);
}

BOOL WINAPI MJGetCursorPos(LPPOINT point)
{
	if (ShowMenu)
	{
		point->x = 0;
		point->y = 0;
		return 1;
	}
	return oIGetCursorPos(point);
}

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

HRESULT APIENTRY MJPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!ImGui_Initialised)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX11Interface::Device)))
		{
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			(void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			DirectX11Interface::Device->GetImmediateContext(&DirectX11Interface::DeviceContext);

			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc(&Desc);
			WindowHwnd = Desc.OutputWindow;

			ID3D11Texture2D* BackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
			DirectX11Interface::Device->CreateRenderTargetView(BackBuffer, nullptr,
			                                                   &DirectX11Interface::RenderTargetView);
			BackBuffer->Release();

			ImGui_ImplWin32_Init(WindowHwnd);
			ImGui_ImplDX11_Init(DirectX11Interface::Device, DirectX11Interface::DeviceContext);

			SetupFonts();

			ImGui_ImplDX11_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
			Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui_Initialised = true;
		}
	}
	if (GetAsyncKeyState(VK_INSERT) & 1) ShowMenu = !ShowMenu;


	ImGui_ImplDX11_NewFrame();

	ImGui_ImplWin32_NewFrame();


	ImGui::NewFrame();
	// ImGui::StyleColorsDark();
	ImGui::SetupImGuiStyle(true, 1.f);
	Menu::ShowOverlay();

	ImGui::GetIO().MouseDrawCursor = ShowMenu;
	if (ShowMenu == true)
	{
		Menu::ShowMenu();
	}

	// imgui notifications
	// Render toasts on top of everything, at the end of your code!
	// You should push style vars here
	ImGui::PushFont(Menu::tahomaFont);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	// Background color
	ImGui::RenderNotifications(); // <-- Here we render all notifications
	ImGui::PopStyleVar(1); // Don't forget to Pop()
	ImGui::PopStyleColor(1);
	ImGui::PopFont();


	ImGui::EndFrame();
	ImGui::Render();
	DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

void APIENTRY MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation,
                            INT BaseVertexLocation)
{
}

void ReleaseAllAndRestore()
{
	Process::IsExiting = true;
	SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (LONG_PTR)Process::WndProc);
	MH_Uninitialize();
	DisableAll();
	DeleteWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	DebugActiveProcessStop(GetCurrentProcessId());
	// release dx11
	DirectX11Interface::Device->Release();
	DirectX11Interface::DeviceContext->Release();
	DirectX11Interface::RenderTargetView->Release();
	//DirectX11Interface::DirectInput->Release();
	//DirectX11Interface::LpdiMouse->Release();
}

uintptr_t GetFunctionAddr(uintptr_t vTable, int index)
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

	auto vTable = **(uintptr_t**)(&lpdiMouse);
	uintptr_t addrFunDeviceState = GetFunctionAddr(vTable, 9);

	std::cout << "Vtable addr: " << std::hex << vTable << "\n";
	std::cout << "Function addr: " << std::hex << addrFunDeviceState << "\n";

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


	uintptr_t addrFunDeviceData = GetFunctionAddr(vTable, 10);
	std::cout << "Function addr: " << std::hex << addrFunDeviceData << "\n";

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
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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

	// institate currentgame by presets

	State::currentGame = std::make_unique<Game>("Game");

	bool InitHook = false;
	while (InitHook == false)
	{
		if (DirectX11::Init() == true)
		{
			CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
			CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);

			// check for dinput
			if (CheckIfUsingDInput())
			{
				// find hooks for it
				HookDInput();
			}
			HookCursor();
			InitHook = true;
		}
	}

	// release all
	while ((GetAsyncKeyState(VK_F1) & 1) == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

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
		if (ChecktDirectXVersion(DirectXVersion.D3D11) == true)
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
