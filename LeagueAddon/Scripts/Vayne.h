#pragma once
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Functions.h"
#include "../Render.h"
#include "../ObjectManager.h"
#include "../HealthBar.h"
#include "../Helper.h"
#include "../Input.h"
#include "OrbWalker.h"
#include "../TargetSelector.h"

namespace Vayne {
	inline bool autoE = false;
	inline bool autoEHighChance = false;

	void OnDraw();
	void OnMenu();
	void Initialize();
}