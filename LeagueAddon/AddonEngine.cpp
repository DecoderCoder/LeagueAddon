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
			if (ImGui::BeginCombo("Target mode 1", TargetSelector::targetingModeLabels[(int)TargetSelector::mode])) {
				for (int i = 0; i < 10; i++) {
					if (ImGui::Selectable(TargetSelector::targetingModeLabels[i], (int)TargetSelector::mode == i)) {
						TargetSelector::mode = (TargetingMode)i;
					}
				}
				ImGui::EndCombo();
			}
			if (TargetSelector::mode == TargetingMode::AutoPriority) {
				if (ImGui::BeginCombo("Target mode 2", TargetSelector::targetingModeLabels[(int)TargetSelector::mode2])) {
					for (int i = 1; i < 10; i++) {
						if (ImGui::Selectable(TargetSelector::targetingModeLabels[i], (int)TargetSelector::mode2 == i)) {
							TargetSelector::mode2 = (TargetingMode)i;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Separator();
				int secondTarget = 0;
				for (int i = 0; i < TargetSelector::attackOrder.size(); i++) {
					ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrder[i]->GetChampionName()).c_str());
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("TARGETORDER", &i, sizeof(int));
						ImGui::Text(TargetSelector::attackOrder[i]->GetChampionName().c_str());
						ImGui::EndDragDropSource();
						Render::BeginOverlay();
						for (auto hero : ObjectManager::HeroList()) {
							if (hero->NetworkID == TargetSelector::attackOrder[i]->NetworkID) {
								Render::Draw_Circle3DFilled(hero->Position, hero->GetBoundingRadius(), ImColor(255, 0, 0));
								break;
							}

						}
						Render::EndOverlay();
					}

					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
						if (payload != NULL) {
							if (*(int*)payload->Data >= 10) {
								int pos = *(int*)payload->Data - 10;								
								TargetSelector::attackOrder.insert(TargetSelector::attackOrder.begin() + i + 1, TargetSelector::attackOrderIgnore[pos]);
								TargetSelector::attackOrderIgnore.erase(TargetSelector::attackOrderIgnore.begin() + pos);
								//TargetSelector::attackOrder[*(int*)payload->Data] = swap;
							}
							else {
								GameObject* swap = TargetSelector::attackOrder[i];
								TargetSelector::attackOrder[i] = TargetSelector::attackOrder[*(int*)payload->Data];
								TargetSelector::attackOrder[*(int*)payload->Data] = swap;
							}
						}
						ImGui::EndDragDropTarget();
					}
				}
				ImGui::Separator();
				ImGui::Text("Drag the hero at this label to ignore priority");

				if (ImGui::BeginDragDropTarget())				{

					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
					if (payload != NULL && *(int*)payload->Data < 10 && TargetSelector::attackOrder.size() > 1) {
						TargetSelector::attackOrderIgnore.push_back(TargetSelector::attackOrder[*(int*)payload->Data]);
						TargetSelector::attackOrder.erase(TargetSelector::attackOrder.begin() + *(int*)payload->Data);
						//GameObject* swap = TargetSelector::attackOrder[i];
						//TargetSelector::attackOrder[i] = ;
						//TargetSelector::attackOrder[*(int*)payload->Data] = swap;
					}
				}
				for (int i = 0; i < TargetSelector::attackOrderIgnore.size(); i++) {
					ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrderIgnore[i]->GetChampionName()).c_str());
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						secondTarget = i + 10;
						ImGui::SetDragDropPayload("TARGETORDER", &secondTarget, sizeof(int));
						ImGui::Text(TargetSelector::attackOrderIgnore[secondTarget - 10]->GetChampionName().c_str());
						ImGui::EndDragDropSource();
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
	if (Local->GetChampionName() == "Draven")
		Draven::Initialize();
	if (Local->GetChampionName() == "Vayne")
		Vayne::Initialize();

	//// Main code

	Visual::Initialize();
	OrbWalker::Initialize();
	Evade::Core::Initalize();
	SkinChanger::Initialize();
	Misc::Initialize();

	Debug::Initialize();

	return true;
}

void AddonEngine::Uninitialize() {
	EventManager::Trigger(EventManager::EventType::OnUnload);
}