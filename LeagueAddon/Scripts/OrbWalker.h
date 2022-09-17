#pragma once
#include "../Offset.h"
#include "../Utils.h"
#include "../ObjectManager.h"
#include "../Helper.h"
#include "../Health.h"

namespace OrbWalker {
	inline std::list<GameObject*> MinionListAA;

	enum class OrbwalkingMode
	{
		LastHit,
		Mixed,
		LaneClear,
		Combo,
		Freeze,
		CustomMode,
		None
	};

	enum TargetType {
		LowestHealth,
	};

	inline GameObject* _prevMinion = nullptr;
	inline int DelayOnFire = 0;
	inline unsigned int DelayOnFireId = 0;
	inline int BrainFarmInt = -100;
	inline float LaneClearWaitTimeMod = 2.f;
	inline OrbwalkingMode _mode = OrbwalkingMode::None;
	inline TargetingMode _targetingMode = TargetingMode::LowHP;
	
	inline bool enabled = false;
	inline bool selectedPriority = false;

	inline bool _working = false;

	inline bool useIssueOrder = true;
	inline bool lockCamera = true;

	inline float LastAttackCommandT;
	inline float LastMoveCommandT;

	float GetMyProjectileSpeed();

	bool ShouldWait();

	bool CanAttack();
	bool CanMove(float extraWindup);
	GameObject* GetTarget();
	void Orbwalk(GameObject* target, float extraWindup = 0.0f);
	void OnUpdate();
	void OnDraw();
	void OnMenu();
	void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
	void OnDeleteObject(void* thisPtr, GameObject* obj);
	void Initialize();
	void OnLoad();
	void OnUnload();

	std::string AttackResets[];
}