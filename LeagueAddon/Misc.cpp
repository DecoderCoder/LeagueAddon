#include "Misc.h"

int smiteSlotNum = 0;
CSpellSlot* smiteSlot = nullptr;

GameObject* enemyJungler = nullptr;

void Misc::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);

	for (int i = 0; i < 6; i++) {
		if (StringContains(Local->SpellBook.GetSpellSlotByID(i)->GetName(), "SummonerSmite", false)) {
			smiteSlotNum = i;
			smiteSlot = Local->SpellBook.GetSpellSlotByID(i);
			break;
		}
	}

	for (auto hero : ObjectManager::HeroList()) {
		if (hero->IsEnemyTo(Local)) {
			for (int i = 0; i < 6; i++) {
				auto spellSlot = hero->SpellBook.GetSpellSlotByID(i);
				if (spellSlot && StringContains(spellSlot->GetName(), "SummonerSmite", false)) {
					enemyJungler = hero;
					break;
				}
			}
		}
		if (enemyJungler)
			break;
	}
	//EventManager::AddEventHandler(EventManager::EventType::OnThread, OnDraw);
}

void Misc::OnThread() {

}

void Misc::OnMenu() {
	if (ImGui::CollapsingHeader("Misc")) {
		if (ImGui::TreeNode("Anti AFK")) {
			ImGui::Checkbox("Enabled", &AntiAFK);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Auto smite")) {
			ImGui::Checkbox("Enabled", &AutoSmite);
			ImGui::Checkbox("Smite Dragon", &AutoSmite_Dragon);
			ImGui::Checkbox("Smite Baron", &AutoSmite_Baron);
			ImGui::Checkbox("Smite buff if enemy jungler is near", &AutoSmite_Buff);
			ImGui::Checkbox("Smite crab if enemy jungler is near", &AutoSmite_Crab);
			ImGui::Checkbox("Smite buff/craba when have 2 charges", &AutoSmite_HasSecondSmite);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Fun")) {
			if (ImGui::Button("Toggle \"Ping 0\"+")) {
				BYTE* ping = (BYTE*)(*(int*)DEFINE_RVA(Offset::Data::PingObject) + (DWORD)0x1A7);
				*ping += 0x1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Toggle \"Ping 0\"-")) {
				bool* ping = (bool*)(*(int*)DEFINE_RVA(Offset::Data::PingObject) + (DWORD)0x1A7);
				*ping -= 0x1;
			}
			int i = 0;
			for (auto obj : ObjectManager::HeroList()) {
				if (i++ == (int)(*(BYTE*)(*(int*)DEFINE_RVA(Offset::Data::PingObject) + (DWORD)0x1A7))) {
					ImGui::Text(obj->GetChampionName().c_str());
					break;
				}
			}

			ImGui::TreePop();
		}
		//ImGui::Checkbox("Autosmite", &AutoSmite);
	}
}

int lastAAFK = 0;
int lastSmite = 0;

void castSmite(GameObject* to) {

	if (lastSmite < GetTickCount()) {
		Vector3 w2s = Function::WorldToScreen(&to->Position);
		if (w2s.x > 0 && w2s.y > 0 && w2s.x < GetSystemMetrics(SM_CXSCREEN) && w2s.y < GetSystemMetrics(SM_CYSCREEN)) {
			Input::Move(w2s.x, w2s.y);
			switch (smiteSlotNum) {
			case 4:
				Input::PressKey(D);
				break;
			case 5:
				Input::PressKey(F);
				break;
			}
		}
	}

	lastSmite = GetTickCount() + 70;
}

bool IsValid2Smite(GameObject* target) {
	return target->IsVisible && !target->IsInvulnearable() && target->IsTargetable && Function::IsAlive(target);
}

void Misc::OnDraw() {
	if (AutoSmite && smiteSlot) {
		float smiteDamage = smiteSlot->GetDamage();
		if (smiteSlot->IsReady()) {
			auto minionsInRange = filter(ObjectManager::MinionList(), [&](GameObject* minion) { return minion->Health <= smiteDamage && minion->IsInRange(Local, 600, false) && IsValid2Smite(minion); });
			for (auto obj : minionsInRange) {
				if (AutoSmite_Dragon) {
					if (StringContains(obj->GetChampionName(), "SRU_Dragon")) {
						castSmite(obj);
						break;
					}
				}

				if (AutoSmite_Baron) {
					if (StringContains(obj->GetChampionName(), "SRU_Baron") || StringContains(obj->GetChampionName(), "SRU_RiftHerald")) {
						castSmite(obj);
						break;
					}
				}

				if (enemyJungler && (smiteSlot->GetCharges() == 2 || !AutoSmite_HasSecondSmite)) {
					if (AutoSmite_Buff)
						if (StringContains(obj->GetChampionName(), "SRU_Red") || StringContains(obj->GetChampionName(), "SRU_Blue")) {
							if (enemyJungler->IsVisible && Function::IsAlive(enemyJungler) && enemyJungler->Position.distanceTo(obj->Position) < 700) {
								castSmite(obj);
								break;
							}
						}

					if (AutoSmite_Crab)
						if (StringContains(obj->GetChampionName(), "Sru_Crab"), false) {
							if (enemyJungler->IsVisible && Function::IsAlive(enemyJungler) && enemyJungler->Position.distanceTo(obj->Position) < 700) {
								castSmite(obj);
								break;
							}
						}
				}
			}
		}
	}

	if (Settings::Global::useIssueOrder && AntiAFK && !IsLeagueInForeground() && lastAAFK < GetTickCount()) {
		Function::IssueOrder(Local, 2, &Local->Position, NULL, false, false, NULL);
		lastAAFK = GetTickCount() + 1000;
	}
}