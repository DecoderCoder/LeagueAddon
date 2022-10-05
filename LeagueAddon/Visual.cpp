#include "Visual.h"



std::map<int, float> distanceToHero;
std::map<GameObject*, bool> alwaysVisible;
std::map<GameObject*, float> recallState;
std::map<GameObject*, float> trackableObjects;

std::vector<GameObject*> spellTrackerHeroes;

bool lastSelectedHighlightChangeColorChanged = false;
float lastSelectedHighlightChange = 0;

void Visual::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);
	EventManager::AddEventHandler(EventManager::EventType::OnUnload, Unload);

	for (auto obj : ObjectManager::HeroList()) {
	//	if (obj->IsEnemyTo(Local)) {
		distanceToHero.insert({ obj->NetworkID, 0 });
		alwaysVisible.insert({ obj, false });
		recallState.insert({ obj, false });
		spellTrackerHeroes.push_back(obj);
		//}
	}
}

void Visual::Unload() {
	EventManager::RemoveEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::RemoveEventHandler(EventManager::EventType::OnMenu, OnDraw);
	distanceToHero.clear();
	alwaysVisible.clear();
	recallState.clear();
	spellTrackerHeroes.clear();
}


void Visual::OnThread() {

}

void Visual::OnMenu() {
	if (ImGui::CollapsingHeader("Visual")) {
		if (ImGui::TreeNode("ESP")) {
			ImGui::Checkbox("ESP", &ESP);
			if (ESP) {
				ImGui::SliderInt("ESP Distance", &ESP_Distance, 600, 15000);
			}
			ImGui::Checkbox("Show own attack radius", &ShowAttackRadius);
			ImGui::Checkbox("Show enemy bounding radius", &ShowBoundingRadius);
			ImGui::Checkbox("Highlight selected", &HighlightSelected);
			ImGui::Checkbox("Always visible", &AlwaysVisible);
			if (ImGui::TreeNode("Always visible heroes")) {

				for (auto obj : alwaysVisible) {
					if (ImGui::Checkbox(obj.first->GetChampionName().c_str(), &obj.second)) {
						alwaysVisible[obj.first] = obj.second;
					}
				}
				ImGui::Separator();

				ImGui::TreePop();
			}
			ImGui::Checkbox("Show last position", &ShowLastPos);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Spell Tracker")) {
			ImGui::Checkbox("Enabled Menu", &SpellTrackerMenu);
			ImGui::Checkbox("Enabled Overlay", &SpellTrackerMenu);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Recall Tracker")) {
			ImGui::Checkbox("Enabled Menu", &RecallTrackerMenu);
			ImGui::Checkbox("Enabled Overlay", &RecallTracker);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vision Tracker")) {

			ImGui::TreePop();
		}
	}

	if (SpellTrackerMenu) {
		int yOffset = 20;
		ImVec2 size = ImVec2(40, 40);
		Render::BeginOverlayTab("SpellTracker");

		for (int i = 0; i < spellTrackerHeroes.size(); i++) {
			auto hero = spellTrackerHeroes[i];

			ImVec2 textPos = ImVec2(ImGui::GetWindowPos().x + 10, ImGui::GetWindowPos().y + yOffset);

			Render::Draw_Text_Centered(textPos, ImVec2(ImGui::GetWindowContentRegionMax().x - 10, 16), hero->GetChampionName());
			// Not working because this is render...
			/*if (ImGui::BeginDragDropTarget()) {
				auto payload = ImGui::GetDragDropPayload();
				if (payload != nullptr) {
					GameObject* swap = spellTrackerHeroes[i];
					spellTrackerHeroes[i] = spellTrackerHeroes[*(int*)payload->Data];
					spellTrackerHeroes[*(int*)payload->Data] = swap;
				}

			}
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("SPELLTRACKERMENU", &i, sizeof(int));
				ImGui::Text(hero->GetChampionName().c_str());
				ImGui::EndDragDropSource();
			}*/
			yOffset += 18;
			for (int spellNum = 3; spellNum <= 5; spellNum++) {
				//MessageBoxA(0, hero->GetChampionName().c_str(), ("ChampionName - " + to_hex((int)hero)).c_str(), 0);
				CSpellSlot* spell = hero->SpellBook.GetSpellSlotByID(spellNum);
				if (!spell)
					continue;
				ImVec2 imagePos = ImVec2(ImGui::GetWindowPos().x + 10 + ((spellNum - 3) * size.x), ImGui::GetWindowPos().y + yOffset);
				string Image = spell->GetSpellInfo()->GetSpellData()->GetSpellName();

				if (spell->GetLevel() > 0) {
					if (spell->IsReady())
						if (Render::Images_Manager.HasImage(Image)) {
							Render::Draw_Image(imagePos, size, Render::Images_Manager.GetImageByName(Image.c_str()));
							Render::Draw_FilledRectangle(imagePos, size, ImColor::ImColor(0, 0, 0, 0), ImColor::ImColor(0, 0, 0, 255));
						}
						else {
							Render::Draw_FilledRectangle(imagePos, size, ImColor::ImColor(39, 174, 96), ImColor::ImColor(0, 0, 0));
							Render::Draw_Text_Centered(imagePos, size, QWERDF[spellNum]);
						}
					else
						if (Render::Images_Manager.HasImage(Image)) {
							Render::Draw_Image(imagePos, size, Render::Images_Manager.GetImageByName(Image.c_str()));
							Render::Draw_FilledRectangle(imagePos, size, ImColor::ImColor(0, 0, 0, 170));
							Render::Draw_Text_Centered(imagePos, size, to_string((int)spell->GetCD()).c_str());
							Render::Draw_FilledRectangle(imagePos, size, ImColor::ImColor(0, 0, 0, 0), ImColor::ImColor(0, 0, 0, 255));
						}
						else {
							Render::Draw_FilledRectangle(imagePos, size, ImColor::ImColor(192, 57, 43), ImColor::ImColor(0, 0, 0));
							Render::Draw_Text_Centered(imagePos, size, to_string((int)spell->GetCD()).c_str());
						}
					int charges = spell->GetCharges();
					if (charges > 0) {
						Render::Draw_Text(ImVec2(imagePos.x + 2, imagePos.y + 2), to_string(charges));
					}
				}
			}
			yOffset += size.y + 4;
		}

		Render::EndOverlayTab();
	}

	if (RecallTrackerMenu) {
		int yOffset = 10;
		Render::BeginOverlayTab("RecallTracker");
		for (auto obj : recallState) {
			int recallTime = 0;
			switch (obj.first->RecallState) {
			case kRecallState::Odin_Recall:
				recallTime = 8000;
				break;
			case kRecallState::Super_Recall:
				recallTime = 4000;
				break;
			case kRecallState::Teleporting:
				recallTime = 4000;
				break;
			default:
				recallTime = 0;
			}

			if (recallTime != 0 && obj.second == 0) {
				recallState[obj.first] = GetTickCount() + recallTime;
			}
			else {
				if (obj.first->RecallState == kRecallState::None)
					recallState[obj.first] = 0;
			}

			if (recallTime != 0) {
				float windowWidth = ImGui::GetWindowContentRegionWidth() - 10;
				float drawX = ImGui::GetWindowPos().x + 10;
				//float	drawY = w2sP.y + 20;
				float drawY = ImGui::GetWindowPos().y + 10 + yOffset;

				float width = (((GetTickCount() - obj.second - recallTime) * windowWidth) / (obj.second - obj.second - recallTime));

				Render::Draw_Text_Centered(drawX, drawY, windowWidth, 15, obj.first->GetChampionName());
				Render::Draw_FilledRectangle(drawX, drawY + 15, drawX + windowWidth, drawY + 10 + 15, ImColor(0, 0, 0));
				Render::Draw_FilledRectangle(drawX, drawY + 15, drawX + abs(windowWidth - width), drawY + 10 + 15, ImColor(41, 128, 185));
				yOffset += 30;
			}
		}
		Render::EndOverlayTab();
	}
}

