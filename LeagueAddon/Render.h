#pragma once
#include "ImGui/imgui.h"
#include "Vector.h"
#include "Functions.h"
#include "Geometry.h"

namespace Render {
	void BeginOverlay();
	void EndOverlay();

	void Draw_Line(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness);
	void Draw_Line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float thickness);

	void Draw_FilledRectangle(ImVec2 pos, ImVec2 size, ImColor color, ImColor borderColor = ImColor::ImColor(0, 0, 0, 0), float thickness = 1);
	void Draw_FilledRectangle(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, ImColor borderColor = ImColor::ImColor(0, 0, 0, 0), float thickness = 1);

	void Draw_Circle3DFilled(Vector3 center, float radius, ImColor color);
	void Draw_Circle3D(Vector3 center, float radius, ImColor color, float thickness);

	void Polygon(Geometry::Polygon poly, ImColor color, float tick);
	void Rect(Vector3 start, Vector3 end, float radius, float width, ImColor color);

	void Draw_Text(ImVec2 pos, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text(float_t x, float_t y, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text_Centered(ImVec2 pos, ImVec2 size, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
	void Draw_Text_Centered(float_t x, float_t y, float_t width, float_t height, std::string text, ImU32 color = 0xFFFFFFFF, int fontSize = 13);
}