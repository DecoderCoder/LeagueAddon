#include "Tristana.h"

float lastPressTimeTristana = 0;

void Tristana::OnDraw()
{
	Render::BeginOverlay();

	if (autoE) {
		auto eSpell = Local->SpellBook.GetSpellSlotByID(2);
		if (true) {
			GameObject* target = TargetSelector::GetTarget(550, DamageType::Physical, false);
			//auto target = Local;
			if (target) {
				Vector3 targetPos = target->Position;
				

				if (eSpell->IsReady() && Local->Mana > 90 && GetAsyncKeyStateN(VK_SPACE)) {
					Vector3 w2s = Function::WorldToScreen(&target->Position);
					if (lastPressTimeTristana < GetTickCount())
					{
						Input::Move(w2s.x, w2s.y);
						Input::PressKey(E);
						lastPressTimeTristana = GetTickCount() + 70;
					}
				}
			}
		}
	}


	Render::EndOverlay();
}

void Tristana::OnMenu()
{
	if (ImGui::CollapsingHeader("Tristana")) {
		if (ImGui::TreeNode("AutoE##Tristana")) {
			ImGui::Checkbox("Enabled##TristanaTargetE", &autoE);
			
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("OrbWalker##Tristana")) {
			ImGui::Checkbox("E Priority", &OrbWalker::TristanaTargetE);
			ImGui::TreePop();
		}
	}
}

void Tristana::Initialize()
{
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}
