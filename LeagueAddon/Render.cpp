#include "Render.h"

//ImFont* overlayFont;

void Render::BeginOverlay() {
	//if (!OnProcessSpellHooked) {

	//	style->Colors[ImGuiCol_TitleBg] = ImVec4(192 / 255.f, 57 / 255.f, 43 / 255.f, 1.0);
	//	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(192 / 255.f, 57 / 255.f, 43 / 255.f, 1.0);
	//	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(231 / 255.f, 76 / 255.f, 60 / 255.f, 1.0);
	//}
	//else {
	//	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.039f, 0.039f, 0.039f, 1.000f);
	//	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.000f, 0.000f, 0.000f, 0.510f);
	//	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.160f, 0.290f, 0.480f, 1.000f);
	//}
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoTitleBar);
}

void Render::EndOverlay() {
	ImGui::End();
}

void Render::BeginOverlayTab(string name) {
	if (GetAsyncKeyState(VK_TAB))
		ImGui::Begin(name.c_str());

	else
		ImGui::Begin(name.c_str(), NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoTitleBar);

}
void Render::EndOverlayTab() {
	ImGui::End();
}

void Render::Draw_Circle(float_t x, float_t y, float_t radius, ImColor color, float thickness) {
	ImGui::GetWindowDrawList()->AddCircle(ImVec2(x, y), radius, color, 0, thickness);
}

void Render::Draw_Circle3DFilled(Vector3 center, float radius, ImColor color) {
	ImVec2 vertices[46];
	Vector3 vertices2[46];
	bool verticesValid[46];

	float angle = 0;
	const float angleStep = (M_PI * 2) / 45;

	for (int i = 0; i <= 45; ++i, angle += angleStep)
	{
		Vector3 pos = Vector3(radius * cosf(angle) + center.x, center.y, radius * sinf(angle) + center.z);
		Vector3 w2sPos;
		verticesValid[i] = Function::World2Screen(&pos, &w2sPos);
		vertices[i] = ImVec2(w2sPos.x, w2sPos.y);
		vertices2[i] = Vector3(w2sPos.x, w2sPos.y, 0);
	}

	for (int i = 0; i < 45; ++i)
		if (vertices2[i].distanceTo(vertices2[i + 1]) > radius)
			return;
	ImGui::GetWindowDrawList()->AddConvexPolyFilled(vertices, 46, color);
}

void Render::Draw_Circle3D(Vector3 center, float radius, ImColor color, float thickness) {
	Vector3 vertices[46];
	bool verticesValid[46];

	float angle = 0;
	const float angleStep = (M_PI * 2) / 45;

	for (int i = 0; i <= 45; ++i, angle += angleStep)
	{
		Vector3 pos = Vector3(radius * cosf(angle) + center.x, center.y, radius * sinf(angle) + center.z);
		Vector3 w2sPos;
		verticesValid[i] = Function::World2Screen(&pos, &w2sPos);
		vertices[i] = Vector3(w2sPos.x, w2sPos.y, 0);
	}

	for (int i = 0; i < 45; ++i) {
		/*Renderer::Draw_Text(vertices[i].x, vertices[i].y, std::to_string(vertices[i].x).c_str(), color);
		Renderer::Draw_Text(vertices[i].x, vertices[i].y + 20, std::to_string(vertices[i].y).c_str(), color);*/
		if (vertices[i].distanceTo(vertices[i + 1]) < radius)
			if ((verticesValid[i] && verticesValid[i + 1]))
				ImGui::GetWindowDrawList()->AddLine({ vertices[i].x, vertices[i].y }, { vertices[i + 1].x,  vertices[i + 1].y }, color, thickness);
	}
}

void Render::Draw_Line3D(Vector3 pos1, Vector3 pos2, ImColor color, float thickness) {
	Vector3 w2s1 = Function::WorldToScreen(&pos1), w2s2 = Function::WorldToScreen(&pos2);

	Render::Draw_Line(w2s1.x, w2s1.y, w2s2.x, w2s2.y, color, thickness);
}


void Render::Draw_Line(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness) {
	Render::Draw_Line(pos1.x, pos1.y, pos2.x, pos2.y, color, thickness);
}

int random(int min, int max) //range : [min, max]
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void Render::Draw_Line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float thickness) {

	//Vector3 vec1;
	//Vector3 vec2;

	//vec1.x = x1;
	//vec1.y = y1;

	//vec2.x = x2;
	//vec2.y = y2;

	//float distance = vec1.distanceTo(vec2);



	//if (distance > 1000) {
	//	Vector3 lastVec = vec1;
	//	Vector3 newVec;
	//	for (int i = 0; i < distance / 200; i++) {
	//		color = ImColor(random(0, 255), random(0, 255), random(0, 255));
	//		newVec = lastVec.Extend(vec2, min(200, lastVec.distanceTo(vec2)));
	//		ImGui::GetWindowDrawList()->AddLine(ImVec2(lastVec.x, lastVec.y), ImVec2(newVec.x, newVec.y), color, thickness);
	//		lastVec = newVec;
	//	}
	//}
	//else
		ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void Render::Draw_FilledRectangle(ImVec2 pos, ImVec2 size, ImColor color, ImColor borderColor, float thickness) {
	Render::Draw_FilledRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, color, borderColor, thickness);
}

