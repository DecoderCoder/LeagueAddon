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
		ImGui::Checkbox("Anti AFK", &AntiAFK);
		ImGui::Checkbox("Autosmite", &AutoSmite);
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
