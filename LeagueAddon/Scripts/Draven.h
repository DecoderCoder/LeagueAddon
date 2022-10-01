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

namespace Draven {
	inline int prevAxesCount = 0;
	inline bool autoW = false;

	void OnDraw();
	void OnMenu();
	void Initialize();
}