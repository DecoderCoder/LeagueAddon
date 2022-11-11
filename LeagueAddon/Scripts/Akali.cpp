#include "Akali.h"
#include "../HealthBar.h"
#include "../Input.h"
#include "windows.h"
#include "../Utils.h"

bool has8Rune = true;

void Akali::OnMenu() {
	if (ImGui::CollapsingHeader("Akali")) {
		ImGui::Checkbox("Show skill Damage", &SkillDamage);
		if (SkillDamage) {
			ImGui::Checkbox("has 8 Rune", &has8Rune);
		}
		ImGui::Separator();
	}
}

//GameObject* FindHoveredHero() {
//	int heroIndex = *(int*)(*(uintptr_t*)(baseAddr + Offset::Data::HudInstance) + 0x1C0);
//	int heroIndexPractice = *(int*)(*(uintptr_t*)(baseAddr + Offset::Data::HudInstance) + 0x13C); // pointer in Practice mode
//
//	for (auto obj : ObjectManager::HeroList()) {
//		if (obj->Index == heroIndex || obj->Index == heroIndexPractice) {
//
//			return obj;
//		}
//			
//	}
//	return NULL;
//}
//
//bool IsBadHeroToUlt(GameObject* hero) {
//	BuffManager* buffMng = &hero->BuffManager;
//	if (
//		hero->IsTargetable
//		&& !hero->IsInvulnearable
//		&& !buffMng->hasBuff("UndyingRage")
//		&& !buffMng->hasBuff("sionpassivezombie")
//		)
//		return false;
//	return true;
//}

