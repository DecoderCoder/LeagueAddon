#pragma once
#include <Windows.h>
#include "../Offset.h"
#include "../Vector.h"
#include "../Utils.h"
#include "SpellSlot.h"

class CSpellBook {
public:

	CSpellSlot* GetSpellSlotByID(int ID) {
		DWORD spellSlotPointerBuffer[7] = {};
		memcpy(&spellSlotPointerBuffer, this, sizeof(DWORD) * 6);
		return (CSpellSlot*)(spellSlotPointerBuffer[ID]);
	}

	//bool HasItem(char* itemName) {
	//	for (int i = 4; i < 11; i++) {
	//		if (this->GetSpellSlotByID(i)->GetName() == itemName) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}
};