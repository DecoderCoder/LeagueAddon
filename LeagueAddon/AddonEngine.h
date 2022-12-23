#pragma once
#include "Render.h"
#include "Utils.h"
#include "EventManager/EventManager.h"
#include "Render.h"
#include "ObjectManager.h"
#include "Functions.h"
#include "Scripts.h"
#include "TargetSelector.h"
#include "Hooks/Hooks.h"
#include "psapi.h"
#include <thread>
#include "winternl.h"
#include "Evade/Evade.h"
#include "JustEvade/JustEvade.h"
#include "Visual.h"
#include "Debug.h"
#include "SkinChanger.h"
#include "Misc.h"

namespace AddonEngine {
	bool Initialize();
	void Uninitialize();
}