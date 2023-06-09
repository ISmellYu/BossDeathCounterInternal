#pragma once

#include <dxgi.h>
#include <d3d11.h>

#include "Global.h"

namespace DirectX11
{
	inline bool Init()
	{
		if (Global::InitWindow() == false)
		{
			return false;
		}

		HMODULE D3D11Module = GetModuleHandle("d3d11.dll");
		if (D3D11Module == nullptr)
		{
			Global::DeleteWindow();
			return false;
		}

		void* D3D11CreateDeviceAndSwapChain = GetProcAddress(D3D11Module, "D3D11CreateDeviceAndSwapChain");
		if (D3D11CreateDeviceAndSwapChain == nullptr)
		{
			Global::DeleteWindow();
			return false;
		}

		D3D_FEATURE_LEVEL FeatureLevel;
		const D3D_FEATURE_LEVEL FeatureLevels[] = {D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0};

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = Global::WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;

		if (static_cast<long(__stdcall*)(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext**)>(D3D11CreateDeviceAndSwapChain)(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			                                                       FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc,
			                                                       &SwapChain, &Device, &FeatureLevel, &Context) < 0)
		{
			Global::DeleteWindow();
			return false;
		}

		Global::MethodsTable = static_cast<uintx_t*>(::calloc(205, sizeof(uintx_t)));
		memcpy(Global::MethodsTable, *(uintx_t**)SwapChain, 18 * sizeof(uintx_t));
		memcpy(Global::MethodsTable + 18, *(uintx_t**)Device, 43 * sizeof(uintx_t));
		memcpy(Global::MethodsTable + 18 + 43, *(uintx_t**)Context, 144 * sizeof(uintx_t));

		MH_Initialize();
		SwapChain->Release();
		SwapChain = nullptr;
		Device->Release();
		Device = nullptr;
		Context->Release();
		Context = nullptr;
		Global::DeleteWindow();
		return true;
	}
}
