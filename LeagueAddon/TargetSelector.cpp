#include "TargetSelector.h"
//#include "../Constants.h"
#include "Helper.h"

bool TargetSelector::checkUnit(GameObject* unit)
{
	if (!Function::IsAlive(unit) || !unit->IsVisible)
		return false;
	if (unit->IsAllyTo(ObjectManager::GetLocalPlayer()))
		return false;

	return true;
}

void TargetSelector::Initialize() {
	attackOrder.clear();
	for (auto hero : ObjectManager::HeroList()) {
		//if (hero->IsEnemyTo(Local)) {
		attackOrder.push_back(hero);
		if (hero->GetChampionName() == "Yasuo")
			hasEnemyYasuo = hero->Index;
		if (hero->GetChampionName() == "Samira")
			hasEnemySamira = hero;

		//}


	}
}

bool TargetSelector::IsValidUnit(GameObject* unit, float range, bool checkTeam, Vector3 from)
{
	if (unit == nullptr || !Function::IsAlive(unit) || !unit->IsVisible)
		return false;
	if (checkTeam && unit->IsAllyTo(ObjectManager::GetLocalPlayer()))
		return false;
	if (unit->ChampionName == "WardCorpse")
		return false;
	if (!ObjectManager::GetLocalPlayer()->IsInRange(unit, range, true))
		false;
	return true;
}

GameObject* TargetSelector::GetTarget(float range, DamageType damageType, bool ignoreShield, Vector3 rangeCheckFrom)
{
	return GetTarget(Local, range, damageType, ignoreShield, rangeCheckFrom);
}

float calcActiveHP(float hp, float armor, float penetration, float lethality) {
	//1. Armor reduction, flat
	//2. Armor reduction, percentage
	//3. Armor penetration, percentage
	//4. Lethality

	armor = armor * penetration - lethality;
	return (1 + armor / 100) * hp;
}

GameObject* TargetSelector::GetTarget(GameObject* champion, float range, DamageType damageType, bool ignoreShield, Vector3 rangeCheckFrom)
{
	GameObject* first = nullptr;
	int prevPriority = attackOrder.size();
	TargetingMode prevTargetMode = mode;
	std::list<GameObject*> objects = filter(ObjectManager::HeroList(), [&](GameObject* enemy) { return Helper::isValidUnit(enemy) && enemy->IsInRange(Local, range, true); });

	Render::BeginOverlay();
	if (!IgnoreMissileShield && (hasEnemyYasuo || hasEnemySamira)) {
		std::vector<Geometry::Polygon> shields;
		if (hasEnemyYasuo) {
			auto yasuoShields = filter(ObjectManager::MissileList(), [&](MissileSpellInfo* mis) { return StringCompare(mis->BasicAttackSpellData->Name, "YasuoW_VisualMis") && hasEnemyYasuo == mis->source_id; });
			float shieldWidth = 250 + 70 * ObjectManager::FindObjectByIndex(hasEnemyYasuo)[0].SpellBook.GetSpellSlotByID(1)->GetLevel();

			for (auto shield : yasuoShields) {
				Geometry::Polygon area = Geometry::Rectangle(shield->EndPosition.Extend(shield->StartPosition, shield->BasicAttackSpellData->Resource->Radius * 2), shield->EndPosition, shieldWidth / 2).ToPolygon();
				shields.push_back(area);
			}
		}

		if (hasEnemySamira) {
			if (hasEnemySamira->BuffManager.hasBuff("SamiraW")) {
				auto area = Geometry::Circle(hasEnemySamira->Position, Function::GetBoundingRadius(hasEnemySamira) / 2 + 325).ToPolygon();
				shields.push_back(area);
			}
		}
		
		for (auto shield : shields) {
			Render::Polygon(shield, ImColor(255, 255, 255), 1);
		}

		if (shields.size() > 0) {
			auto it = objects.begin();
			while (it != objects.end()) {
				bool intersec = false;
				for (auto shield : shields) {
					if (Evade::Core::FindIntersections(shield, Local->Position, (*it)->Position).size() > 0) {
						intersec = true;
						break;
					}
				}
				if (intersec) {
					it = objects.erase(it);
				}
				else {
					++it;
				}
			}
		}		
	}

	for (auto h : objects) {
		Render::Draw_Line3D(Local->Position, h->Position, ImColor(255, 0, 0), 1);
	}

	Render::EndOverlay();

	for (GameObject* next : objects)
	{
		switch (mode)
		{
		case TargetingMode::AutoPriority:
			//if (first == nullptr)
			//	first = next;
			//if (first != nullptr)
			for (int i = 0; i < attackOrder.size(); i++) {
				if (next->NetworkID == attackOrder[i]->NetworkID && i < prevPriority) {
					prevPriority = i;
					first = next;
					break;
				}
			}
			break;
		case TargetingMode::LowHP:
			if (first == nullptr)
				first = next;
			if (first != nullptr && first->Health >= next->Health)
				first = next;
			break;
		case TargetingMode::LowEfficientHP:
			if (first == nullptr)
				first = next;
			if (first != nullptr && calcActiveHP(first->Health, first->Armor + first->BonusArmor, first->ArmorPenMod, first->ArmorPen) >= calcActiveHP(next->Health, next->Armor + next->BonusArmor, next->ArmorPenMod, next->ArmorPen))
				first = next;
			break;
		case TargetingMode::MostAD:
			if (first == nullptr)
				first = next;
			if (first != nullptr && first->BaseAttackDamage + first->FlatPhysicalDamageMod > next->BaseAttackDamage + next->FlatPhysicalDamageMod)
				first = next;
			break;
			//case TargetingMode::MostAP:
			//	if (first == nullptr)
			//		first = next;
			//	if (first != nullptr && first->BaseAbilityDamage + first->FlatMagicDamageMod > next->BaseAbilityDamage + next->FlatMagicDamageMod)
			//		first = next;
			//	break;
		case TargetingMode::Closest:
			if (first == nullptr)
				first = next;
			if (first != nullptr && (rangeCheckFrom.IsValid() ? rangeCheckFrom : champion->Position).DistanceSquared(first->Position) >= (rangeCheckFrom.IsValid() ? rangeCheckFrom : champion->Position).DistanceSquared(next->Position))
				first = next;
			break;
		case TargetingMode::NearMouse:
			if (first == nullptr)
				first = next;
			if (first != nullptr && first->Position.DistanceSquared(*Function::GetMouseWorldPosition()) >= next->Position.DistanceSquared(*Function::GetMouseWorldPosition()))
				first = next;
			break;
		};
	}
	if (first == nullptr && mode == TargetingMode::AutoPriority && objects.size() > 0) {
		mode = mode2;
		first = GetTarget(champion, range, damageType, ignoreShield, rangeCheckFrom);
		mode = prevTargetMode;
	}
	return first;
}