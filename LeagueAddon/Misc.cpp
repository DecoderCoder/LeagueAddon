#include "Misc.h"

void Misc::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);
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
			ImGui::Checkbox("Smite Dragon", &AutoSmite);
			ImGui::Checkbox("Smite Baron", &AutoSmite);
			ImGui::Checkbox("Smite buff if enemy jungler is near", &AutoSmite);
			ImGui::Checkbox("Smite crab if enemy jungler is near", &AutoSmite);

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

void Misc::OnDraw() {
	if (AutoSmite) {

	}

	if (AntiAFK && !IsLeagueInForeground() && lastAAFK < GetTickCount()) {
		Function::IssueOrder(Local, 2, &Local->Position, NULL, false, false, NULL);
		lastAAFK = GetTickCount() + 1000;
	}
}
