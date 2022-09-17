#pragma once
#include "ImGui/imgui.h"
#include "EventManager/EventManager.h"
#include "ObjectManager.h"
#include "Render.h"
#include <map>

namespace Visual {
	inline bool ESP = true;
	inline int ESP_Distance = 2000;

	inline bool HighlightSelected = true;

	inline bool ShowAttackRadius = true;
	inline bool ShowBoundingRadius = true;

	void Initialize();

	void OnThread();
	void OnMenu();
	void OnDraw();
}