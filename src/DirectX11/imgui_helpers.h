#pragma once
#include <string>
#include <Windows.h>
#include <corecrt_math_defines.h>

#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"
using RGBA = struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
};

namespace ImGui
{
	inline bool SetWindowOverlayPos(int position, ImGuiWindowFlags* flags)
	{
		ImGuiIO& io = ImGui::GetIO();
		*flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
		if (position >= 0)
		{
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (position & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (position & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (position & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (position & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			*flags |= ImGuiWindowFlags_NoMove;
			return true;
		}
		return false;
	}
}

static std::string string_To_UTF8(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	auto pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, nullptr, NULL, nullptr, nullptr);
	auto pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, nullptr, nullptr);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = nullptr;
	pBuf = nullptr;
	return retStr;
}

static void DrawStrokeText(int x, int y, RGBA* color, const char* str)
{
	ImFont a;
	auto utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1),
	                                        ImGui::ColorConvertFloat4ToU32(
		                                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1),
	                                        ImGui::ColorConvertFloat4ToU32(
		                                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 1, y),
	                                        ImGui::ColorConvertFloat4ToU32(
		                                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1, y),
	                                        ImGui::ColorConvertFloat4ToU32(
		                                        ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y),
	                                        ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                        color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                        color->A / 255.0)), utf_8_2.c_str());
}

static void DrawNewText(int x, int y, RGBA* color, const char* str)
{
	ImFont a;
	auto utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y),
	                                        ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                        color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                        color->A / 255.0)), utf_8_2.c_str());
}

static void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h),
	                                        ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                        color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                        color->A / 255.0)), 0, 0, thickness);
}

static void DrawRectCustom(int upX, int upY, int lowX, int lowY, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(upX, upY), ImVec2(lowX, lowY),
	                                        ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                        color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                        color->A / 255.0)), 0, 0, thickness);
}

static void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
	                                              ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                              color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                              color->A / 255.0)), 0, 0);
}

static void DrawCircleFilled(int x, int y, int radius, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius,
	                                                ImGui::ColorConvertFloat4ToU32(
		                                                ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                                       color->A / 255.0)));
}

static void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius,
	                                          ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                          color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                          color->A / 255.0)), segments);
}

static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickne)
{
	ImGui::GetForegroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
	                                            ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                            color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                            color->A / 255.0)), thickne);
}

static void DrawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
	                                                  ImGui::ColorConvertFloat4ToU32(
		                                                  ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                                         color->A / 255.0)));
}

static void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2),
	                                        ImGui::ColorConvertFloat4ToU32(ImVec4(
		                                        color->R / 255.0, color->G / 255.0, color->B / 255.0,
		                                        color->A / 255.0)), thickness);
}

static void DrawCornerBox(int x, int y, int w, int h, int borderPx, RGBA* color)
{
	DrawFilledRect(x + borderPx, y, w / 3, borderPx, color);
	DrawFilledRect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color);
	DrawFilledRect(x, y, borderPx, h / 3, color);
	DrawFilledRect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
	DrawFilledRect(x + borderPx, y + h + borderPx, w / 3, borderPx, color);
	DrawFilledRect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color);
	DrawFilledRect(x + w + borderPx, y, borderPx, h / 3, color);
	DrawFilledRect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
}

static ImVec2 GetCordByAngle(float angle, float distance)
{
	angle = static_cast<float>(M_PI) / 180 * angle;
	auto x = distance * cos(angle);
	auto y = distance * sin(angle);
	return ImVec2(x, y);
}
