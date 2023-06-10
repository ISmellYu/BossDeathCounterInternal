#include "GraphicsHook.h"


#include "DirectX11.h"
#include "DirectX11Interface.h"
#include "Fonts.h"
#include "Menu.h"
#include "Process.h"

#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"

#include "FunctionDefinitions.h"
#include "imgui_helpers.h"
#include "imgui_notify.h"
#include "State.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace GraphicsHook
{
	
	LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (GraphicsHook::ShowMenu)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
			return true;
		}
		return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
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
				Global::WindowHwnd = Desc.OutputWindow;

				ID3D11Texture2D* BackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
				DirectX11Interface::Device->CreateRenderTargetView(BackBuffer, nullptr,
				                                                   &DirectX11Interface::RenderTargetView);
				BackBuffer->Release();

				ImGui_ImplWin32_Init(Global::WindowHwnd);
				ImGui_ImplDX11_Init(DirectX11Interface::Device, DirectX11Interface::DeviceContext);

				Fonts::SetupFonts();

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
			Menu::ShowMenu();

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


		// increment by 1 death counter
		if ((GetAsyncKeyState(VK_F2) & 1))
		{
			// State::currentGame->IncrementDeaths();
		}

		// increment by 1 death counter
		if ((GetAsyncKeyState(VK_F9) & 1))
		{
			State::currentGame->DecrementDeaths();
		}

		DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
	}

	void APIENTRY MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation,
	                            INT BaseVertexLocation)
	{
	}

	bool Hook()
	{
		bool InitHook = false;
		while (InitHook == false)
		{
			if (DirectX11::Init() == true)
			{
				Global::CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
				Global::CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
				InitHook = true;
			}
		}
		return true;
	}

}


