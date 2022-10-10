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

namespace Vayne {
	void OnDraw();
	void OnMenu();
	void Initialize();
}