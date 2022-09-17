#pragma once
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Functions.h"
#include "../Render.h"
#include "../ObjectManager.h"
#include "../Helper.h"

namespace Akali {
	static bool SkillDamage = false;

	void OnDraw();
	void OnMenu();
	void Initialize();
}
