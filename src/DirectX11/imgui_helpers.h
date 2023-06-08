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
		ImGuiIO& io = GetIO();
		*flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
		if (position >= 0)
		{
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (position & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (position & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (position & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (position & 2) ? 1.0f : 0.0f;
			SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			*flags |= ImGuiWindowFlags_NoMove;
			return true;
		}
		return false;
	}

	inline void SetupImGuiStyle(bool bStyleDark_, float alpha_)
	{
		ImGuiStyle& style = GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.GrabRounding = style.FrameRounding = 2.3f;
	}

	inline void AlignForWidth(float width, float alignment = 0.5f)
	{
		ImGuiStyle& style = GetStyle();
		float avail = GetContentRegionAvail().x;
		float off = (avail - width) * alignment;
		if (off > 0.0f)
			SetCursorPosX(GetCursorPosX() + off);
	}

	inline ImVec4 RGBAtoIV4(int r, int g, int b, float a)
	{
		float newr = r / static_cast<float>(255);
		float newg = g / static_cast<float>(255);
		float newb = b / static_cast<float>(255);
		float newa = a;
		return {newr, newg, newb, newa};
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
