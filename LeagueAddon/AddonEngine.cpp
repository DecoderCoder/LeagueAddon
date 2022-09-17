#include "AddonEngine.h"

using namespace std;

void DrawMenu() {
	BlockInput(true);
	ImGui::Begin("LeagueAddon");

	if (Hooks::Hooked) {
		ImGui::Text(("Local ID: " + to_string(Local->NetworkID)).c_str());

		//if (ImGui::CollapsingHeader("Missiles")) {
		//	auto missiles = ObjectManager::MissileList();
		//	ImGui::Text(to_string(missiles.size()).c_str());
		//	for (auto obj : missiles) {
		//		if (!IsBadReadPtr(obj, 1)) {
		//			Vector3 w2s;
		//			Vector3* spellPos = (Vector3*)((int)obj + 0xD8);
		//			//Vector3* spellEndPos = (Vector3*)((int)obj + 0x2DC);
		//			Function::World2Screen(spellPos, &w2s);
		//			ImGui::Text(GetStr((DWORD)obj + 0x54));

		//			Render::BeginOverlay();

		//			Render::Rect(obj->CurrentPos, obj->EndPos, 0, 100, ImColor(255, 0, 0));
		//			Render::Draw_Text(w2s.x, w2s.y, GetStr((DWORD)obj + 0x54));

		//			

		//			Render::EndOverlay();
		//		}
		//		//else
		//		//	ImGui::Text(to_hex((int)obj).c_str());
		//	}
		//}	


		EventManager::Trigger(EventManager::EventType::OnMenu);

		if (ImGui::CollapsingHeader("Settings")) {
			if (ImGui::BeginCombo("Target mode", TargetSelector::targetingModeLabels[(int)TargetSelector::mode])) {
				for (int i = 0; i < 10; i++) {
					if (ImGui::Selectable(TargetSelector::targetingModeLabels[i], (int)TargetSelector::mode == i)) {
						TargetSelector::mode = (TargetingMode)i;
					}
				}
				ImGui::EndCombo();
			}
			if (TargetSelector::mode == TargetingMode::AutoPriority) {
				ImGui::Separator();
				for (int i = 0; i < TargetSelector::attackOrder.size(); i++) {
					ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrder[i]->GetChampionName()).c_str());
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("TARGETORDER", &i, sizeof(int));
						ImGui::Text(TargetSelector::attackOrder[i]->GetChampionName().c_str());
						ImGui::EndDragDropSource();
						/*Render::BeginOverlay();
						for (auto hero : ObjectManager::HeroList()) {
							if (hero->NetworkID == TargetSelector::attackOrder[i]->NetworkID) {
								Render::Draw_Circle3DFilled(hero->Position, hero->GetBoundingRadius(), ImColor(255, 0, 0));
								break;
							}

						}
						Render::EndOverlay();*/
					}
					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
						if (payload != NULL) {
							GameObject* swap = TargetSelector::attackOrder[i];
							TargetSelector::attackOrder[i] = TargetSelector::attackOrder[*(int*)payload->Data];
							TargetSelector::attackOrder[*(int*)payload->Data] = swap;
						}
						ImGui::EndDragDropTarget();
					}
				}
				ImGui::Separator();
			}

			if (ImGui::Checkbox("Debug mode", &Debug::Enabled)) {
				Debug::detectedSpells.clear();
				Debug::detectedSpellsPtr.clear();
			}
		}
	}
	else {
		ImGui::Text("Waiting for Hooks, use recall");
	}

	ImGui::End();
}

bool AddonEngine::Initialize() {
	Local = ObjectManager::GetLocalPlayer();
	TargetSelector::Initialize();

	// Init interface

	//

	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &DrawMenu);

	//// Scripts

	if (Local->GetChampionName() == "Twitch")
		Twitch::Initialize();
	if (Local->GetChampionName() == "Garen")
		Garen::Initialize();
	if (Local->GetChampionName() == "Akali")
		Akali::Initialize();
	if (Local->GetChampionName() == "Yasuo")
		Yasuo::Initialize();

	//// OrbWalker

	Visual::Initialize();
	OrbWalker::Initialize();
	Evade::Core::Initalize();
	SkinChanger::Initialize();

	Debug::Initialize();

	return true;
}