#include "Helper.h"

bool Helper::isValidUnit(GameObject* unit, float range, bool checkTeam, Vector3 from)
{
	if (unit == NULL || !unit->IsVisible || !Function::IsAlive(unit) || !unit->IsTargetable
		|| unit->IsInvulnearable(Local)
		|| unit->BuffManager.hasBuff("NilahWBuff")
		|| unit->BuffManager.hasBuff("NilahWAllyBuff")
		|| (unit->Health < 150 && unit->BuffManager.hasBuff("UndyingRage")) // Tryndamere ult
		|| unit->BuffManager.hasBuff("NilahWAllyBuff"))
	{
		return false;
	}

	if (checkTeam && unit->Team == Local->Team)
	{
		return false;
	}

	if (unit->GetChampionName() == "WardCorpse")
	{
		return false;
	}
	(from.IsValid() ? from : Local->Position).DistanceSquared(unit->Position) > range* range;
	return !(range < FLT_MAX) || (from.IsValid() ? from : Local->Position).DistanceSquared(unit->Position) > range * range;
}

float Helper::CalcDamage(GameObject* from, GameObject* to, bool isAA, float damage, DamageType damageType) {

	float totalDamage;
	return 0;
}

float Helper::calcReducedDamage(float rawDamage, float armor, float penetration, float lethality, bool calc0armor)
{
	//1. Armor reduction, flat
	//2. Armor reduction, percentage
	//3. Armor penetration, percentage
	//4. Lethality


	float reduced;
	armor = armor * penetration - lethality;

	if (!calc0armor && armor < 0)
		armor = 0;

	if (armor >= 0) {
		reduced = rawDamage * (100 / (100 + armor));
	}
	else {
		reduced = rawDamage * (2 - (100 / (100 - armor)));
	}
	return reduced;
}

float Helper::CalcAutoAttackDamage(GameObject* from, GameObject* to) {
	HeroInventory* inventory = &from->Inventory;
	BuffManager* buffMng = &from->BuffManager;

	const float baseAttack = from->BaseAttackDamage;
	const float addAttack = from->BonusAttackDamage;
	float totalDmg = 0;
	float physDmg = baseAttack + addAttack;
	float magDmg = 0;

	return physDmg;

	// Kraken // 6672buff
	if (buffMng->hasBuff("6672buff") && buffMng->getBuff("6672buff")->getCount() == 2) {
		totalDmg += 50 + addAttack * 0.4;
	}

	// Сияние
	if (buffMng->hasBuff("sheen")) {
		physDmg += baseAttack;
	}

	// Крушитель богов
	if (buffMng->hasBuff("6632buff")) {
		physDmg += baseAttack * 1.25 + (to->MaxHealth * 0.06);
	}

	// Тройственный союз
	if (buffMng->hasBuff("3070trinityforce")) {
		physDmg += baseAttack * 2;
	}

	// Скорострельная пушка и Осколок кирхайс
	if (buffMng->hasBuff("itemstatikshankcharge")) {
		if (inventory->HasItem(3094)) {
			magDmg += 120;
		}
		if (inventory->HasItem(2015)) {
			magDmg += 80;
		}
	}

	if (Function::IsMinion(to)) {
		// Колчан
		if (inventory->HasItem(6670)) {
			physDmg += 20;
		}

		// Начальные предметы (Слеза богини, Кольцо Дорана, Щит Дорана)
		/*если купить например щит дорана, то ты нанесешь крипам на 5 урона больше
			если купить щит дорана и слезу богини - то всё равно нанесешь только на 5 урона больше т.к.не стакаются


			А если купить Щит Дорана, Кольцо Дорана и Слезу богини - то ты нанесешь на 0 урона больше вообще ахахах*/
			// int CountCauseBug = 0
		if (inventory->HasItem(3070) || inventory->HasItem(1056) || inventory->HasItem(1054)) {
			physDmg += 5;
		}
	}

	// Зуб нашора
	if (inventory->HasItem(3115)) {
		magDmg += 15 + from->GetAbilityPower() * 0.2;
	}

	//  Клинок падшего короля
	if (inventory->HasItem(3153)) {

		physDmg += to->Health * (from->combat == CombatType::Melee ? 0.12 : 0.08);
	}

	//  Титаническая гидра
	if (inventory->HasItem(3748)) {
		physDmg += 4 + from->Health * (from->combat == CombatType::Melee ? 0.15 : 0.125);
	}



	// Смерть разума TODO
	// 1-9 = 15 | 10 - 35 | 11 - 45 | 12 - 55 | 13 - 65 | 14 - 75 | 15 - 76 | 16 - 78 | 17 - 79 | 18 - 80
	/*if (inventory->HasItem(3091)) {
		if(from->level)
	}*/

	//  Поклон лорда доминика TODO
	//if (inventory->HasItem(3748)) {
	//	physDmg += 4 + from->Health * (from->combat == CombatType::Melee ? 0.15 : 0.125);
	//}

	// If Crit Damage == 100%
	//if () {

	//}

	totalDmg += calcReducedDamage(physDmg, to->Armor, from->ArmorPenMod, from->ArmorPen);
	totalDmg += calcReducedDamage(magDmg, to->MagicRes, from->MagicPenMod, from->MagicPenFlat);

	return totalDmg;
}