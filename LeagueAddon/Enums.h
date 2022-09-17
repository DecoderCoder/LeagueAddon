#pragma once
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