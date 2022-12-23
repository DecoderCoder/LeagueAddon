#pragma once
#include "ObjectManager.h"
#include "Utils.h"
#include "ImGui/ImGui.h"
#include "EventManager/EventManager.h"
#include "Functions.h"
#include "Scripts.h"
#include "SkinChanger.h"
#include "Visual.h"
#include "JustEvade/JustEvade.h"

namespace Debug {
	inline clock_t drawStartTime = 0;
	inline clock_t drawEndTime = 0;

	inline clock_t avarageDrawTime = 0;

	inline bool Enabled = false;


	inline bool drawSpellDirection = false;
	inline bool drawObjectInfo = false;
	inline bool drawObjectInfoBuffs = false;

	inline std::vector<SpellInfo> detectedSpells;
	inline std::vector<uintptr_t> detectedSpellsPtr;

	void Initialize();
	void OnDraw();
	void OnMenu();
	void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
}