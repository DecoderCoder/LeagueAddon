#include "Debug.h"

const char* qwerdf[] = { "Q", "W", "E", "R", "D", "F" };


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

		Render::Draw_Text(100, 100, "Addon time~: " + to_string(addonTime - GetTickCount()));

		for (int i = 0; i < detectedSpells.size(); i++)
		{
			//if(spell)

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
			Render::Draw_Text(w2sEndPos.x, w2sEndPos.y, "EndPos of " + detectedSpells[i].BasicAttackSpellData->Name + " [ " + to_hex((int)detectedSpellsPtr[i]) + " ]");
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
		}

		if (ImGui::CollapsingHeader("OnProcessSpell")) {
			int counter = 0;
			for (auto spell : detectedSpells)
			{
				ImGui::Text(to_string(counter).c_str());
				ImGui::SameLine();
				if (!spell.BasicAttackSpellData->Name.empty())
					ImGui::Text(spell.BasicAttackSpellData->Name.c_str());
				else
					ImGui::Text("Empty");
				counter++;
			}
		}

		if (ImGui::CollapsingHeader("Champions")) {
			for (auto hero : ObjectManager::HeroList()) {
				if (ImGui::TreeNode(hero->GetChampionName().c_str())) {
					bool isAlive = Function::IsAlive(hero);
					bool isValid = hero->isValid();
					bool IsInvulnearable = hero->IsInvulnearable();
					Vector3 pos = hero->Position;
					ImGui::Text(("Index: " + to_string(hero->Index)).c_str());
					ImGui::Text(("NetworkID: " + to_string(hero->NetworkID)).c_str());
					ImGui::Text(("PTR: " + to_hex((int)hero)).c_str());

					ImGui::SliderFloat("Health", &hero->Health, 0, hero->MaxHealth);

					ImGui::Text("Position: ");
					ImGui::SameLine();
					ImGui::Text(to_string(pos.x).c_str());
					ImGui::SameLine();
					ImGui::Text(to_string(pos.y).c_str());
					ImGui::SameLine();
					ImGui::Text(to_string(pos.z).c_str());
					ImGui::SameLine();
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
						for (int i = 0; i < 5; i++) {
							CSpellSlot* spell = spellBook->GetSpellSlotByID(i);
							if (ImGui::TreeNode((("[ " + string(qwerdf[i]) + " ] " + spell->GetName()).c_str()))) {
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

		ImGui::End();
	}
}

void Debug::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &Debug::OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &Debug::OnDraw);
	EventManager::AddEventHandler(EventManager::EventType::OnProcessSpell, &Debug::OnProcessSpell);
}