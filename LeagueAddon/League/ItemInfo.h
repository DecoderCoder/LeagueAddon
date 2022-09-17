#include <Windows.h>
#include "../Offset.h"
#include "../Vector.h"
#include "../Utils.h"
#include "Item.h"

class ItemInfo {
public:
	union {
		DEFINE_MEMBER_N(int ID, Offset::Inventory::ItemSlotInfoId);
		DEFINE_MEMBER_N(float Crit, Offset::Inventory::ItemSlotInfoPercentCritChanceMod);
	};


	//bool HasItem(char* itemName) {
	//	for (int i = 4; i < 11; i++) {
	//		if (this->GetSpellSlotByID(i)->GetName() == itemName) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}
};