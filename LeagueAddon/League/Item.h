#pragma once
#include <Windows.h>
#include "../Offset.h"
#include "../Vector.h"
#include "../Utils.h"
#include "Item.h"
#include "ItemInfo.h"

class Item {
public:
	union {
		DEFINE_MEMBER_N(char* Name, Offset::Inventory::ItemSlotSpellName);
		DEFINE_MEMBER_N(float Price, Offset::Inventory::ItemSlotPrice);
		DEFINE_MEMBER_N(ItemInfo* Info, Offset::Inventory::ItemSlotInfo);
	};
};