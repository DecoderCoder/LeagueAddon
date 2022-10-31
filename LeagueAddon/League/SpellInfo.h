#pragma once
#include "SpellData.h"

class CSpellInfo
{
public:
	CSpellData* GetSpellData() {
		return *(CSpellData**)((DWORD)this + Offset::SpellInfo::SpellData);
	}
};