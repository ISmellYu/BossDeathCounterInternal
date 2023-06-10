#pragma once
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace DirectX11Interface
{
	inline ID3D11Device* Device;
	inline ID3D11DeviceContext* DeviceContext;
	inline ID3D11RenderTargetView* RenderTargetView;
	inline IDirectInput8* DirectInput;
	inline LPDIRECTINPUTDEVICE8 LpdiMouse;
}
