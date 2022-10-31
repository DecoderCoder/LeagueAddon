#include "Debug.h"

//const char* qwerdf[] = { "Q", "W", "E", "R", "D", "F" };


void Debug::OnProcessSpell(void* spellBook, SpellInfo* castInfo) {
	if (!Enabled)
		return;
	auto Caster = ObjectManager::FindObjectByIndex(ObjectManager::MinionList(), castInfo->source_id);
	if (Caster)
		return;
	//MessageBoxA(0, to_hex((int)spell->BasicAttackSpellData).c_str(), "Debug::OnProcessSpell", 0);
	detectedSpells.push_back(*castInfo);
	detectedSpellsPtr.push_back((uintptr_t)castInfo);
	//MessageBoxA(0, ("Detected: " + castInfo->BasicAttackSpellData->Name).c_str(), "Debug::OnProcessSpell", 0);
}

void Debug::OnDraw() {
	if (Enabled) {
		Render::BeginOverlay();

		Render::Draw_Text(100, 100, "GameTime  : " + to_string(Function::GameTime()));
		Render::Draw_Text(100, 115, "Ping      : " + to_string(Function::GetPing()));

		if (drawSpellDirection)
			for (int i = 0; i < detectedSpells.size(); i++)
			{
				Vector3 w2sStartPos;
				Vector3 w2sEndPos;
				Function::World2Screen(&detectedSpells[i].StartPosition, &w2sStartPos);
				Function::World2Screen(&detectedSpells[i].EndPosition, &w2sEndPos);
				if (detectedSpells[i].EndPosition2.x != 0 && detectedSpells[i].EndPosition2.y != 0) {
					Vector3 w2sEndPos2;
					Function::World2Screen(&detectedSpells[i].EndPosition2, &w2sEndPos2);
					Render::Draw_Line(w2sEndPos2.x, w2sEndPos2.y, w2sEndPos.x, w2sEndPos.y, ImColor(255, 0, 0), 1);
				}
				Render::Draw_Line(w2sStartPos.x, w2sStartPos.y, w2sEndPos.x, w2sEndPos.y, ImColor(0, 0, 255), 1);
				Render::Draw_Text(w2sStartPos.x, w2sStartPos.y, "StartPos of " + detectedSpells[i].BasicAttackSpellData->Name + " [ " + to_hex((int)detectedSpellsPtr[i]) + " ]");
				Render::Draw_Text(w2sEndPos.x, w2sEndPos.y, "EndPos");
			}

		if (drawObjectInfo)
			for (auto obj : ObjectManager::GetAllObjects()) {
				Vector3 w2s;
				Function::World2Screen(&obj->Position, &w2s);
				int offsetY = 0;
				const int offsetYStep = 15;

				Render::Draw_Text(w2s.x, w2s.y + offsetY, "Name: " + obj->GetChampionName());
				offsetY += offsetYStep;
				Render::Draw_Text(w2s.x, w2s.y + offsetY, "Ptr: " + to_hex((int)obj));
				offsetY += offsetYStep;
				Render::Draw_Text(w2s.x, w2s.y + offsetY, "IsAlive: " + string(Function::IsAlive(obj) ? "true" : "false"));
				offsetY += offsetYStep;
				Render::Draw_Text(w2s.x, w2s.y + offsetY, "IsVisible: " + string(obj->IsVisible ? "true" : "false"));
				offsetY += offsetYStep;

				if (drawObjectInfoBuffs) {
					for (auto buff : obj->BuffManager.entries()) {
						Render::Draw_Text(w2s.x + offsetYStep, w2s.y + offsetY, "BuffName    : " + buff->name());
						offsetY += offsetYStep;
						Render::Draw_Text(w2s.x + offsetYStep, w2s.y + offsetY, "BuffEndTime : " + to_string(buff->endTime));
						offsetY += offsetYStep;
					}
				}
			}
		Render::EndOverlay();
	}
}



