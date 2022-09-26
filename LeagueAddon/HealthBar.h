#pragma once
#include "../Render.h"
#include <vector>
#include "Utils.h"

using namespace std;

enum HealthBarType {
	Default = 0,
	UnderHero = 1,
};

enum HealthBarAreaType {
	FromLeft,
	FromRight,
	Numeric,
	Percents,
};

class HealthBarArea {
public:
	HealthBarArea(ImColor color, HealthBarAreaType healthBarAreaType, float from, float to) {
		this->Color = color;
		this->Type = healthBarAreaType;
	}

	HealthBarArea(ImColor color, HealthBarAreaType healthBarAreaType, float size) {
		this->Color = color;
		this->Type = healthBarAreaType;
		this->Size = size;
	}

	HealthBarAreaType Type = HealthBarAreaType::Numeric;
	ImColor Color;
	float From; // Numeric - 100  | Percents 0.10 or To   > 1 then From / 100 = 0.x
	float To;   // Numeric - 200  | Percents 0.20 or To   > 1 then To   / 100 = 0.x
	float Size;
};

class HealthBar {
public:
	HealthBar(HealthBarType type, float width = 140, float height = 20) {
		this->Type = type;
		if (type == HealthBarType::Default) {

		}
		else {
			this->Width = width;
			this->Height = height;
		}
	}

	void AddLeftArea(float size, ImColor color, HealthBarAreaType healthBarAreaType = HealthBarAreaType::FromLeft) {
		HealthBarArea area = HealthBarArea(color, healthBarAreaType, size);
		leftAreas.push_back(area);
	}

	void AddRightArea(float size, ImColor color, HealthBarAreaType healthBarAreaType = HealthBarAreaType::FromRight) {
		HealthBarArea area = HealthBarArea(color, healthBarAreaType, size);
		rightAreas.push_back(area);
	}

	void AddRightArea(float from, float to, ImColor color, HealthBarAreaType healthBarAreaType = HealthBarAreaType::Numeric) {
		if (healthBarAreaType == HealthBarAreaType::Percents) {
			if (from > 1) {
				from /= 100;
			}
			if (to > 1) {
				to /= 100;
			}
		}
		HealthBarArea area = HealthBarArea(color, healthBarAreaType, from, to);
		areas.push_back(area);
	}

	void AddArea(float from, float to, ImColor color, HealthBarAreaType healthBarAreaType = HealthBarAreaType::Numeric) {
		if (healthBarAreaType == HealthBarAreaType::Percents) {
			if (from > 1) {
				from /= 100;
			}
			if (to > 1) {
				to /= 100;
			}
		}
		HealthBarArea area = HealthBarArea(color, healthBarAreaType, from, to);
		areas.push_back(area);
	}

	void Draw(Vector3 pos, float currentHp, float maxHp, bool center = true) {
		const float yOffset = 0;

		float drawX;
		float drawY;

		Vector3 leftTop;
		Vector3 rightTop;
		Vector3 leftBottom;
		Vector3 rightBottom;

		Vector3 w2sP;

		Function::World2Screen(&pos, &w2sP);

		/*if (this->Type == HealthBarType::UnderHero_3D) {
			Vector3 newPos;
			if (center) {
				newPos = Vector3(pos.x - this->Width / (1 + w2sP.y / 1920), pos.y, pos.z);
				Functions.WorldToScreen(&newPos, &leftTop);
				newPos = Vector3(pos.x + this->Width / (1 + w2sP.y / 1920), pos.y, pos.z);
				Functions.WorldToScreen(&newPos, &rightTop);

				newPos = Vector3(pos.x - this->Width / (1 + w2sP.y / 1920), pos.y + this->Height * 2, pos.z);
				Functions.WorldToScreen(&newPos, &leftBottom);
				newPos = Vector3(pos.x + this->Width / (1 + w2sP.y / 1920), pos.y + this->Height * 2, pos.z);
				Functions.WorldToScreen(&newPos, &rightBottom);
			}
			else {

			}
		}
		else */
		{
			Vector3 w2s;
			Function::World2Screen(&pos, &w2s);
			if (center) {
				drawX = w2s.x - this->Width / 2;
				//	drawY = w2s.y + yOffset;
			}
			else {
				drawX = w2s.x;
				//drawY = w2s.y + yOffset;
			}
			drawY = w2s.y + yOffset;
		}

		// TODO: 3D Optimization

		if (drawX > Render::RenderWidth || drawY > Render::RenderHeight)
			return;
		if (drawX + this->Width < 0 || drawY + this->Height < 0)
			return;

		switch (this->Type) {
		case HealthBarType::UnderHero: {
			Render::Draw_FilledRectangle(drawX, drawY, drawX + this->Width, drawY + this->Height, ImColor::ImColor(0, 0, 0));
			float hpWidth = currentHp / maxHp * this->Width;

			float hpCoof = 1;
			if (currentHp > maxHp)
			{
				hpCoof = this->Width / hpWidth;
				hpWidth = this->Width;
			}

			Render::Draw_FilledRectangle(drawX, drawY, drawX + hpWidth, drawY + this->Height, this->HpColor);

			//for (auto area : areas) {
			//	
			//	Renderer::Draw_FilledRectangle(drawX1, drawY1, drawX1 + hpWidth, drawY1 + this->Height, this->HpColor);
			//}

			float rightAreaOffset = 0;
			for (auto area : rightAreas) {
				float leftX = hpWidth - area.Size / maxHp * this->Width * hpCoof;
				leftX -= rightAreaOffset;
				if (leftX < 0)
					leftX = 0;
				float rightX = hpWidth - rightAreaOffset;
				if (rightX < 0)
					rightX = 0;
				rightAreaOffset += rightX - leftX;
				/*Renderer::Draw_Text(drawX, drawY + abs(rightAreaOffset), to_string(area.Size) + "   -   " + to_string(rightAreaOffset));*/
				Render::Draw_FilledRectangle(drawX + leftX, drawY, drawX + rightX, drawY + this->Height, area.Color);
			}

			std::sort(leftAreas.begin(), leftAreas.end(), this->sortLeftAreasFunc);	 // `cause left side is fixed, it need sort from bigest to smallest to display all

			for (auto area : leftAreas) {
				float rightX = area.Size / maxHp * this->Width * hpCoof;
				if (rightX > hpWidth)
					rightX = hpWidth;
				Render::Draw_FilledRectangle(drawX, drawY, drawX + rightX, drawY + this->Height, area.Color);
			}

			break;
		}
		}
	}

private:
	static bool sortLeftAreasFunc(HealthBarArea area1, HealthBarArea area2) {
		return (area1.Size > area2.Size);
	}

	std::vector<HealthBarArea> areas;
	std::vector<HealthBarArea> leftAreas;
	std::vector<HealthBarArea> rightAreas;
	HealthBarType Type;
	float Width;
	float Height;

	ImColor HpColor = ImColor::ImColor(39, 174, 96);
};