#include "Draven.h"

int lastWPressed = 0;

void Draven::OnDraw()
{
	Render::BeginOverlay();
	
	if (GetAsyncKeyStateN(VK_SPACE) && Local->SpellBook.GetSpellSlotByID(1)->IsReady() && lastWPressed < GetTickCount()) {
		Input::PressKey(W);
		lastWPressed = GetTickCount() + 70;
	}

	Render::EndOverlay();
}

void Draven::OnMenu()
{
	if (ImGui::CollapsingHeader("Draven")) {
		if (ImGui::TreeNode("Q")) {
			ImGui::Checkbox("OrbWalking Draven Mode", &OrbWalker::dravenMode);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("W")) {
			ImGui::Checkbox("Auto W", &autoW);		
			ImGui::TreePop();
		}
	}
}

void Draven::Initialize()
{
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}
