#include "Garen.h"
#include "../HealthBar.h"
#include "../Input.h"
#include "windows.h"
#include "../Utils.h"

float Last_E_Pressed = 0;
float Last_R_Pressed = 0;

void Garen::OnMenu() {
	if (ImGui::CollapsingHeader("Garen")) {
		ImGui::Checkbox("Ult Autocast", &AutoCast);
		ImGui::Checkbox("Show skill Damage", &SkillDamage);
		ImGui::Separator();
	}
}

GameObject* FindHoveredHero() {
	int heroIndex = *(int*)(*(uintptr_t*)(baseAddr + Offset::Data::HudInstance) + 0x1C0);
	int heroIndexPractice = *(int*)(*(uintptr_t*)(baseAddr + Offset::Data::HudInstance) + 0x13C); // pointer in Practice mode

	for (auto obj : ObjectManager::HeroList()) {
		if (obj->Index == heroIndex || obj->Index == heroIndexPractice) {

			return obj;
		}
			
	}
	return NULL;
}

bool IsBadHeroToUlt(GameObject* hero) {
	BuffManager* buffMng = &hero->BuffManager;
	if (
		hero->IsTargetable
		&& !hero->IsInvulnearable()
		&& !buffMng->hasBuff("UndyingRage")
		&& !buffMng->hasBuff("sionpassivezombie")
		)
		return false;
	return true;
}

