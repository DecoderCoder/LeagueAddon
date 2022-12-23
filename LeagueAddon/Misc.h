#pragma once
#include "EventManager/EventManager.h"
#include "ObjectManager.h"
#include "ImGui/imgui.h"
#include "Functions.h"
#include "Input.h"
#include "Settings.h"

namespace Misc {
	inline bool AutoSmite = true;
	inline bool AutoSmite_Dragon = true;
	inline bool AutoSmite_Baron = true;
	inline bool AutoSmite_Buff = true;
	inline bool AutoSmite_Crab = true;
	inline bool AutoSmite_HasSecondSmite = true;

	inline bool AntiAFK = false;

	void Initialize();

	void OnThread();
	void OnMenu();
	void OnDraw();
}
