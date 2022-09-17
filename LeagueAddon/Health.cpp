#include "Health.h"
#include "Functions.h"
#include "ObjectManager.h"
#include "Helper.h"
#include "EventManager/EventManager.h"

bool HealthPrediction::IsAutoAttack(std::string name)
{
	if (ToLower(name).find("attack") != std::string::npos)
		return true;
	return false;
}

float HealthPrediction::GetHealthPrediction(GameObject* unit, int time, int delay)
{
	float predictedDamage = 0.0f;

	for (auto attack : ActiveAttacks)
	{
		float attackDamage = 0.0f;
		if (attack.second->target->IsInRange(Local, FLT_MAX, false) && attack.second->target->NetworkID == unit->NetworkID)
		{
			float bonding = max(attack.second->target->GetBoundingRadius(), unit->Position.Distance(attack.second->startPos) - attack.second->source->GetBoundingRadius());
			if (attack.second->source->combat == CombatType::Melee)
				bonding = 0;

			float landTime = attack.second->startTick + attack.second->delay + 1000 * bonding / attack.second->projectileSpeed + delay;

			if (landTime < Function::GameTimeTick() + time)
				attackDamage = attack.second->damage;
		}
		predictedDamage += attackDamage;
	}

	return unit->Health + (unit->Armor + unit->BonusArmor) - predictedDamage;
}

float HealthPrediction::LaneClearHealthPrediction(GameObject* unit, int time, int delay)
{
	float predictedDamage = 0.0f;

	for (auto attack : ActiveAttacks)
	{
		int n = 0;
		if (Function::GameTimeTick() - 100 <= attack.second->startTick + attack.second->animationTime && attack.second->target->IsInRange(Local, FLT_MAX, false) && attack.second->source->IsInRange(Local, FLT_MAX, false) && attack.second->target->NetworkID == unit->NetworkID)
		{
			float fromT = attack.second->startTick;
			float toT = Function::GameTimeTick() + time;

			while (fromT < toT)
			{
				if (fromT >= Function::GameTimeTick() && (fromT + attack.second->delay + max(0, unit->Position.Distance(attack.second->source->Position) - attack.second->source->GetBoundingRadius() / 2) / attack.second->projectileSpeed < toT))
					n++;
				fromT += (int)attack.second->animationTime;
			}
		}
		predictedDamage += n * attack.second->damage;

	}

	return unit->Health + (unit->Armor + unit->BonusArmor) - predictedDamage;
}

void HealthPrediction::OnGameUpdate()
{
	for (auto pair : ActiveAttacks)
	{
		if (pair.second->startTick < Function::GameTimeTick() - 3000)
			ActiveAttacks.erase(pair.first);
	}
}

void HealthPrediction::OnDraw()
{
	OnGameUpdate();
	//console.Print("%d", ActiveAttacks.size());
}


void HealthPrediction::OnProcessSpell(void* spellBook, SpellInfo* castInfo) {
	auto caster = ObjectManager::FindObjectByIndex(castInfo->source_id);
	if (caster->Team != Local->Team || Helper::isValidUnit(caster, 3000, false) || (!Function::IsMinion(caster) || !Function::IsTurret(caster)))
		return;
	auto target = ObjectManager::FindObjectByIndex(castInfo->targetIndex());

	if (!target || target && !Helper::isValidUnit(target))
		return;

	//console.Print("%s", caster->GetChampionName().c_str());

	ActiveAttacks.erase(caster->NetworkID);



	auto data = new PredictedDamage(
		caster,
		target,
		caster->Position,
		Function::GameTimeTick() - Function::GetPing() / 2,
		Function::GetAttackCastDelay(caster) * 1000,
		Function::GetAttackDelay(caster) * 1000 + (Function::IsTurret(caster) ? 70 : 0),
		caster->combat == CombatType::Melee ? FLT_MAX : (int)castInfo->BasicAttackSpellData->Resource->MissileSpeed,
		caster->BaseAttackDamage + caster->BonusAttackDamage
	);

	ActiveAttacks.insert({ caster->NetworkID, data });

}

void HealthPrediction::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnProcessSpell, OnProcessSpell);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, OnDraw);

	EventManager::AddEventHandler(EventManager::EventType::OnLoad, OnLoad);
	EventManager::AddEventHandler(EventManager::EventType::OnUnLoad, OnUnload);
}

void HealthPrediction::OnLoad()
{

}

void HealthPrediction::OnUnload()
{
	EventManager::RemoveEventHandler(EventManager::EventType::OnProcessSpell, OnProcessSpell);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, OnDraw);
}