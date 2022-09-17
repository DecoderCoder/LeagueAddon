#pragma once
#include <string>
#include <map>
#include "Structs.h"

struct PredictedDamage {

	GameObject* source;
	GameObject* target;
	Vector3 startPos;
	int startTick;
	float delay;
	float animationTime;
	int projectileSpeed;
	float damage;

	PredictedDamage(GameObject* source, GameObject* target, Vector3 startPos, int startTick, float delay, float animationTime, int projectileSpeed, float damage)
	{
		this->source = source;
		this->target = target;
		this->startPos = startPos;
		this->startTick = startTick;
		this->delay = delay;
		this->animationTime = animationTime;
		this->projectileSpeed = projectileSpeed;
		this->damage = damage;
	};
};

namespace HealthPrediction
{
	inline std::map<unsigned int, PredictedDamage*> ActiveAttacks;

	bool IsAutoAttack(std::string name);

	void OnGameUpdate();
	void OnDraw();
	void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
	void Initialize();
	void OnLoad();
	void OnUnload();

	float GetHealthPrediction(GameObject* unit, int time, int delay = 70);
	float LaneClearHealthPrediction(GameObject* unit, int time, int delay = 70);
};