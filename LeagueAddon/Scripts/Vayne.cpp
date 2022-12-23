#include "Vayne.h"

float lastPressTime = 0;

void Vayne::OnDraw()
{
	Render::BeginOverlay();

	if (autoE) {
		auto eSpell = Local->SpellBook.GetSpellSlotByID(2);
		if (true) {
			GameObject* target = TargetSelector::GetTarget(550, DamageType::Physical, false);
			//auto target = Local;
			if (target) {
				Vector3 targetPos = target->Position;
				// knockback dist = 475
				// missilespeed 2200
				const float castTime = 0.413f; // in seconds
				const float flyTime = Local->Position.distanceTo(target->Position) / 2200;
				auto targetAI = target->GetAIManager();
				Vector3 velocity = targetAI->Velocity;

				Vector3 nextPos = targetPos + velocity * ((castTime + flyTime) * 10);

				Vector3 hitPos1 = nextPos.Extend(Local->Position, -475);
				Vector3 hitPos2 = targetPos.Extend(Local->Position, -475);

				bool hitWall1 = false, hitWall2 = false;
				int i1, i2;
				for (i1 = 0; i1 < 475; i1 += 5) {
					if (Function::IsWall(nextPos.Extend(hitPos1, i1))) {
						hitWall1 = true;
						break;
					}
				}

				for (i2 = 0; i2 < 475; i2 += 5) {
					if (Function::IsWall(targetPos.Extend(hitPos2, i2))) {
						hitWall2 = true;
						break;
					}
				}

				Render::Draw_Line3D(targetPos, nextPos, hitWall1 ? ImColor(231, 76, 60) : ImColor(52, 152, 219), 1);
				Render::Draw_Line3D(nextPos, nextPos.Extend(hitPos1, i1), hitWall1 ? ImColor(231, 76, 60) : ImColor(52, 152, 219), 1);

				if (hitWall1 && (!autoEHighChance || hitWall2) && eSpell->IsReady() && Local->Mana > 90 && GetAsyncKeyStateN(VK_SPACE)) {
					Vector3 w2s = Function::WorldToScreen(&target->Position);
					
					if (lastPressTime < GetTickCount())
					{
						Input::Move(w2s.x, w2s.y);
						Input::PressKey(E);
						lastPressTime = GetTickCount() + 70;
					}
				}
			}
		}
	}


	Render::EndOverlay();
}

void Vayne::OnMenu()
{
	if (ImGui::CollapsingHeader("Vayne")) {
		if (ImGui::TreeNode("AutoE##Vayne")) {
			ImGui::Checkbox("Enabled##VayneAutoE", &autoE);
			ImGui::Checkbox("High precision##VayneAutoE", &autoEHighChance);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("OrbWalker##Vayne")) {
			ImGui::Checkbox("Wait before attack in Q", &OrbWalker::vayneQMode);
			ImGui::SliderFloat("Delay", &OrbWalker::vayneQDelay, 0, 1, "%1.2f");
			ImGui::TreePop();
		}
	}
}

void Vayne::Initialize()
{
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}