bool IsAlwaysVisible(GameObject* obj) {
	for (auto av : alwaysVisible) {
		if (av.first == obj && av.second)
			return true;
	}
	return false;
}

void Visual::OnDraw() {
	Vector3 lw2s; // Local World 2 Screen
	Function::World2Screen(&Local->Position, &lw2s);

	Render::BeginOverlay();
	if (ESP) {
		if (RecallTracker)
			for (auto obj : recallState) {
				int recallTime = 0;
				switch (obj.first->RecallState) {
				case kRecallState::Odin_Recall:
					recallTime = 8000;
					break;
				case kRecallState::Super_Recall:
					recallTime = 4000;
					break;
				case kRecallState::Teleporting:
					recallTime = 4000;
					break;
				default:
					recallTime = 0;
				}

				if (recallTime != 0 && obj.second == 0) {
					recallState[obj.first] = GetTickCount() + recallTime;
				}
				else {
					if (obj.first->RecallState == kRecallState::None)
						recallState[obj.first] = 0;
				}

				if (recallTime != 0) {
					// TODO: 3D Optimization

					Vector3 w2sP;
					Function::World2Screen(&obj.first->Position, &w2sP);

					float drawX = w2sP.x - 140 / 2;
					//float	drawY = w2sP.y + 20;

					float drawY = w2sP.y + 23;

					if (drawX > Render::RenderWidth || drawY > Render::RenderHeight)
						continue;
					if (drawX + 140 < 0 || drawY + 10 < 0)
						continue;

					float width = (((GetTickCount() - obj.second - recallTime) * 140) / (obj.second - obj.second - recallTime));

					float hpWidth = obj.first->Health / obj.first->MaxHealth * 140;

					Render::Draw_FilledRectangle(drawX, drawY, drawX + 140, drawY + 10, ImColor(0, 0, 0));
					Render::Draw_FilledRectangle(drawX, drawY, drawX + abs(140 - width), drawY + 10, ImColor(41, 128, 185));

					Render::Draw_FilledRectangle(drawX, drawY + 10, drawX + 140, drawY + 20, ImColor(0, 0, 0));
					Render::Draw_FilledRectangle(drawX, drawY + 10, drawX + hpWidth, drawY + 20, (hpWidth > 70 ? ImColor(46, 204, 113) : ImColor(231, 76, 60)));
				}
			}

		for (auto obj : ObjectManager::HeroList()) {
			if (obj->IsEnemyTo(Local)) {
				if (Function::IsAlive(obj) && obj->IsVisible) {
					float distance = Local->Position.distanceTo(obj->Position);
					if (distance < ESP_Distance) {
						Vector3 ew2s; // Enemy W2S
						Function::World2Screen(&obj->Position, &ew2s);

						if (distanceToHero[obj->NetworkID] > distance)
							Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(231, 76, 60), 2);
						else
							Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(243, 156, 18), 2);


						Vector3 nameCoord = Local->Position.Extend(obj->Position, Local->AttackRange);

						Vector3 nameCoordw2s;
						Function::World2Screen(&nameCoord, &nameCoordw2s);

						Render::Draw_Text_Centered(ImVec2(nameCoordw2s.x, nameCoordw2s.y), ImVec2(50, 20), obj->GetChampionName(), 0xFFFFFFFF, 15);
					}
					distanceToHero[obj->NetworkID] = distance;
				}
			}
		}
	}

	if (ShowLastPos || AlwaysVisible)
		for (auto obj : ObjectManager::HeroList()) {
			bool selected = IsAlwaysVisible(obj);
			if (AlwaysVisible) {
				if (selected)
					obj->IsVisible = true;
			}
			if (ShowLastPos && obj->IsEnemyTo(Local) && Function::IsAlive(obj)) {
				ImColor drawColor;
				ImColor drawTextColor;
				Vector3 w2sPos;
				Vector2 mapPos = Function::WorldToMap(obj->Position);
				Function::World2Screen(&obj->Position, &w2sPos);


				drawColor = ImColor::ImColor(255, 255, 255);
				drawTextColor = ImColor::ImColor(0, 0, 0);
				if (obj->IsVisible) {
					Render::Draw_Circle3D(obj->Position, Function::GetBoundingRadius(obj), ImColor::ImColor(255, 255, 255), 1.f);
				}
				else {
					if (!selected) {
						Render::Draw_Circle3DFilled(obj->Position, Function::GetBoundingRadius(obj), ImColor::ImColor(255, 255, 0));
						Render::Draw_Text_Centered(ImVec2(w2sPos.x - Function::GetBoundingRadius(obj) / 2, w2sPos.y), ImVec2(Function::GetBoundingRadius(obj), 20), obj->GetChampionName(), ImColor::ImColor(0, 0, 0));
						Render::Draw_Circle(mapPos.x, mapPos.y, 4, drawColor, 7);
						Render::Draw_Text_Centered(mapPos.x - 1, mapPos.y - 1, 4, 4, string(1, obj->GetChampionName()[0]), drawTextColor);
					}
				}
			}
		}

	if (ShowAttackRadius) {
		Render::Draw_Circle3D(Local->Position, Local->AttackRange + Function::GetBoundingRadius(Local), ImColor(243, 156, 18), 3);
	}

	if (ShowBoundingRadius) {
		for (auto obj : ObjectManager::HeroList()) {
			if (obj->IsEnemyTo(Local) && obj->IsVisible && Function::IsAlive(obj) && obj->IsVisible)
				Render::Draw_Circle3D(obj->Position, Function::GetBoundingRadius(obj), ImColor::ImColor(255, 255, 255), 1);
		}
	}

	if (HighlightSelected) {
		int selected = Function::GetSelected();

		if (selected != 0) {
			if (GetTickCount() > lastSelectedHighlightChange) {
				// changeColor
				lastSelectedHighlightChangeColorChanged = !lastSelectedHighlightChangeColorChanged;
				lastSelectedHighlightChange = GetTickCount() + 500;
			}
			GameObject* selectedObj = ObjectManager::GetObjectByIndex(selected);
			if (selectedObj != nullptr)
				if (lastSelectedHighlightChangeColorChanged) {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(52, 152, 219), 2);
				}
				else {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(236, 240, 241), 2);
				}
		}
	}

	Render::EndOverlay();
}