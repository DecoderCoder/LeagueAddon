#include "Vayne.h"

void Vayne::OnDraw()
{
	Render::BeginOverlay();
	
	/*if (GetAsyncKeyStateN(VK_SPACE) && Local->SpellBook.GetSpellSlotByID(1)->IsReady() && lastWPressed < GetTickCount()) {
		Input::PressKey(W);
		lastWPressed = GetTickCount() + 70;
	}*/

	Render::EndOverlay();
}

void Vayne::OnMenu()
{
	if (ImGui::CollapsingHeader("Vayne")) {
		if (ImGui::TreeNode("OrbWalker##Vayne")) {
			ImGui::Checkbox("Dont attack in Q", &OrbWalker::vayneQMode);
			ImGui::TreePop();
		}
	}
}

void Vayne::Initialize()
{
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}
