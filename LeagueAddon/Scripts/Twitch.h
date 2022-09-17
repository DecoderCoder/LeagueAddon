#pragma once
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Functions.h"
#include "../Render.h"
#include "../ObjectManager.h"
#include "../HealthBar.h"
#include "../Helper.h"

namespace Twitch {
	inline bool _eDamage = false;

	void OnDraw();
	void OnMenu();
	void Initialize();
}