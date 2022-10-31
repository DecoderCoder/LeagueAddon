#pragma once
#include "Vector.h"

namespace GameLocation {
	inline const Vector3 RadiantBase = Vector3(394, 182, 462);
	inline const Vector3 DireBase = Vector3(14340, 172, 14390);

	//inline const Vector3 RadiantBaseARAM = Vector3();
	//inline const Vector3 DireBaseARAM = Vector3();
}

enum class kSpellSlot
{
	SpellSlot_Unknown = -1,
	SpellSlot_Q,
	SpellSlot_W,
	SpellSlot_E,
	SpellSlot_R,
	SpellSlot_Summoner1,
	SpellSlot_Summoner2,
	SpellSlot_Item1,
	SpellSlot_Item2,
	SpellSlot_Item3,
	SpellSlot_Item4,
	SpellSlot_Item5,
	SpellSlot_Item6,
	SpellSlot_Trinket,
	SpellSlot_Recall,
	SpellSlot_SpecialAttack = 45,
	SpellSlot_BasicAttack1 = 64,
	SpellSlot_BasicAttack2 = 65,
};

enum class CombatType
{
	Any,
	Melee,
	Ranged
};

enum class DamageType
{
	Physical = 0,
	Magical = 1,
	True = 2,
};

enum class TargetingMode
{
	AutoPriority,

	LowHP,

	LowEfficientHP,

	MostAD,

	MostAP,

	Closest,

	NearMouse,

	LessAttack,

	LessCast,

	MostStack
};

enum class PrintChatType {
	WithTime = 0xFFFFFFFF,
	WithoutTime = 0
};

enum class kRecallState
{
	None = 0,
	Odin_Recall = 6,
	Super_Recall = 11,
	Teleporting = 16,
	Stand_United = 19,
	Yuumi_W_Ally = 10
};

enum class IsNotWallFlags {
	Ground = 0x0,
	Ground2 = 0x800000,
	wall = 0x20000,
	Wall2 = 0x820000,
	Bush = 0x10000,
	Bush2 = 0x810000,
	Structure = 0x460000
};

static const char* WardTypeName[] = { "YellowTrinket", "BlueTrinket", "JammerDevice", "ShacoBox", "TeemoShroom", "NidaliTrap", "CaytlinTrap", "JinxTrap", "JhinTrap", "MaokaiTrap"};

enum class WardType {
	Yellow = 0,
	Blue = 1,
	JammerDevice = 2,
	ShacoBox = 3,
	TeemoTrap = 4, // no 
	NidaliTrap = 5, // no
	CaytlinTrap = 6, // no
	JinxTrap = 7,
	JhinTrap = 8,
	MaokaiTrap = 9,
};