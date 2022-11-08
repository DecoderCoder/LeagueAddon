#pragma once
#include "./ObjectManager.h"
#include <vector>
#include "Geometry.h"
#include "Evade/Evade.h"

using namespace std;

namespace TargetSelector
{
	inline bool enemyYasuo = false;
	inline std::list<GameObject*> enemySamira; // URF All4One etc.
	inline bool IgnoreMissileShield = false;

	inline const char* targetingModeLabels[] = { "AutoPriority", "LowHP", "LowEfficientHP", "MostAD", "MostAP", "Closest", "NearMouse", "LessAttack", "LessCast", "MostStack" };

	inline std::vector<GameObject*> attackOrder;
	inline std::vector<GameObject*> attackOrderIgnore;
	inline std::vector<GameObject*> attackOrderLast;
	inline TargetingMode mode = TargetingMode::LowEfficientHP;
	inline TargetingMode mode2 = TargetingMode::LowEfficientHP;

	void Initialize();
	bool checkUnit(GameObject* unit);
	bool IsValidUnit(GameObject* unit, float range = FLT_MAX, bool checkTeam = true, Vector3 from = Vector3());
	GameObject* GetTarget(float range, DamageType damageType, bool ignoreShield = false, Vector3 rangeCheckFrom = Vector3());
	GameObject* GetTarget(GameObject* champion, float range, DamageType damageType, bool ignoreShield = false, Vector3 rangeCheckFrom = Vector3(0, 0, 0));
}