void Debug::OnMenu() {
	if (Enabled) {
		ImGui::Begin("Debug");

		

		if (ImGui::CollapsingHeader("OrbWalker")) {
			ImGui::Text(("LastAA  : " + to_string(OrbWalker::LastAttackCommandT)).c_str());
			ImGui::Text(("LastMove: " + to_string(OrbWalker::LastMoveCommandT)).c_str());
			ImGui::Text(("Reseted : " + string(OrbWalker::Reseted ? "TRUE" : "FALSE")).c_str());
		}

		if (ImGui::CollapsingHeader("OnProcessSpell")) {
			int counter = 0;
			for (auto spell : detectedSpells)
			{
				if (!spell.BasicAttackSpellData->Name.empty()) {
					if (ImGui::TreeNode((to_string(counter) + " " + spell.BasicAttackSpellData->Name + "##DebugOnProcessSpell").c_str())) {
						ImGui::Text(("Distance 1: " + to_string(spell.StartPosition.distanceTo(spell.EndPosition))).c_str());
						ImGui::Text(("Distance 2: " + to_string(spell.StartPosition.distanceTo(spell.EndPosition2))).c_str());
						ImGui::Text(("Radius: " + to_string(spell.BasicAttackSpellData->Resource->Radius)).c_str());
						ImGui::Text(("Speed: " + to_string(spell.BasicAttackSpellData->Resource->MissileSpeed)).c_str());
						//ImGui::Text(("Target: " + (spell.HasTarget ? to_string(spell.targetIndex()) : string("Empty"))).c_str());
						//ImGui::Text(("Speed: " + to_string(spell.BasicAttackSpellData->Resource->MissileSpeed)).c_str());
						ImGui::TreePop();
					}
				}
				else
					ImGui::Text("Empty");

				counter++;
			}
		}

		if (ImGui::CollapsingHeader("Missiles")) {
			auto missiles = ObjectManager::MissileList();
			ImGui::Text(to_string(missiles.size()).c_str());
			for (auto obj : missiles) {
				Vector3 w2s;
				Vector3* spellPos = (Vector3*)((int)obj + 0xD8);
				//Vector3* spellEndPos = (Vector3*)((int)obj + 0x2DC);
				Function::World2Screen(spellPos, &w2s);
				ImGui::Text(GetStr((DWORD)obj + 0x54));
			}
		}

		if (ImGui::CollapsingHeader("Champions")) {
			for (auto hero : ObjectManager::HeroList()) {
				if (ImGui::TreeNode((hero->GetChampionName() + "##" + to_string(hero->Index)).c_str())) {
					AIManager* aiManager = hero->GetAIManager();
					bool isAlive = Function::IsAlive(hero);
					bool isValid = hero->isValid();
					bool IsInvulnearable = hero->IsInvulnearable(Local);
					Vector3 pos = hero->Position;
					ImGui::Text(("Index: " + to_string(hero->Index)).c_str());
					ImGui::Text(("NetworkID: " + to_string(hero->NetworkID)).c_str());
					ImGui::Text(("PTR: " + to_hex((int)hero)).c_str());
					ImGui::Text(("AiManager PTR: " + to_hex((int)aiManager)).c_str());

					ImGui::SliderFloat("Health", &hero->Health, 0, hero->MaxHealth);
					ImGui::Text(("RecallState: " + to_string((int)hero->RecallState)).c_str());
					ImGui::Text("Position: ");
					ImGui::SameLine();
					ImGui::Text(to_string(pos.x).c_str());
					ImGui::SameLine();
					ImGui::Text(to_string(pos.y).c_str());
					ImGui::SameLine();
					ImGui::Text(to_string(pos.z).c_str());


					ImGui::DragFloat("X", &hero->Position.x);
					ImGui::DragFloat("Y", &hero->Position.y);
					ImGui::DragFloat("Z", &hero->Position.z);

					ImGui::Separator();


					ImGui::Text("AD Damage: ");
					ImGui::SameLine();
					ImGui::Text(to_string(hero->BaseAttackDamage).c_str());
					ImGui::SameLine();
					ImGui::Text(to_string(hero->BonusAttackDamage).c_str());
					ImGui::Text("AP Damage: ");
					ImGui::SameLine();
					ImGui::Text(to_string(hero->BaseAbilityDamage).c_str());

					ImGui::Checkbox("IsValid", &isValid);
					ImGui::Checkbox("IsTargetable", &hero->IsTargetable);
					ImGui::Checkbox("IsInvulnearable", &IsInvulnearable);
					ImGui::Checkbox("IsVisible", &hero->IsVisible);

					ImGui::Checkbox("IsAlive", &isAlive);

					if (ImGui::CollapsingHeader("Spells")) {
						CSpellBook* spellBook = &hero->SpellBook;
						for (int i = 0; i < 6; i++) {
							CSpellSlot* spell = spellBook->GetSpellSlotByID(i);
							if (!spell)
								continue;
							if (ImGui::TreeNode((("[ " + QWERDF[i] + " ] " + spell->GetName()).c_str()))) {
								ImGui::Text(("Spell CD		: " + to_string(spell->GetCD())).c_str());
								ImGui::Text(("Spell Charges	: " + to_string(spell->GetCharges())).c_str());
								//ImGui::Text(("Spell Charges	: " + to_string(spell->GetSpellInfo())).c_str());
								ImGui::TreePop();
							}
						}
					}

					if (ImGui::CollapsingHeader("BuffManager")) {
						BuffManager* buffManager = &hero->BuffManager;
						for (auto buff : buffManager->entries()) {
							ImGui::Text(("Name : " + buff->name()).c_str());
							ImGui::Text(("Count: " + to_string(buff->getCount())).c_str());
						}
					}

					if (ImGui::CollapsingHeader("Inventory")) {
						HeroInventory* inventory = &hero->Inventory;
						for (int i = 0; i < 5; i++) {
							Item* item = inventory->GetItemByID(i);
							if (item != 0) {
								ImGui::Text(("Id   : " + to_string(item->Info->ID)).c_str());
								ImGui::Text(("Name : " + string(item->Name)).c_str());
							}
						}
					}

					ImGui::TreePop();
				}
			}
		}
		if (ImGui::CollapsingHeader("SkinChanger")) {
			if (ImGui::TreeNode("Change skin")) {
				for (auto obj : SkinChanger::heroesSkins) {
					if (ImGui::InputInt(obj.first->GetChampionName().c_str(), &obj.second)) {
						SkinChanger::changeSkin2Player(obj.first, obj.second);
						SkinChanger::heroesSkins[obj.first] = obj.second;
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("SkinDB")) {
				for (auto obj : SkinChanger::SkinsDB) {
					if (ImGui::TreeNode(obj.first.c_str())) {
						for (auto skin : obj.second) {
							for (auto skinName : skin.Name) {
								ImGui::Text(("[ " + skinName.first + " ] " + skinName.second).c_str());
							}
							if (ImGui::Button(("Set this skin##" + obj.first + to_string(skin.Id)).c_str())) {
								for (auto hero : ObjectManager::HeroList()) {
									if (hero->GetChampionName() == obj.first) {
										SkinChanger::changeSkin2Player(hero, skin.Id);
										break;
									}
								}
							}

						}
						ImGui::TreePop();
					}
				}
			}
		}

		if (ImGui::CollapsingHeader("Trackable Objects")) {
			for (auto obj : Visual::trackableObjects) {
				ImGui::Text(("-- " + string(WardTypeName[(int)obj.Type])).c_str());
				ImGui::Text(("NetId : " + to_string(obj.NetworkdId)).c_str());
				ImGui::Text(("Bounding radius : " + to_string(obj.BoundingRadius)).c_str());
				ImGui::Text(("Vision radius   : " + to_string(obj.VisionRadius)).c_str());
				ImGui::Separator();
			}
		}

		if (ImGui::CollapsingHeader("Render")) {
			{
				//Vector3 bdp; // BaseDrawPosition
				/*Function::GetHPBarPosition(Local, &bdp);
				ImGui::Text("GetHPBarPosition: ");
				ImGui::SameLine();
				ImGui::Text(to_string(bdp.x).c_str());
				ImGui::SameLine();
				ImGui::Text(to_string(bdp.y).c_str());
				ImGui::SameLine();
				ImGui::Text(to_string(bdp.z).c_str());*/

				if (ImGui::TreeNode("Images Manager")) {
					for (auto obj : *Render::Images_Manager.GetVector()) {
						if (ImGui::TreeNode(("[" + obj.Name + "]").c_str())) {
							ImGui::Image(obj.Image, ImVec2(100, 100));
							ImGui::Separator();
							ImGui::TreePop();
						}
					}
					ImGui::Separator();
					ImGui::TreePop();
				}
			}

			ImGui::Text(("SM_CXSCREEN: " + to_string(GetSystemMetrics(SM_CXSCREEN))).c_str());
			ImGui::Text(("SM_CYSCREEN: " + to_string(GetSystemMetrics(SM_CYSCREEN))).c_str());

			ImGui::Text(("RenderWidth : " + to_string(Render::RenderWidth)).c_str());
			ImGui::Text(("RenderHeight: " + to_string(Render::RenderHeight)).c_str());


		}

		if (ImGui::CollapsingHeader("Hero scripts")) {
			if (ImGui::Button("Twitch"))
				Twitch::Initialize();
			if (ImGui::Button("Garen"))
				Garen::Initialize();
			if (ImGui::Button("Akali"))
				Akali::Initialize();
			if (ImGui::Button("Yasuo"))
				Yasuo::Initialize();
			if (ImGui::Button("Draven"))
				Draven::Initialize();
			if (ImGui::Button("Vayne"))
				Vayne::Initialize();
		}

		Vector3 mousePos = *Function::GetMouseWorldPosition();
		ImGui::Text("MousePosition: ");
		ImGui::SameLine();
		ImGui::Text(to_string(mousePos.x).c_str());
		ImGui::SameLine();
		ImGui::Text(to_string(mousePos.y).c_str());
		ImGui::SameLine();
		ImGui::Text(to_string(mousePos.z).c_str());

		ImGui::Text(("IsWall: " + string((Function::IsWall(Function::GetMouseWorldPosition()) ? "true" : "false"))).c_str());

		ImGui::Text(("GameTime    : " + to_string(Function::GameTime())).c_str());
		ImGui::Text(("GameTick    : " + to_string(Function::GameTimeTick())).c_str());
		ImGui::Text(("GetTickCount: " + to_string(GetTickCount())).c_str());

		ImGui::End();
	}
}

void Debug::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &Debug::OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &Debug::OnDraw);
	EventManager::AddEventHandler(EventManager::EventType::OnProcessSpell, &Debug::OnProcessSpell);
}