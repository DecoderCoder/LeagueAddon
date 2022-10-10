#pragma once
#include "ObjectManager.h"
#include "Utils.h"
#include "ImGui/ImGui.h"
#include "EventManager/EventManager.h"
#include "Functions.h"
#include "Scripts.h"
#include "SkinChanger.h"
#include "Visual.h"

namespace Debug {
	inline bool Enabled = false;

	inline std::vector<SpellInfo> detectedSpells;
	inline std::vector<uintptr_t> detectedSpellsPtr;

	void Initialize();
	void OnDraw();
	void OnMenu();
	void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
}