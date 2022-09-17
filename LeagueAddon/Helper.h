#pragma once
#include "Structs.h"
#include "ObjectManager.h"
#include "Functions.h"

namespace Helper
{
	float calcReducedDamage(float rawDamage, float armor, float penetration = 0, float lethality = 0, bool calc0armor = 0);
	float CalcAutoAttackDamage(GameObject* from, GameObject* to);
	bool isValidUnit(GameObject* unit, float range = FLT_MAX, bool checkTeam = true, Vector3 from = Vector3());
};