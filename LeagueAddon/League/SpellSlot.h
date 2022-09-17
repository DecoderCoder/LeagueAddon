#pragma once
#include "../Vector.h"
#include "../Utils.h"
#include "SpellInfo.h"
#include "../Offset.h"

class CSpellSlot {
public:
	int GetLevel() {
		if (IsBadReadPtr((int*)(this + Offset::SpellSlot::Level), 4))
			return 0;
		return *(int*)((DWORD)this + Offset::SpellSlot::Level);
	}

	float GetTime() {
		if (IsBadReadPtr((int*)(this + Offset::SpellSlot::Time), 4))
			return 0;
		return *(float*)((DWORD)this + Offset::SpellSlot::Time);
	}

	float GetCD() {
		float gameTime = *(float*)(baseAddr + Offset::Data::GameTime);
		if (GetTime() - gameTime > 0)
			return GetTime() - gameTime;
		return 0;
	}

	bool IsReady() {
		if (GetTime() <= *(float*)(baseAddr + Offset::Data::GameTime))
			return true;
		return false;
	}

	std::string GetName() {
		return this->GetSpellInfo()->GetSpellData()->GetSpellName();
	}


	CSpellInfo* GetSpellInfo() {
		return *(CSpellInfo**)((DWORD)this + Offset::SpellSlot::SpellInfo);
	}

	int GetCharges() {
		return *(int*)((DWORD)this + Offset::SpellSlot::Charges);
	}

	float GetChargeTime() {
		return *(float*)((DWORD)this + Offset::SpellSlot::TimeCharge);
	}

	int GetDamage() {
		return *(float*)((DWORD)this + Offset::SpellSlot::Damage);
	}

};