void Garen::OnDraw() {
	if (GetAsyncKeyState('E') && Local->SpellBook.GetSpellSlotByID(2)->IsReady()) {
		Last_E_Pressed = Function::GameTime();
	}

	int ultLvl = Local->SpellBook.GetSpellSlotByID(3)->GetLevel();
	Render::BeginOverlay();
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

	for (auto obj : ObjectManager::HeroList()) {
		float health = obj->Health;
		float shield = obj->Shield;
		float maxHealth = obj->MaxHealth;
		Vector3 pos = obj->Position;
		//Vector3 w2sPos = obj.GetHpBarPosition();
		//Vector3 HpBarPosition = obj.GetHpBarPosition();
		Vector3 w2sPos;
		Function::World2Screen(&pos, &w2sPos);

		HealthBar hpBar = HealthBar(HealthBarType::UnderHero);


		if (obj->isValid() && obj->IsEnemyTo(Local) && Function::IsAlive(obj) && obj->IsVisible) {
			float percents = 0;
			float hp = 0;

			if (ultLvl == 1) {
				percents = 25;
				hp = 150;
			}
			else if (ultLvl == 2) {
				percents = 30;
				hp = 300;
			}
			else if (ultLvl == 3) {
				percents = 35;
				hp = 450;
			}

			float damage = hp + ((maxHealth - health) / 100 * percents);
			if (damage < 0)
				damage = 0;

			float dmg = 0;
			float dhp = maxHealth;

			//if (!CheatEngine::PerformanceMode) {
				for (dhp = maxHealth; (dhp + 0) > dmg; dhp--) {
					dmg = hp + ((maxHealth - dhp) / 100 * percents);
				}
			//}
			if ((health + shield) < damage) {
				hpBar.AddLeftArea(health + shield, ImColor::ImColor(192, 57, 43));
				hpBar.Draw(pos, health, maxHealth);
				if (AutoCast && Local->SpellBook.GetSpellSlotByID(3)->IsReady()) {
					GameObject* hovered = FindHoveredHero();
					if (hovered != NULL && hovered->NetworkID == obj->NetworkID)
					{
						if (Local->Position.Distance(pos) <= 400 && !IsBadHeroToUlt(obj) && Last_R_Pressed < GetTickCount()) { // Garen Ult distance 384
							Input::PressKey(HKey::R);
							Last_R_Pressed = GetTickCount() + 80;
						}
					}
				}
			}
			else {
				if (SkillDamage) {
					CSpellSlot* QSpell = Local->SpellBook.GetSpellSlotByID(0);
					CSpellSlot* ESpell = Local->SpellBook.GetSpellSlotByID(2);

					int drawOffset = 0;
					if (QSpell->GetLevel() > 0 && QSpell->IsReady()) { // Q Damage
						int baseDamage = 0;
						int summaryDamage = 0;
						switch (QSpell->GetLevel()) {
						case 1:
						{
							baseDamage = 30;
							break;
						}
						case 2:
						{
							baseDamage = 60;
							break;
						}
						case 3:
						{
							baseDamage = 90;
							break;
						}
						case 4:
						{
							baseDamage = 120;
							break;
						}
						case 5:
						{
							baseDamage = 150;
							break;
						}
						}

						summaryDamage = GetDamageWithArmor(baseDamage + (Local->BaseAttackDamage + Local->BonusAttackDamage) * 1.50, obj->Armor);
						hpBar.AddRightArea(summaryDamage, ImColor::ImColor(243, 156, 18));
					}



					if (ESpell->GetLevel() > 0 && ESpell->IsReady() || Last_E_Pressed + 3 >= Function::GameTime()) { // E Damage
						const int duration = 3;

						float coefficient = 0;
						float coefficientN = 0;
						int ticks = 7;
						int baseDamage = 0;
						int baseDamageN = 0;
						int summaryDamage = 0;
						int summaryDamageN = 0;
						switch (ESpell->GetLevel()) {
						case 1:
						{
							baseDamage = 4;
							baseDamageN = 5;
							coefficient = 32;
							coefficientN = 40;
							break;
						}
						case 2:
						{
							baseDamage = 8;
							baseDamageN = 10;
							coefficient = 34;
							coefficientN = 42.5;
							break;
						}
						case 3:
						{
							baseDamage = 12;
							baseDamageN = 15;
							coefficient = 36;
							coefficientN = 45;
							break;
						}
						case 4:
						{
							baseDamage = 16;
							baseDamageN = 20;
							coefficient = 38;
							coefficientN = 47.5;
							break;
						}
						case 5:
						{
							baseDamage = 20;
							baseDamageN = 25;
							coefficient = 40;
							coefficientN = 50;
							break;
						}
						}
						ticks += floor(Local->AttackSpeedMulti * 100 / 25);
						coefficient /= 100;
						coefficientN /= 100;
						const float dmgPerTick = GetDamageWithArmor((baseDamage + (Local->BaseAttackDamage + Local->BonusAttackDamage) * coefficient), obj->Armor);
						const float dmgPerTickN = GetDamageWithArmor((baseDamageN + (Local->BaseAttackDamage + Local->BonusAttackDamage) * coefficientN), obj->Armor);
						summaryDamage = dmgPerTick * ticks;
						summaryDamageN = dmgPerTickN * ticks;
						if (ESpell->GetSpellInfo()->GetSpellData()->GetSpellName() == "GarenECancel") {
							float timePassed = Last_E_Pressed - Function::GameTime();
							float damageGiven = dmgPerTick * floor(timePassed / duration * ticks);
							float damageGivenN = dmgPerTickN * floor(timePassed / duration * ticks);
							summaryDamage += damageGiven;
							summaryDamageN += damageGivenN;
						}
						hpBar.AddRightArea(summaryDamage, ImColor::ImColor(236, 240, 241));
						hpBar.AddRightArea(-(summaryDamageN - summaryDamage), ImColor::ImColor(127, 140, 141)); // dont know why, but nearest damage is not nearest damage
					}
				}

				if (ultLvl > 0 && Local->SpellBook.GetSpellSlotByID(3)->IsReady())
					hpBar.AddLeftArea(dhp, ImColor::ImColor(241, 196, 15));
				hpBar.Draw(pos, health + shield, maxHealth);

			}

		}
	}

	Render::EndOverlay();
}

void Garen::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}