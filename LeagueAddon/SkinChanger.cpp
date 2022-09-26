#include "SkinChanger.h"

void SkinChanger::changeSkin2Player(GameObject* object, int skin) {

	auto addr = &object->CharacterDataStack;

	Function::CharacterDataStack_Push(addr, object->ChampionName, skin);
	Function::CharacterDataStack_Update(addr, true);
	//MessageBoxA(0, *(const char**)((uintptr_t)object->GetPtr() + (uintptr_t)0x2AC4), to_string(skin).c_str(), 0);
}

void SkinChanger::Initialize() {
	Resources::LoadSkinsDB(SkinsDB);

	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);

	for (auto hero : ObjectManager::HeroList()) {
		heroesSkins[hero] = *(int*)((int)hero->SkinPtr + 0x14);
	}
}


void SkinChanger::OnThread() {

}



void SkinChanger::OnMenu() {
	if (ImGui::CollapsingHeader("SkinChanger")) {
		//ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
		if (ImGui::Button("Disable all skins")) {
			for (auto obj : heroesSkins) {
				changeSkin2Player(obj.first, 0);
			}
		}

		int counter = 0;
		for (auto obj : heroesSkins) {
			Skin currentSkin;
			for (auto dbSkin : SkinsDB[obj.first->GetChampionName()]) {
				if (dbSkin.Id == obj.second) {
					currentSkin = dbSkin;
					break;
				}
			}
			if (ImGui::BeginCombo((obj.first->GetChampionName() + "##" + to_string(counter++)).c_str(), currentSkin.Name[currentLang].c_str())) {
				for (auto dbSkin : SkinsDB[obj.first->GetChampionName()]) {
					if (ImGui::Selectable(dbSkin.Name[currentLang].c_str(), dbSkin.Id == heroesSkins[obj.first])) {
						changeSkin2Player(obj.first, dbSkin.Id);
						heroesSkins[obj.first] = dbSkin.Id;
					}
				}
				ImGui::EndCombo();
			}
		}
	}
}

void SkinChanger::OnDraw() {

}