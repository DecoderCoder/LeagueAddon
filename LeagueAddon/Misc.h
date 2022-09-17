#pragma once
#include "EventManager/EventManager.h"
#include "ObjectManager.h"
#include "ImGui/imgui.h"
#include "Functions.h"


namespace Misc {
	inline bool AutoSmite = true;
	inline bool AntiAFK = true;

	void Initialize();

	void OnThread();
	void OnMenu();
	void OnDraw();
}
