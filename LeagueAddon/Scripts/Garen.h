#pragma once
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Functions.h"
#include "../Render.h"
#include "../ObjectManager.h"

namespace Garen {
	static bool AutoCast = false;
	static bool SkillDamage = false;
	static double Last_E_Pressed = 0;

	void OnDraw();
	void OnMenu();
	void Initialize();
}