void Render::Draw_FilledRectangle(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, ImColor borderColor, float thickness) {
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), color);
	if (borderColor != ImColor::ImColor(0, 0, 0, 0)) {
		ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y1), borderColor, thickness);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(x2, y1), ImVec2(x2, y2), borderColor, thickness);

		ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y2), ImVec2(x2, y2), borderColor, thickness);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y2), ImVec2(x1, y1), borderColor, thickness);
	}
}

void Render::Polygon(Geometry::Polygon poly, ImColor color, float tick)
{
	Vector3 out;
	ImVec2 points[200];
	int i = 0;
	for (auto& point : poly.Points)
	{
		out = Function::WorldToScreen(&point);
		Vector2 screenSpace = Vector2(out.x, out.y);

		points[i].x = screenSpace.x;
		points[i].y = screenSpace.y;
		i++;
	}

	ImGui::GetWindowDrawList()->AddPolyline(points, i, color, true, tick);
}

void draw_line(Vector2 start_pos, Vector2 end_pos, ImColor color, float thickness)
{
	ImGui::GetWindowDrawList()->AddLine(ImVec2(start_pos.x, start_pos.y), ImVec2(end_pos.x, end_pos.y), color, thickness);
}
void draw_line(Vector3 start_pos, Vector3 end_pos, ImColor color, float thickness)
{
	Vector3 temp = Function::WorldToScreen(&start_pos);
	Vector2 startWorldPos = Vector2(temp.x, temp.y);
	temp = Function::WorldToScreen(&end_pos);
	Vector2 endWorldPos = Vector2(temp.x, temp.y);
	//delete temp;
	ImGui::GetWindowDrawList()->AddLine(ImVec2(startWorldPos.x, startWorldPos.y), ImVec2(endWorldPos.x, endWorldPos.y), color, thickness);
}
void draw_line3D(Vector3 start_pos, Vector3 end_pos, ImColor color, float thickness)
{
	Vector3 temp = Function::WorldToScreen(&start_pos);
	Vector2 startWorldPos = Vector2(temp.x, temp.y);
	temp = Function::WorldToScreen(&end_pos);
	Vector2 endWorldPos = Vector2(temp.x, temp.y);
	//delete temp;
	ImGui::GetWindowDrawList()->AddLine(ImVec2(startWorldPos.x, endWorldPos.y), ImVec2(endWorldPos.x, endWorldPos.y), color, thickness);
}

void Render::Rect(Vector3 start, Vector3 end, float radius, float width, ImColor color) {
	Vector3 vector = (end - start).Normalized();
	Vector3 value = vector.Perpendicular();
	Vector3 point2 = start + value * radius;
	Vector3 point3 = start - value * radius;
	Vector3 point4 = end + value * radius;
	Vector3 point5 = end - value * radius;

	draw_line(point2, point4, color, width);
	draw_line(point3, point5, color, width);
	draw_line(point2, point3, color, width);
	draw_line(point5, point4, color, width);
}

void Render::Draw_Text(ImVec2 pos, std::string text, ImU32 color, int fontSize) {
	Render::Draw_Text(pos.x, pos.y, text, color, fontSize);
}

void Render::Draw_Text(float_t x, float_t y, std::string text, ImU32 color, int fontSize) {
	ImGui::GetWindowDrawList()->AddText(NULL, fontSize, ImVec2(x, y), color, text.c_str());
}

void Render::Draw_Text_Centered(ImVec2 pos, ImVec2 size, std::string text, ImU32 color, int fontSize) {
	Render::Draw_Text_Centered(pos.x, pos.y, size.x, size.y, text, color, fontSize);
}

void Render::Draw_Text_Centered(float_t x, float_t y, float_t width, float_t height, std::string text, ImU32 color, int fontSize) {
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

	ImGui::GetWindowDrawList()->AddText(NULL, fontSize, ImVec2(x + width / 2 - textSize.x / 2, y + height / 2 - textSize.y / 2), color, text.c_str());
}

void Render::Draw_Image(ImVec2 pos, ImVec2 size, ID3D11ShaderResourceView* image) {
	Render::Draw_Image(pos.x, pos.y, size.x, size.y, image);
}

void Render::Draw_Image(float_t x, float_t y, float_t width, float_t height, ID3D11ShaderResourceView* image) {
	ImGui::GetWindowDrawList()->AddImage(image, ImVec2(x, y), ImVec2(x + width, y + height));
}