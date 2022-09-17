#pragma once
#include <Windows.h>
#include "../Offset.h"
#include "../Vector.h"
#include "../Utils.h"
#include "Item.h"

class HeroInventory {
public:
	Item* GetItemByID(int ID) {
		DWORD spellSlotPointerBuffer[12] = {};
		memcpy(&spellSlotPointerBuffer, this, sizeof(DWORD) * 11);
		return (Item*)(*(DWORD*)(spellSlotPointerBuffer[ID] + 0xC));
	}

	bool HasItem(int ID) {
		for (int i = 0; i < 11; i++) {
			auto item = this->GetItemByID(i);
			if (item != 0)
				if (item->Info->ID == ID) {
					return true;
				}
		}
		return false;
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