void Akali::OnDraw() {
	int ultLvl = Local->SpellBook.GetSpellSlotByID(3)->GetLevel();
	Render::BeginOverlay();
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(Render::RenderWidth, Render::RenderHeight));

	for (auto obj : ObjectManager::HeroList()) {
		float health = obj->Health;
		float shield = obj->Shield;
		float maxHealth = obj->MaxHealth;
		Vector3 pos = obj->Position;
		//Vector3 w2sPos = obj.GetHpBarPosition();
		//Vector3 HpBarPosition = obj.GetHpBarPosition();
		Vector3 w2sPos = Function::WorldToScreen(&pos);

		HealthBar hpBar = HealthBar(HealthBarType::UnderHero);



		if (obj->isValid() && obj->IsEnemyTo(Local) && Function::IsAlive(obj) && obj->IsVisible) {
			if (SkillDamage) {
				for (auto obj : ObjectManager::HeroList()) {
					if (obj->IsVisible && obj->IsEnemyTo(Local) && Function::IsAlive(obj)) {
						float maxHP = obj->MaxHealth;
						float currentHP = obj->Health;
						HealthBar healthBar = HealthBar(HealthBarType::UnderHero);

						{ // R
							CSpellSlot* spellR = Local->SpellBook.GetSpellSlotByID(3);
							if (spellR->GetLevel() > 0 && spellR->IsReady()) {
								float abilityPower = Local->GetAbilityPower();

								//float minBaseDmg = 80 + 140 * (spellR->GetLevel() - 1);
								float maxBaseDmg = (60 + 70 * (spellR->GetLevel() - 1)) * 3;

								float additionalMinDamage = abilityPower * 0.3;
								float additionalMaxDamage = abilityPower * 0.9;

								float maxDmg = maxBaseDmg + additionalMaxDamage;
								float minDmg = maxDmg / 3;

								float hpPercent = currentHP / maxHP;
								if (hpPercent < 0.30)
									hpPercent = 0.30;
								float damagePercent = abs((((hpPercent - 0.30) * (1 - 0)) / (1 - 0.3)) - 1);

								float damage = (((damagePercent - 0) * (maxDmg - minDmg)) / (1 - 0)) + minDmg;

								if (has8Rune && hpPercent < 0.40) {
									damage += damage * 0.08;
								}
								float penFlat = Local->MagicPenFlat;
								if (Local->Inventory.HasItem(4645)) {
									float pen = ((currentHP - 1000) * (10 - 20)) / (2500 - 1000) + 20;
									if (pen < 10)
										pen = 10;
									if (pen > 20)
										pen = 20;
									penFlat += pen;
								}

								float reducedDamage = Helper::calcReducedDamage(damage, obj->MagicRes, Local->MagicPenMod, penFlat, false);

								if (obj->Health + obj->Shield < reducedDamage)
									healthBar.AddRightArea(reducedDamage, ImColor(231, 76, 60));
								else
									if (damagePercent > 0.95)
										healthBar.AddRightArea(reducedDamage, ImColor(241, 196, 15));
									else
										healthBar.AddRightArea(reducedDamage, ImColor(52, 152, 219));

								Vector3 w2s;
								Function::World2Screen(&obj->Position, &w2s);
							}
						}

						healthBar.Draw(obj->Position, obj->Health + obj->Shield, obj->MaxHealth);

						
					}
				}
			}





			//float damage = hp + ((maxHealth - health) / 100 * percents);
			//if (damage < 0)
			//	damage = 0;

			//float dmg = 0;
			//float dhp = maxHealth;

			//if (!CheatEngine::PerformanceMode) {
				/*for (dhp = maxHealth; (dhp + 0) > dmg; dhp--) {
					dmg = hp + ((maxHealth - dhp) / 100 * percents);
				}*/
				//}
				//if ((health + shield) < damage) {
				//	hpBar.AddLeftArea(health + shield, ImColor::ImColor(192, 57, 43));
				//	hpBar.Draw(pos, health, maxHealth);
				//	//if (AutoCast && Local->SpellBook.GetSpellSlotByID(3)->IsReady()) {
				//	//	GameObject* hovered = FindHoveredHero();
				//	//	if (hovered != NULL && hovered->NetworkID == obj->NetworkID)
				//	//	{
				//	//		if (Local->Position.Distance(pos) <= 400 && !IsBadHeroToUlt(obj) && Last_R_Pressed < GetTickCount()) { // Akali Ult distance 384
				//	//			Input::PressKey(HKey::R);
				//	//			Last_R_Pressed = GetTickCount() + 80;
				//	//		}
				//	//	}
				//	//}
				//}
				//else {
				//	//if (SkillDamage) {
				//	//	CSpellSlot* QSpell = Local->SpellBook.GetSpellSlotByID(0);
				//	//	CSpellSlot* ESpell = Local->SpellBook.GetSpellSlotByID(2);

				//	//	int drawOffset = 0;
				//	//	if (QSpell->GetLevel() > 0 && QSpell->IsReady()) { // Q Damage
				//	//		int baseDamage = 0;
				//	//		int summaryDamage = 0;
				//	//		switch (QSpell->GetLevel()) {
				//	//		case 1:
				//	//		{
				//	//			baseDamage = 30;
				//	//			break;
				//	//		}
				//	//		case 2:
				//	//		{
				//	//			baseDamage = 60;
				//	//			break;
				//	//		}
				//	//		case 3:
				//	//		{
				//	//			baseDamage = 90;
				//	//			break;
				//	//		}
				//	//		case 4:
				//	//		{
				//	//			baseDamage = 120;
				//	//			break;
				//	//		}
				//	//		case 5:
				//	//		{
				//	//			baseDamage = 150;
				//	//			break;
				//	//		}
				//	//		}

				//	//		summaryDamage = GetDamageWithArmor(baseDamage + (Local->BaseAttackDamage + Local->BonusAttackDamage) * 1.50, obj->Armor);
				//	//		hpBar.AddRightArea(summaryDamage, ImColor::ImColor(243, 156, 18));
				//	//	}



				//		//if (ESpell->GetLevel() > 0 && ESpell->IsReady() || Last_E_Pressed + 3 >= Function::GameTime()) { // E Damage
				//		//	const int duration = 3;

				//		//	float coefficient = 0;
				//		//	float coefficientN = 0;
				//		//	int ticks = 7;
				//		//	int baseDamage = 0;
				//		//	int baseDamageN = 0;
				//		//	int summaryDamage = 0;
				//		//	int summaryDamageN = 0;
				//		//	switch (ESpell->GetLevel()) {
				//		//	case 1:
				//		//	{
				//		//		baseDamage = 4;
				//		//		baseDamageN = 5;
				//		//		coefficient = 32;
				//		//		coefficientN = 40;
				//		//		break;
				//		//	}
				//		//	case 2:
				//		//	{
				//		//		baseDamage = 8;
				//		//		baseDamageN = 10;
				//		//		coefficient = 34;
				//		//		coefficientN = 42.5;
				//		//		break;
				//		//	}
				//		//	case 3:
				//		//	{
				//		//		baseDamage = 12;
				//		//		baseDamageN = 15;
				//		//		coefficient = 36;
				//		//		coefficientN = 45;
				//		//		break;
				//		//	}
				//		//	case 4:
				//		//	{
				//		//		baseDamage = 16;
				//		//		baseDamageN = 20;
				//		//		coefficient = 38;
				//		//		coefficientN = 47.5;
				//		//		break;
				//		//	}
				//		//	case 5:
				//		//	{
				//		//		baseDamage = 20;
				//		//		baseDamageN = 25;
				//		//		coefficient = 40;
				//		//		coefficientN = 50;
				//		//		break;
				//		//	}
				//		//	}
				//		//	ticks += floor(Local->AttackSpeedMulti * 100 / 25);
				//		//	coefficient /= 100;
				//		//	coefficientN /= 100;
				//		//	const float dmgPerTick = GetDamageWithArmor((baseDamage + (Local->BaseAttackDamage + Local->BonusAttackDamage) * coefficient), obj->Armor);
				//		//	const float dmgPerTickN = GetDamageWithArmor((baseDamageN + (Local->BaseAttackDamage + Local->BonusAttackDamage) * coefficientN), obj->Armor);
				//		//	summaryDamage = dmgPerTick * ticks;
				//		//	summaryDamageN = dmgPerTickN * ticks;
				//		//	if (ESpell->GetSpellInfo()->GetSpellData()->GetSpellName() == "AkaliECancel") {
				//		//		float timePassed = Last_E_Pressed - Function::GameTime();
				//		//		float damageGiven = dmgPerTick * floor(timePassed / duration * ticks);
				//		//		float damageGivenN = dmgPerTickN * floor(timePassed / duration * ticks);
				//		//		summaryDamage += damageGiven;
				//		//		summaryDamageN += damageGivenN;
				//		//	}
				//		//	hpBar.AddRightArea(summaryDamage, ImColor::ImColor(236, 240, 241));
				//		//	hpBar.AddRightArea(-(summaryDamageN - summaryDamage), ImColor::ImColor(127, 140, 141)); // dont know why, but nearest damage is not nearest damage
				//		//}
				//	//}

				//	//if (ultLvl > 0 && Local->SpellBook.GetSpellSlotByID(3)->IsReady())
				//	//	hpBar.AddLeftArea(dhp, ImColor::ImColor(241, 196, 15));
				//	hpBar.Draw(pos, health + shield, maxHealth);
				//}

		}
	}

	Render::EndOverlay();
}

void Akali::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}