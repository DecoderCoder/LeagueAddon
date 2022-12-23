#include "Visual.h"



std::map<int, float> distanceToHero;
std::map<GameObject*, bool> alwaysVisible;
std::map<GameObject*, float> recallState;
bool showTrackableModel = true;
std::vector<GameObject*> spellTrackerHeroes;

float lastPing = 0;
std::map<GameObject*, float> lastPingEnemy;
std::map<GameObject*, Vector3> lastEnemyPos;


void Visual::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);
	EventManager::AddEventHandler(EventManager::EventType::OnUnload, Unload);

	for (auto obj : ObjectManager::HeroList()) {
		if (obj->IsEnemyTo(Local)) {
			distanceToHero.insert({ obj->NetworkID, 0 });
			alwaysVisible.insert({ obj, false });
			recallState.insert({ obj, false });
			spellTrackerHeroes.push_back(obj);
			lastEnemyPos.insert({ obj, obj->Position });
			lastPingEnemy.insert({ obj, GetTickCount() });
		}
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
			ImGui::Checkbox("Show Navgitoon path", &ESP_ShowNav);
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
		if (ImGui::TreeNode("Ping enemy in fog of war")) {
			ImGui::Checkbox("Enabled", &PingInvisibleEnemy);

			if (ImGui::BeginCombo("Ping type enemy is coming", PingTypeName[(int)PingInvisibleEnemyTypeTo])) {
				if (ImGui::Selectable("Attack", PingInvisibleEnemyTypeTo == PingType::Attack)) {
					PingInvisibleEnemyTypeTo = PingType::Attack;
				}
				if (ImGui::Selectable("Danger", PingInvisibleEnemyTypeTo == PingType::Danger)) {
					PingInvisibleEnemyTypeTo = PingType::Danger;
				}
				if (ImGui::Selectable("Missing", PingInvisibleEnemyTypeTo == PingType::Missing)) {
					PingInvisibleEnemyTypeTo = PingType::Missing;
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Ping type is leaving", PingTypeName[(int)PingInvisibleEnemyTypeFrom])) {
				if (ImGui::Selectable("Attack", PingInvisibleEnemyTypeFrom == PingType::Attack)) {
					PingInvisibleEnemyTypeFrom = PingType::Attack;
				}
				if (ImGui::Selectable("Danger", PingInvisibleEnemyTypeFrom == PingType::Danger)) {
					PingInvisibleEnemyTypeFrom = PingType::Danger;
				}
				if (ImGui::Selectable("Missing", PingInvisibleEnemyTypeFrom == PingType::Missing)) {
					PingInvisibleEnemyTypeFrom = PingType::Missing;
				}
				ImGui::EndCombo();
			}
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
			ImGui::Checkbox("Wards and traps##VisionTracker", &VisionTracker);
			//ImGui::Checkbox("Clones##VisionTracker", &VisionTracker);
			if (ImGui::TreeNode("Settings")) {
				ImGui::Checkbox("Show model", &showTrackableModel);
				ImGui::TreePop();
			}
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

			//if (recallTime != 0 && obj.second == 0) {
			//	recallState[obj.first] = GetTickCount() + recallTime;
			//}
			//else {
			//	if (obj.first->RecallState == kRecallState::None) {

			//		recallState[obj.first] = 0;
			//	}


			//}

			if (recallTime != 0) {
				float windowWidth = ImGui::GetWindowContentRegionWidth() - 10;
				float drawX = ImGui::GetWindowPos().x + 10;
				//float	drawY = w2sP.y + 20;
				float drawY = ImGui::GetWindowPos().y + 10 + yOffset;

				float width = (((GetTickCount() - obj.second - recallTime) * windowWidth) / (obj.second - obj.second - recallTime));

				float hpWidth = obj.first->Health / obj.first->MaxHealth * windowWidth;


				Render::Draw_Text_Centered(drawX, drawY, windowWidth, 15, obj.first->GetChampionName());
				Render::Draw_FilledRectangle(drawX, drawY + 15, drawX + windowWidth, drawY + 10 + 15, ImColor(0, 0, 0));
				Render::Draw_FilledRectangle(drawX, drawY + 15, drawX + abs(windowWidth - width), drawY + 10 + 15, ImColor(41, 128, 185));

				Render::Draw_FilledRectangle(drawX, drawY + 25, drawX + windowWidth, drawY + 35, ImColor(0, 0, 0));
				Render::Draw_FilledRectangle(drawX, drawY + 25, drawX + hpWidth, drawY + 35, (hpWidth > windowWidth / 2 ? ImColor(46, 204, 113) : ImColor(231, 76, 60)));
				yOffset += 40;
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

bool HasTrackable(int netId) {
	for (auto& obj : Visual::trackableObjects) {
		if (obj.NetworkdId == netId)
			return true;
	}
	return false;
}

bool AddTrackable(GameObject* obj, WardType type) {
	if (HasTrackable(obj->NetworkID))
		return false;
	Ward ward;
	ward.NetworkdId = obj->NetworkID;
	ward.EndTime = Function::GameTime() + obj->Mana;
	ward.Type = type;
	ward.Position = obj->Position;
	ward.GameObject = obj;

	switch (ward.Type) {
	case WardType::ShacoBox:
		ward.BoundingRadius = 300;
		ward.Danger = true;
		break;
	case WardType::JhinTrap:
		ward.BoundingRadius = 140;
		ward.Danger = true;
		break;
	case WardType::JinxTrap:
		ward.BoundingRadius = 50;
		ward.Danger = true;
		break;
	case WardType::TeemoTrap:
		ward.BoundingRadius = 75;
		ward.Danger = true;
		break;
	case WardType::MaokaiTrap:
		ward.BoundingRadius = 50;
		ward.Danger = true;
		break;
	case WardType::NidaliTrap:
		ward.BoundingRadius = 50;
		ward.Danger = true;
		break;
	case WardType::CaytlinTrap:
		ward.BoundingRadius = 50;
		ward.Danger = true;
		break;

	case WardType::Yellow:
		ward.VisionRadius = 900;
		break;
	case WardType::Blue:
		ward.VisionRadius = 900;
		break;
	case WardType::JammerDevice:
		ward.VisionRadius = 900;
		break;
	}


	Visual::trackableObjects.push_back(ward);
	return true;
}

void Visual::OnDraw() {
	if (GetTickCount() > blinkTimer) {
		// changeColor
		blink = !blink;
		blinkTimer = GetTickCount() + 500;
	}

	Vector3 lw2s = Function::WorldToScreen(&Local->Position);; // Local World 2 Screen


	if (PingInvisibleEnemy) {
		for (auto obj : lastEnemyPos) {
			if ((Function::IsAlive(obj.first)) && obj.first->Position.distanceTo(lastEnemyPos[obj.first]) > 100) { // !obj.first->IsVisible   				
				if (obj.first->Position.distanceTo(Local->Position) < ESP_Distance * 2) {
					if (!obj.first->IsVisible && lastPingEnemy[obj.first] < GetTickCount() && lastPing < GetTickCount()) {
						//MessageBoxA(0, (obj.first->Position.ToString() + "\n\n\n" + lastEnemyPos[obj.first].ToString()).c_str(), "Position", 0);
						lastPing = GetTickCount() + 500;
						if (obj.first->IsFacing(Local)) {
							Function::SendPing(&obj.first->Position, obj.first->NetworkID, PingInvisibleEnemyTypeTo);
						}
						else {
							Function::SendPing(&obj.first->Position, obj.first->NetworkID, PingInvisibleEnemyTypeFrom);
						}

						//Function::PrintChat("Pinging: " + to_string(obj.first->NetworkID));
						lastPingEnemy[obj.first] = GetTickCount() + 5000;
					}
				}
			}
		}
	}

	Render::BeginOverlay();
	if (ESP) {
		if (ESP_ShowNav) {

			for (auto obj : ObjectManager::HeroList()) {
				if (obj->IsAllyTo(Local))
					continue;
				AIManager* mng = obj->GetAIManager();
				if (Function::IsAlive(obj) && mng->Moving) {
					//Render::Draw_Line3D(obj->Position, mng->NavArray[mng->currentSegment()], ImColor(255, 255, 255), 1);

					for (int i = 0; i < mng->pathSize() - 1; i++) {
						Render::Draw_Line(Function::WorldToScreen(&mng->NavArray[i]), Function::WorldToScreen(&mng->NavArray[i + 1]), ImColor(255, 255, 255), 1);
						Render::Draw_Line(Function::WorldToMap(mng->NavArray[i]), Function::WorldToMap(mng->NavArray[i + 1]), ImColor(255, 255, 255), 1);
					}

					Render::Draw_Text(Function::WorldToScreen(&mng->NavArray[mng->pathSize() - 1]), obj->GetChampionName(), ImColor(255, 255, 255));
				}

			}

		}

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
					if (obj.first->RecallState == kRecallState::None) {
						if (GetTickCount() > recallState[obj.first] - 500) {
							//obj.first->Health = obj.first->MaxHealth;
						}
						recallState[obj.first] = 0;
					}


				}

				if (recallTime != 0) {
					// TODO: 3D Optimization

					Vector3 w2sP = Function::WorldToScreen(&obj.first->Position);


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
						Vector3 ew2s = Function::WorldToScreen(&obj->Position); // Enemy W2S


						if (distanceToHero[obj->NetworkID] > distance)
							Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(231, 76, 60), 2);
						else
							Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(243, 156, 18), 2);


						Vector3 nameCoord = Local->Position.Extend(obj->Position, Local->AttackRange);

						Vector3 nameCoordw2s = Function::WorldToScreen(&nameCoord);


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
				Vector2 mapPos = Function::WorldToMap(obj->Position);
				Vector3 w2sPos = Function::WorldToScreen(&obj->Position);



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
						Render::Draw_Line3D(obj->Position, obj->Position.Extend(obj->GetObjectDirection(), obj->GetBoundingRadius()), ImColor(255, 0, 0), 10);
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
			GameObject* selectedObj = ObjectManager::GetObjectByIndex(selected);
			if (selectedObj != nullptr)
				if (blink) {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(52, 152, 219), 2);
				}
				else {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(236, 240, 241), 2);
				}
		}
	}

	if (VisionTracker) {
		auto it = trackableObjects.begin();
		while (it != trackableObjects.end()) {
			if (it->EndTime > Function::GameTime() || !Function::IsAlive(it->GameObject)) {
				it = trackableObjects.erase(it);// erase and go to next
			}
			else {
				++it;  // go to next
			}
		}

		for (auto obj : ObjectManager::MinionList()) {
			if (obj->IsEnemyTo(Local))
				if (obj->GetChampionName() == "YellowTrinket")
					AddTrackable(obj, WardType::Yellow);
				else if (obj->GetChampionName() == "SightWard")
					AddTrackable(obj, WardType::Yellow);
				else if (obj->GetChampionName() == "YellowTrinketUpgrade")
					AddTrackable(obj, WardType::Yellow);
				else if (obj->GetChampionName() == "Ward")
					AddTrackable(obj, WardType::Yellow);
				else if (obj->GetChampionName() == "BlueTrinket")
					AddTrackable(obj, WardType::Blue);
				else if (obj->GetChampionName() == "JammerDevice")
					AddTrackable(obj, WardType::JammerDevice);
				else if (obj->GetChampionName() == "ShacoBox")
					AddTrackable(obj, WardType::ShacoBox);
				else if (obj->GetChampionName() == "JinxMine")
					AddTrackable(obj, WardType::JinxTrap);
				else if (obj->GetChampionName() == "JhinTrap")
					AddTrackable(obj, WardType::JhinTrap);
				else if (obj->GetChampionName() == "NidaleeSpear")
					AddTrackable(obj, WardType::NidaliTrap);
				else if (obj->GetChampionName() == "MaokaiSproutling")
					AddTrackable(obj, WardType::MaokaiTrap);
				else if (obj->GetChampionName() == "TeemoMushroom")
					AddTrackable(obj, WardType::TeemoTrap);
				else if (obj->GetChampionName() == "CaytlinTrap")
					AddTrackable(obj, WardType::TeemoTrap);
		}

		for (auto ward : trackableObjects) {
			if (showTrackableModel)
				ward.GameObject->IsVisible = true;

			ImColor blinkColor = ward.Danger ? (blink ? ImColor(231, 76, 60) : ImColor(192, 57, 43)) : (blink ? ImColor(241, 196, 15) : ImColor(243, 156, 18));

			Render::Draw_Circle3D(ward.Position, ward.BoundingRadius, blinkColor, 2);

			if (ward.VisionRadius > 0)
				Render::Draw_Circle3D(ward.Position, ward.VisionRadius, ImColor(52, 152, 219, 1.0), 4);
		}


	}

	Render::EndOverlay();

	for (auto obj : lastEnemyPos) {
		lastEnemyPos[obj.first] = obj.first->Position;
	}
}