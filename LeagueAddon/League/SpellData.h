#pragma once
#include "../Utils.h"
class CSpellData
{
public:
	std::string GetSpellName() {
		char buff[50];
		memcpy(&buff, (void*)(*(DWORD*)((DWORD)this + Offset::SpellData::SpellName)), 50);
		return buff;
	}

	float GetCooldownTime(int lvl = 0) {
		return *(float*)((DWORD)this + Offset::SpellData::CooldownTime + (lvl - 1) * sizeof(float));
	}

	CSpellData* GetSpellData() {
		return *(CSpellData**)((DWORD)this + 0x44);
	}
};