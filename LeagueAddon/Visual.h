#pragma once
#include "ImGui/imgui.h"
#include "EventManager/EventManager.h"
#include "ObjectManager.h"
#include "Render.h"
#include <map>

namespace Visual {
	inline bool ESP = true;
	inline int ESP_Distance = 3000;

	inline bool SpellTrackerMenu = false;
	inline bool RecallTracker = true;
	inline bool RecallTrackerMenu = true;

	inline bool HighlightSelected = true;

	inline bool ShowAttackRadius = true;
	inline bool ShowBoundingRadius = true;

	inline bool AlwaysVisible = true;
	inline bool ShowLastPos = true;

	void Initialize();
	void Unload();

	void OnThread();
	void OnMenu();
	void OnDraw();
}