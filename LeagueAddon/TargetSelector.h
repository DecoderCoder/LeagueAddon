#pragma once
#include "./ObjectManager.h"
#include <vector>

using namespace std;

namespace TargetSelector
{
	inline const char* targetingModeLabels[] = { "AutoPriority", "LowHP", "LowEfficientHP", "MostAD", "MostAP", "Closest", "NearMouse", "LessAttack", "LessCast", "MostStack" };

	inline std::vector<GameObject*> attackOrder;
	inline TargetingMode mode = TargetingMode::LowHP;

	void Initialize();
	bool checkUnit(GameObject* unit);
	bool IsValidUnit(GameObject* unit, float range = FLT_MAX, bool checkTeam = true, Vector3 from = Vector3());
	GameObject* GetTarget(float range, DamageType damageType, bool ignoreShield = true, Vector3 rangeCheckFrom = Vector3());
	GameObject* GetTarget(GameObject* champion, float range, DamageType damageType, bool ignoreShield = true, Vector3 rangeCheckFrom = Vector3(0, 0, 0));
}