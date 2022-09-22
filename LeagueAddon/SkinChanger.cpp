#include "SkinChanger.h"

void SkinChanger::changeSkin2Player(GameObject* object, int skin) {

	auto addr = &object->CharacterDataStack;

	Function::CharacterDataStack_Push(addr, object->ChampionName, skin);
	Function::CharacterDataStack_Update(addr, true);
	//MessageBoxA(0, *(const char**)((uintptr_t)object->GetPtr() + (uintptr_t)0x2AC4), to_string(skin).c_str(), 0);
}

void SkinChanger::Initialize() {
	LoadSkinsDB();

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

void SkinChanger::LoadSkinsDB() {
	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Pharaoh Amumu";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Vancouver Amumu";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Emumu";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Re-Gifted Amumu";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Almost-Prom King Amumu";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Little Knight Amumu";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Sad Robot Amumu";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Surprise Party Amumu";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "Infernal Amumu";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 23;
		skin10.Name["en_US"] = "Hextech Amumu";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 24;
		skin11.Name["en_US"] = "Pumpkin Prince Amumu";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 34;
		skin12.Name["en_US"] = "Porcelain Amumu";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Amumu"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Black Alistar";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Golden Alistar";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Matador Alistar";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Longhorn Alistar";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Unchained Alistar";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Infernal Alistar";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Sweeper Alistar";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Marauder Alistar";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "SKT T1 Alistar";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Moo Cow Alistar";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 19;
		skin11.Name["en_US"] = "Hextech Alistar";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 20;
		skin12.Name["en_US"] = "Conqueror Alistar";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 22;
		skin13.Name["en_US"] = "Blackfrost Alistar";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 29;
		skin14.Name["en_US"] = "Lunar Beast Alistar";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		SkinsDB["Alistar"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Cyber Pop Akshan";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Crystal Rose Akshan";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Akshan"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Goth Annie";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Red Riding Annie";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Annie in Wonderland";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Prom Queen Annie";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Frostfire Annie";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Reverse Annie";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "FrankenTibbers Annie";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Panda Annie";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Sweetheart Annie";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Hextech Annie";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 11;
		skin11.Name["en_US"] = "Super Galaxy Annie";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 12;
		skin12.Name["en_US"] = "Annie-Versary";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 13;
		skin13.Name["en_US"] = "Lunar Beast Annie";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 22;
		skin14.Name["en_US"] = "Cafe Cuties Annie";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		SkinsDB["Annie"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Justicar Aatrox";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Mecha Aatrox";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Sea Hunter Aatrox";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 7;
		skin4.Name["en_US"] = "Blood Moon Aatrox";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "Prestige Blood Moon Aatrox";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Victorious Aatrox";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 11;
		skin7.Name["en_US"] = "Odyssey Aatrox";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 20;
		skin8.Name["en_US"] = "Prestige Blood Moon Aatrox (2022)";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		SkinsDB["Aatrox"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dynasty Ahri";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Midnight Ahri";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Foxfire Ahri";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Popstar Ahri";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Challenger Ahri";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Academy Ahri";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Arcade Ahri";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Star Guardian Ahri";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "K/DA Ahri";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 16;
		skin10.Name["en_US"] = "Prestige K/DA Ahri";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 17;
		skin11.Name["en_US"] = "Elderwood Ahri";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 27;
		skin12.Name["en_US"] = "Spirit Blossom Ahri";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 28;
		skin13.Name["en_US"] = "K/DA ALL OUT Ahri";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 42;
		skin14.Name["en_US"] = "Coven Ahri";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 65;
		skin15.Name["en_US"] = "Prestige K/DA Ahri (2022)";
		skin15.Chromas = false;
		heroSkins.push_back(skin15);

		Skin skin16;
		skin16.Id = 66;
		skin16.Name["en_US"] = "Arcana Ahri";
		skin16.Chromas = true;
		heroSkins.push_back(skin16);

		SkinsDB["Ahri"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Stinger Akali";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Infernal Akali";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "All-star Akali";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Nurse Akali";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blood Moon Akali";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Silverfang Akali";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Headhunter Akali";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Sashimi Akali";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "K/DA Akali";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 13;
		skin10.Name["en_US"] = "Prestige K/DA Akali";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 14;
		skin11.Name["en_US"] = "PROJECT: Akali";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 15;
		skin12.Name["en_US"] = "True Damage Akali";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 32;
		skin13.Name["en_US"] = "K/DA ALL OUT Akali";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 50;
		skin14.Name["en_US"] = "Crime City Nightmare Akali";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 60;
		skin15.Name["en_US"] = "Prestige K/DA Akali (2022)";
		skin15.Chromas = false;
		heroSkins.push_back(skin15);

		SkinsDB["Akali"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Team Spirit Anivia";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Bird of Prey Anivia";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Noxus Hunter Anivia";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Hextech Anivia";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blackfrost Anivia";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Prehistoric Anivia";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Festival Queen Anivia";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Papercraft Anivia";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "Cosmic Flight Anivia";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 27;
		skin10.Name["en_US"] = "Divine Phoenix Anivia";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Anivia"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Galactic Azir";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Gravelord Azir";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SKT T1 Azir";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Warring Kingdoms Azir";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Elderwood Azir";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Azir"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Freljord Ashe";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sherwood Forest Ashe";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Woad Ashe";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Queen Ashe";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Amethyst Ashe";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Heartseeker Ashe";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Marauder Ashe";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "PROJECT: Ashe";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Championship Ashe";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 11;
		skin10.Name["en_US"] = "Cosmic Queen Ashe";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 17;
		skin11.Name["en_US"] = "High Noon Ashe";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 23;
		skin12.Name["en_US"] = "Fae Dragon Ashe";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 32;
		skin13.Name["en_US"] = "Coven Ashe";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Ashe"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Ashen Lord Aurelion Sol";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Mecha Aurelion Sol";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 11;
		skin3.Name["en_US"] = "Storm Dragon Aurelion Sol";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		SkinsDB["AurelionSol"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Elderwood Bard";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 5;
		skin2.Name["en_US"] = "Snow Day Bard";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 6;
		skin3.Name["en_US"] = "Bard Bard";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 8;
		skin4.Name["en_US"] = "Astronaut Bard";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 17;
		skin5.Name["en_US"] = "Cafe Cuties Bard";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Bard"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Rusty Blitzcrank";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Goalkeeper Blitzcrank";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Boom Boom Blitzcrank";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Piltover Customs Blitzcrank";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Definitely Not Blitzcrank";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "iBlitzcrank";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Riot Blitzcrank";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Battle Boss Blitzcrank";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 20;
		skin9.Name["en_US"] = "Lancer Rogue Blitzcrank";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 21;
		skin10.Name["en_US"] = "Lancer Paragon Blitzcrank";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 22;
		skin11.Name["en_US"] = "Witch's Brew Blitzcrank";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 29;
		skin12.Name["en_US"] = "Space Groove Blitz & Crank";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 36;
		skin13.Name["en_US"] = "Victorious Blitzcrank";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Blitzcrank"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dragonslayer Braum";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "El Tigre Braum";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Braum Lionheart";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 10;
		skin4.Name["en_US"] = "Santa Braum";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 11;
		skin5.Name["en_US"] = "Crime City Braum";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 24;
		skin6.Name["en_US"] = "Sugar Rush Braum";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 33;
		skin7.Name["en_US"] = "Pool Party Braum";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Braum"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Nightbringer Aphelios";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 9;
		skin2.Name["en_US"] = "Lunar Beast Aphelios";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Aphelios"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Resistance Caitlyn";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sheriff Caitlyn";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Safari Caitlyn";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Arctic Warfare Caitlyn";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Officer Caitlyn";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Headhunter Caitlyn";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Lunar Wraith Caitlyn";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Pulsefire Caitlyn";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 13;
		skin9.Name["en_US"] = "Pool Party Caitlyn";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 19;
		skin10.Name["en_US"] = "Arcade Caitlyn";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 20;
		skin11.Name["en_US"] = "Prestige Arcade Caitlyn";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 22;
		skin12.Name["en_US"] = "Battle Academia Caitlyn";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 28;
		skin13.Name["en_US"] = "Arcane Caitlyn";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 29;
		skin14.Name["en_US"] = "Prestige Arcade Caitlyn (2022)";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		SkinsDB["Caitlyn"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Apocalyptic Brand";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Vandal Brand";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Cryocore Brand";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Zombie Brand";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Spirit Fire Brand";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Battle Boss Brand";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Arclight Brand";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Eternal Dragon Brand";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 21;
		skin9.Name["en_US"] = "Debonair Brand";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 22;
		skin10.Name["en_US"] = "Prestige Debonair Brand";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		SkinsDB["Brand"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Program Camille";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Coven Camille";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 10;
		skin3.Name["en_US"] = "iG Camille";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 11;
		skin4.Name["en_US"] = "Arcana Camille";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Camille"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Desperada Cassiopeia";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Siren Cassiopeia";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Mythic Cassiopeia";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Jade Fang Cassiopeia";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "Eternum Cassiopeia";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Spirit Blossom Cassiopeia";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 18;
		skin7.Name["en_US"] = "Coven Cassiopeia";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Cassiopeia"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Nightmare Cho'Gath";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Gentleman Cho'Gath";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Loch Ness Cho'Gath";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Jurassic Cho'Gath";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Battlecast Prime Cho'Gath";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Prehistoric Cho'Gath";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Dark Star Cho'Gath";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Shan Hai Scrolls Cho'Gath";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Chogath"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "UFO Corki";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Ice Toboggan Corki";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Red Baron Corki";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Hot Rod Corki";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Urfrider Corki";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dragonwing Corki";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Fnatic Corki";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Arcade Corki";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 18;
		skin9.Name["en_US"] = "Corgi Corki";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 26;
		skin10.Name["en_US"] = "Astronaut Corki";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Corki"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Lord Darius";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Bioforge Darius";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Woad King Darius";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Dunkmaster Darius";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "Academy Darius";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 14;
		skin6.Name["en_US"] = "Dreadnova Darius";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "God-King Darius";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "High Noon Darius";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 24;
		skin9.Name["en_US"] = "Lunar Beast Darius";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 33;
		skin10.Name["en_US"] = "Crime City Nightmare Darius";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		SkinsDB["Darius"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dark Valkyrie Diana";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Lunar Goddess Diana";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Infernal Diana";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 11;
		skin4.Name["en_US"] = "Blood Moon Diana";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 12;
		skin5.Name["en_US"] = "Dark Waters Diana";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 18;
		skin6.Name["en_US"] = "Dragonslayer Diana";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 25;
		skin7.Name["en_US"] = "Battle Queen Diana";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 26;
		skin8.Name["en_US"] = "Prestige Battle Queen Diana";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 27;
		skin9.Name["en_US"] = "Sentinel Diana";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 37;
		skin10.Name["en_US"] = "Firecracker Diana";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Diana"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Toxic Dr. Mundo";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Mr. Mundoverse";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Corporate Mundo";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Mundo Mundo";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Executioner Mundo";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Rageborn Mundo";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "TPA Mundo";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Pool Party Mundo";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "El Macho Mundo";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Frozen Prince Mundo";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["DrMundo"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Soul Reaver Draven";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Gladiator Draven";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Primetime Draven";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pool Party Draven";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Beast Hunter Draven";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Draven Draven";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 12;
		skin7.Name["en_US"] = "Santa Draven";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 13;
		skin8.Name["en_US"] = "Mecha Kingdoms Draven";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 20;
		skin9.Name["en_US"] = "Ruined Draven";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 29;
		skin10.Name["en_US"] = "Debonair Draven";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Draven"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sandstorm Ekko";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Academy Ekko";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "PROJECT: Ekko";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 11;
		skin4.Name["en_US"] = "SKT T1 Ekko";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 12;
		skin5.Name["en_US"] = "Trick or Treat Ekko";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 19;
		skin6.Name["en_US"] = "True Damage Ekko";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 28;
		skin7.Name["en_US"] = "Pulsefire Ekko";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 36;
		skin8.Name["en_US"] = "Firelight Ekko";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Ekko"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Death Blossom Elise";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Victorious Elise";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Blood Moon Elise";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "SKT T1 Elise";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Super Galaxy Elise";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Bewitching Elise";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "Withered Rose Elise";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Elise"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Shadow Evelynn";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Masquerade Evelynn";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Tango Evelynn";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Safecracker Evelynn";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blood Moon Evelynn";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "K/DA Evelynn";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Prestige K/DA Evelynn";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Sugar Rush Evelynn";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "K/DA ALL OUT Evelynn";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 24;
		skin10.Name["en_US"] = "Coven Evelynn";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 31;
		skin11.Name["en_US"] = "Prestige K/DA Evelynn (2022)";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Evelynn"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Nottingham Ezreal";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Striker Ezreal";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Frosted Ezreal";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Explorer Ezreal";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Pulsefire Ezreal";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "TPA Ezreal";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Debonair Ezreal";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Ace of Spades Ezreal";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Arcade Ezreal";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 18;
		skin10.Name["en_US"] = "Star Guardian Ezreal";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 19;
		skin11.Name["en_US"] = "SSG Ezreal";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 20;
		skin12.Name["en_US"] = "Pajama Guardian Ezreal";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 21;
		skin13.Name["en_US"] = "Battle Academia Ezreal";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 22;
		skin14.Name["en_US"] = "PsyOps Ezreal";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 23;
		skin15.Name["en_US"] = "Prestige PsyOps Ezreal";
		skin15.Chromas = false;
		heroSkins.push_back(skin15);

		Skin skin16;
		skin16.Id = 25;
		skin16.Name["en_US"] = "Porcelain Protector Ezreal";
		skin16.Chromas = false;
		heroSkins.push_back(skin16);

		SkinsDB["Ezreal"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Royal Guard Fiora";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Nightraven Fiora";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Headmistress Fiora";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "PROJECT: Fiora";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Pool Party Fiora";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 22;
		skin6.Name["en_US"] = "Soaring Sword Fiora";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 23;
		skin7.Name["en_US"] = "Heartpiercer Fiora";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 31;
		skin8.Name["en_US"] = "iG Fiora";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 41;
		skin9.Name["en_US"] = "Pulsefire Fiora";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 50;
		skin10.Name["en_US"] = "Lunar Beast Fiora";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 51;
		skin11.Name["en_US"] = "Prestige Lunar Beast Fiora";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 60;
		skin12.Name["en_US"] = "Bewitching Fiora";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["Fiora"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Atlantean Fizz";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Tundra Fizz";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Fisherman Fizz";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Void Fizz";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "Cottontail Fizz";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Super Galaxy Fizz";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Omega Squad Fizz";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Fuzz Fizz";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "Prestige Fuzz Fizz";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 16;
		skin10.Name["en_US"] = "Little Devil Fizz";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 25;
		skin11.Name["en_US"] = "Prestige Fuzz Fizz (2022)";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Fizz"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Spectral Fiddlesticks";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Union Jack Fiddlesticks";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Bandito Fiddlesticks";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pumpkinhead Fiddlesticks";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Fiddle Me Timbers";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Surprise Party Fiddlesticks";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Dark Candy Fiddlesticks";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Risen Fiddlesticks";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Praetorian Fiddlesticks";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Fiddlesticks"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Enchanted Galio";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Hextech Galio";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Commando Galio";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Gatekeeper Galio";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Debonair Galio";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Birdio";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 13;
		skin7.Name["en_US"] = "Infernal Galio";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 19;
		skin8.Name["en_US"] = "Dragon Guardian Galio";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Galio"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Spooky Gangplank";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Minuteman Gangplank";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Sailor Gangplank";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Toy Soldier Gangplank";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Special Forces Gangplank";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Sultan Gangplank";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Captain Gangplank";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Dreadnova Gangplank";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 14;
		skin9.Name["en_US"] = "Pool Party Gangplank";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 21;
		skin10.Name["en_US"] = "FPX Gangplank";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Gangplank"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sanguine Garen";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Desert Trooper Garen";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Commando Garen";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Dreadknight Garen";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Rugged Garen";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Steel Legion Garen";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Rogue Admiral Garen";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Warring Kingdoms Garen";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 13;
		skin9.Name["en_US"] = "God-King Garen";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 14;
		skin10.Name["en_US"] = "Demacia Vice Garen";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 22;
		skin11.Name["en_US"] = "Mecha Kingdoms Garen";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 23;
		skin12.Name["en_US"] = "Prestige Mecha Kingdoms Garen";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 24;
		skin13.Name["en_US"] = "Battle Academia Garen";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Garen"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Scuba Gragas";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Hillbilly Gragas";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Santa Gragas";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Gragas, Esq.";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Vandal Gragas";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Oktoberfest Gragas";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Superfan Gragas";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Fnatic Gragas";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Gragas Caskbreaker";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Arctic Ops Gragas";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 11;
		skin11.Name["en_US"] = "Warden Gragas";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Gragas"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Space Groove Gwen";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 11;
		skin2.Name["en_US"] = "Cafe Cuties Gwen";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Gwen"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Hired Gun Graves";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Jailbreak Graves";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Crime City Graves";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Riot Graves";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Pool Party Graves";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Cutthroat Graves";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Snow Day Graves";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Victorious Graves";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 18;
		skin9.Name["en_US"] = "Praetorian Graves";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 25;
		skin10.Name["en_US"] = "Battle Professor Graves";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 35;
		skin11.Name["en_US"] = "Sentinel Graves";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Graves"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dino Gnar";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Gentleman Gnar";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Snow Day Gnar";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "El León Gnar";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 13;
		skin5.Name["en_US"] = "Super Galaxy Gnar";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 14;
		skin6.Name["en_US"] = "SSG Gnar";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "Astronaut Gnar";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 22;
		skin8.Name["en_US"] = "Elderwood Gnar";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Gnar"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Blood Knight Hecarim";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Reaper Hecarim";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Headless Hecarim";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Arcade Hecarim";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Elderwood Hecarim";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Worldbreaker Hecarim";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Lancer Zero Hecarim";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "High Noon Hecarim";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 14;
		skin9.Name["en_US"] = "Cosmic Charger Hecarim";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 22;
		skin10.Name["en_US"] = "Arcana Hecarim";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Hecarim"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Alien Invader Heimerdinger";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Blast Zone Heimerdinger";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Piltover Customs Heimerdinger";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Snowmerdinger";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Hazmat Heimerdinger";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dragon Trainer Heimerdinger";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "Pool Party Heimerdinger";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 24;
		skin8.Name["en_US"] = "Heimerstinger";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		SkinsDB["Heimerdinger"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Void Bringer Illaoi";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Resistance Illaoi";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 10;
		skin3.Name["en_US"] = "Cosmic Invoker Illaoi";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		SkinsDB["Illaoi"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Nightblade Irelia";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Aviator Irelia";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Infiltrator Irelia";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Frostblade Irelia";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Order of the Lotus Irelia";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Divine Sword Irelia";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "iG Irelia";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "PROJECT: Irelia";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "Prestige PROJECT: Irelia";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 18;
		skin10.Name["en_US"] = "High Noon Irelia";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 26;
		skin11.Name["en_US"] = "Sentinel Irelia";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 36;
		skin12.Name["en_US"] = "Prestige PROJECT: Irelia (2022)";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["Irelia"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Candy King Ivern";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Dunkmaster Ivern";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 11;
		skin3.Name["en_US"] = "Old God Ivern";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		SkinsDB["Ivern"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Tempest Janna";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Hextech Janna";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Frost Queen Janna";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Victorious Janna";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Forecast Janna";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Fnatic Janna";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Star Guardian Janna";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Sacred Sword Janna";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 13;
		skin9.Name["en_US"] = "Bewitching Janna";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 20;
		skin10.Name["en_US"] = "Guardian of the Sands Janna";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 27;
		skin11.Name["en_US"] = "Battle Queen Janna";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 36;
		skin12.Name["en_US"] = "Crystal Rose Janna";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Janna"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Commando Jarvan IV";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Dragonslayer Jarvan IV";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Darkforge Jarvan IV";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Victorious Jarvan IV";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warring Kingdoms Jarvan IV";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Fnatic Jarvan IV";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Dark Star Jarvan IV";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "SSG Jarvan IV";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Hextech Jarvan IV";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 11;
		skin10.Name["en_US"] = "Pool Party Jarvan IV";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 21;
		skin11.Name["en_US"] = "Lunar Beast Jarvan IV";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 30;
		skin12.Name["en_US"] = "Worlds 2021 Jarvan IV";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["JarvanIV"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "The Mighty Jax";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Vandal Jax";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Angler Jax";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "PAX Jax";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Jaximus";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Temple Jax";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Nemesis Jax";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "SKT T1 Jax";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Warden Jax";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 13;
		skin10.Name["en_US"] = "God Staff Jax";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 14;
		skin11.Name["en_US"] = "Mecha Kingdoms Jax";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 20;
		skin12.Name["en_US"] = "Conqueror Jax";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 21;
		skin13.Name["en_US"] = "Prestige Conqueror Jax";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		SkinsDB["Jax"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "High Noon Jhin";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Blood Moon Jhin";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SKT T1 Jhin";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "PROJECT: Jhin";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Dark Cosmic Jhin";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 14;
		skin6.Name["en_US"] = "Shan Hai Scrolls Jhin";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 23;
		skin7.Name["en_US"] = "DWG Jhin";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Jhin"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Full Metal Jayce";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Debonair Jayce";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Forsaken Jayce";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Jayce Brighthammer";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Battle Academia Jayce";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 15;
		skin6.Name["en_US"] = "Resistance Jayce";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 24;
		skin7.Name["en_US"] = "Arcane Jayce";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		SkinsDB["Jayce"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Crime City Jinx";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Firecracker Jinx";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Zombie Slayer Jinx";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Star Guardian Jinx";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 12;
		skin5.Name["en_US"] = "Ambitious Elf Jinx";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 13;
		skin6.Name["en_US"] = "Odyssey Jinx";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 20;
		skin7.Name["en_US"] = "PROJECT: Jinx";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 29;
		skin8.Name["en_US"] = "Heartseeker Jinx";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 37;
		skin9.Name["en_US"] = "Arcane Jinx";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 38;
		skin10.Name["en_US"] = "Battle Cat Jinx";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 40;
		skin11.Name["en_US"] = "Prestige Battle Cat Jinx";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Jinx"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Bullet Angel Kai'Sa";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 14;
		skin2.Name["en_US"] = "K/DA Kai'Sa";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 15;
		skin3.Name["en_US"] = "Prestige K/DA Kai'Sa";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 16;
		skin4.Name["en_US"] = "iG Kai'Sa";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 17;
		skin5.Name["en_US"] = "Arcade Kai'Sa";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 26;
		skin6.Name["en_US"] = "K/DA ALL OUT Kai'Sa";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 27;
		skin7.Name["en_US"] = "Prestige K/DA ALL OUT Kai'Sa";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 29;
		skin8.Name["en_US"] = "Lagoon Dragon Kai'Sa";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 39;
		skin9.Name["en_US"] = "Prestige K/DA Kai'Sa (2022)";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		SkinsDB["Kaisa"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Phantom Karthus";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Statue of Karthus";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Grim Reaper Karthus";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pentakill Karthus";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Fnatic Karthus";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Karthus Lightsbane";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Infernal Karthus";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 17;
		skin8.Name["en_US"] = "Pentakill III: Lost Chapter Karthus";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Karthus"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sun Goddess Karma";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sakura Karma";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Traditional Karma";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Order of the Lotus Karma";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warden Karma";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Winter Wonder Karma";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Conqueror Karma";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Dark Star Karma";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 19;
		skin9.Name["en_US"] = "Dawnbringer Karma";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 26;
		skin10.Name["en_US"] = "Odyssey Karma";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 27;
		skin11.Name["en_US"] = "Ruined Karma";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 44;
		skin12.Name["en_US"] = "Tranquility Dragon Karma";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["Karma"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Festival Kassadin";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Deep One Kassadin";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Pre-Void Kassadin";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Harbinger Kassadin";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Cosmic Reaver Kassadin";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Count Kassadin";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 14;
		skin7.Name["en_US"] = "Hextech Kassadin";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 15;
		skin8.Name["en_US"] = "Shockblade Kassadin";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		SkinsDB["Kassadin"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Blood Moon Kalista";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Championship Kalista";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SKT T1 Kalista";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 5;
		skin4.Name["en_US"] = "Marauder Kalista";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Kalista"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Mercenary Katarina";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Red Card Katarina";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Bilgewater Katarina";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Kitty Cat Katarina";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "High Command Katarina";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Sandstorm Katarina";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Slay Belle Katarina";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Warring Kingdoms Katarina";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "PROJECT: Katarina";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Death Sworn Katarina";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 12;
		skin11.Name["en_US"] = "Battle Academia Katarina";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 21;
		skin12.Name["en_US"] = "Blood Moon Katarina";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 29;
		skin13.Name["en_US"] = "Battle Queen Katarina";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Katarina"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Deadly Kennen";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Swamp Master Kennen";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Karate Kennen";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Kennen M.D.";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Arctic Ops Kennen";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Blood Moon Kennen";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Super Kennen";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Infernal Kennen";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 23;
		skin9.Name["en_US"] = "DWG Kennen";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Kennen"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Silver Kayle";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Viridian Kayle";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Transcended Kayle";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Battleborn Kayle";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Judgment Kayle";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Aether Wing Kayle";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Riot Kayle";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Iron Inquisitor Kayle";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Pentakill Kayle";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 15;
		skin10.Name["en_US"] = "PsyOps Kayle";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 24;
		skin11.Name["en_US"] = "Dragonslayer Kayle";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 33;
		skin12.Name["en_US"] = "Pentakill III: Lost Chapter Kayle";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Kayle"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Shadowfire Kindred";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Super Galaxy Kindred";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Spirit Blossom Kindred";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 12;
		skin4.Name["en_US"] = "Porcelain Kindred";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Kindred"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Mecha Kha'Zix";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Guardian of the Sands Kha'Zix";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Death Blossom Kha'Zix";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Dark Star Kha'Zix";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 11;
		skin5.Name["en_US"] = "Championship Kha'Zix";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 60;
		skin6.Name["en_US"] = "Odyssey Kha'Zix";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		SkinsDB["Khazix"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Soulhunter Kayn";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Odyssey Kayn";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 8;
		skin3.Name["en_US"] = "Nightbringer Kayn";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 9;
		skin4.Name["en_US"] = "Prestige Nightbringer Kayn";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		SkinsDB["Kayn"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Caterpillar Kog'Maw";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sonoran Kog'Maw";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Monarch Kog'Maw";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Reindeer Kog'Maw";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Lion Dance Kog'Maw";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Deep Sea Kog'Maw";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Jurassic Kog'Maw";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Battlecast Kog'Maw";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Pug'Maw";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Hextech Kog'Maw";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 19;
		skin11.Name["en_US"] = "Arcanist Kog'Maw";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 28;
		skin12.Name["en_US"] = "Bee'Maw";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["KogMaw"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sir Kled";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Count Kledula";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 9;
		skin3.Name["en_US"] = "Marauder Kled";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		SkinsDB["Kled"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Bloodstone Lissandra";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Blade Queen Lissandra";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Program Lissandra";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Coven Lissandra";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 12;
		skin5.Name["en_US"] = "Dark Cosmic Lissandra";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 23;
		skin6.Name["en_US"] = "Porcelain Lissandra";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Lissandra"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Valkyrie Leona";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Defender Leona";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Iron Solari Leona";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pool Party Leona";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "PROJECT: Leona";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Barbecue Leona";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Solar Eclipse Leona";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Lunar Eclipse Leona";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Mecha Kingdoms Leona";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 21;
		skin10.Name["en_US"] = "Battle Academia Leona";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 22;
		skin11.Name["en_US"] = "DWG Leona";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 23;
		skin12.Name["en_US"] = "Prestige Battle Academia Leona";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 33;
		skin13.Name["en_US"] = "Debonair Leona";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Leona"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Traditional Lee Sin";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Acolyte Lee Sin";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Dragon Fist Lee Sin";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Muay Thai Lee Sin";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Pool Party Lee Sin";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "SKT T1 Lee Sin";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Knockout Lee Sin";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "God Fist Lee Sin";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Playmaker Lee Sin";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 27;
		skin10.Name["en_US"] = "Nightbringer Lee Sin";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 28;
		skin11.Name["en_US"] = "Prestige Nightbringer Lee Sin";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 29;
		skin12.Name["en_US"] = "FPX Lee Sin";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 31;
		skin13.Name["en_US"] = "Storm Dragon Lee Sin";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 39;
		skin14.Name["en_US"] = "Prestige Nightbringer Lee Sin (2022)";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		SkinsDB["LeeSin"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Spirit Blossom Lillia";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Nightbringer Lillia";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Lillia"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Hired Gun Lucian";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Striker Lucian";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 6;
		skin3.Name["en_US"] = "PROJECT: Lucian";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 7;
		skin4.Name["en_US"] = "Heartseeker Lucian";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "High Noon Lucian";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Demacia Vice Lucian";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 18;
		skin7.Name["en_US"] = "Pulsefire Lucian";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 19;
		skin8.Name["en_US"] = "Prestige Pulsefire Lucian";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 25;
		skin9.Name["en_US"] = "Victorious Lucian";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 31;
		skin10.Name["en_US"] = "Arcana Lucian";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Lucian"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Bittersweet Lulu";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Wicked Lulu";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Dragon Trainer Lulu";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Winter Wonder Lulu";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Pool Party Lulu";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Star Guardian Lulu";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 14;
		skin7.Name["en_US"] = "Cosmic Enchantress Lulu";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 15;
		skin8.Name["en_US"] = "Pajama Guardian Lulu";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 26;
		skin9.Name["en_US"] = "Space Groove Lulu";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 27;
		skin10.Name["en_US"] = "Prestige Space Groove Lulu";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		SkinsDB["Lulu"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sorceress Lux";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Spellthief Lux";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Commando Lux";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Imperial Lux";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Steel Legion Lux";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Star Guardian Lux";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Elementalist Lux";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Lunar Empress Lux";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 14;
		skin9.Name["en_US"] = "Pajama Guardian Lux";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 15;
		skin10.Name["en_US"] = "Battle Academia Lux";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 16;
		skin11.Name["en_US"] = "Prestige Battle Academia Lux";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 17;
		skin12.Name["en_US"] = "Dark Cosmic Lux";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 18;
		skin13.Name["en_US"] = "Cosmic Lux";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 19;
		skin14.Name["en_US"] = "Space Groove Lux";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 29;
		skin15.Name["en_US"] = "Porcelain Lux";
		skin15.Chromas = true;
		heroSkins.push_back(skin15);

		Skin skin16;
		skin16.Id = 39;
		skin16.Name["en_US"] = "Prestige Battle Academia Lux (2022)";
		skin16.Chromas = false;
		heroSkins.push_back(skin16);

		Skin skin17;
		skin17.Id = 40;
		skin17.Name["en_US"] = "Prestige Porcelain Lux";
		skin17.Chromas = false;
		heroSkins.push_back(skin17);

		SkinsDB["Lux"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Wicked LeBlanc";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Prestigious LeBlanc";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Mistletoe LeBlanc";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Ravenborn LeBlanc";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Elderwood LeBlanc";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 12;
		skin6.Name["en_US"] = "Program LeBlanc";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 19;
		skin7.Name["en_US"] = "iG LeBlanc";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 20;
		skin8.Name["en_US"] = "Coven LeBlanc";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 29;
		skin9.Name["en_US"] = "Championship LeBlanc";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 33;
		skin10.Name["en_US"] = "Prestige Coven LeBlanc";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 35;
		skin11.Name["en_US"] = "Debonair LeBlanc";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Leblanc"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Vizier Malzahar";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Shadow Prince Malzahar";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Djinn Malzahar";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Overlord Malzahar";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Snow Day Malzahar";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Battle Boss Malzahar";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Hextech Malzahar";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 9;
		skin8.Name["en_US"] = "Worldbreaker Malzahar";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 18;
		skin9.Name["en_US"] = "Beezahar";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 28;
		skin10.Name["en_US"] = "Debonair Malzahar";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Malzahar"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Shamrock Malphite";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Coral Reef Malphite";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Marble Malphite";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Obsidian Malphite";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Glacial Malphite";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Mecha Malphite";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Ironside Malphite";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Odyssey Malphite";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 23;
		skin9.Name["en_US"] = "Dark Star Malphite";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 24;
		skin10.Name["en_US"] = "Prestige Dark Star Malphite";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 25;
		skin11.Name["en_US"] = "FPX Malphite";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 27;
		skin12.Name["en_US"] = "Old God Malphite";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Malphite"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Charred Maokai";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Totemic Maokai";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Festive Maokai";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Haunted Maokai";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Goalkeeper Maokai";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Meowkai";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Victorious Maokai";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Worldbreaker Maokai";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 24;
		skin9.Name["en_US"] = "Astronaut Maokai";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Maokai"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Assassin Master Yi";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Chosen Master Yi";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Ionia Master Yi";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Samurai Yi";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Headhunter Master Yi";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "PROJECT: Yi";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Cosmic Blade Master Yi";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Eternal Sword Yi";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "Snow Man Yi";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 24;
		skin10.Name["en_US"] = "Blood Moon Master Yi";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 33;
		skin11.Name["en_US"] = "PsyOps Master Yi";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 42;
		skin12.Name["en_US"] = "Debonair Master Yi";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["MasterYi"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Exiled Morgana";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sinful Succulence Morgana";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Blade Mistress Morgana";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Blackthorn Morgana";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Ghost Bride Morgana";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Victorious Morgana";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Lunar Wraith Morgana";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Bewitching Morgana";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "Majestic Empress Morgana";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 26;
		skin10.Name["en_US"] = "Coven Morgana";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 39;
		skin11.Name["en_US"] = "Dawnbringer Morgana";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 41;
		skin12.Name["en_US"] = "Prestige Bewitching Morgana";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["Morgana"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Volcanic Wukong";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "General Wukong";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Jade Dragon Wukong";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Underworld Wukong";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Radiant Wukong";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Lancer Stratus Wukong";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Battle Academia Wukong";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["MonkeyKing"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Koi Nami";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "River Spirit Nami";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Urf the Nami-tee";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 7;
		skin4.Name["en_US"] = "Deep Sea Nami";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "SKT T1 Nami";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Program Nami";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "Splendid Staff Nami";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 24;
		skin8.Name["en_US"] = "Cosmic Destiny Nami";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 32;
		skin9.Name["en_US"] = "Bewitching Nami";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Nami"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Galactic Nasus";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pharaoh Nasus";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Dreadknight Nasus";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Riot K-9 Nasus";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Infernal Nasus";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Archduke Nasus";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Worldbreaker Nasus";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Lunar Guardian Nasus";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 16;
		skin9.Name["en_US"] = "Battlecast Nasus";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 25;
		skin10.Name["en_US"] = "Space Groove Nasus";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Nasus"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dragon Knight Mordekaiser";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Infernal Mordekaiser";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Pentakill Mordekaiser";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Lord Mordekaiser";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "King of Clubs Mordekaiser";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dark Star Mordekaiser";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 13;
		skin7.Name["en_US"] = "PROJECT: Mordekaiser";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 23;
		skin8.Name["en_US"] = "Pentakill III: Lost Chapter Mordekaiser";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Mordekaiser"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Cowgirl Miss Fortune";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Waterloo Miss Fortune";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Secret Agent Miss Fortune";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Candy Cane Miss Fortune";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Road Warrior Miss Fortune";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Crime City Miss Fortune";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Arcade Miss Fortune";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Captain Fortune";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Pool Party Miss Fortune";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 15;
		skin10.Name["en_US"] = "Star Guardian Miss Fortune";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 16;
		skin11.Name["en_US"] = "Gun Goddess Miss Fortune";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 17;
		skin12.Name["en_US"] = "Pajama Guardian Miss Fortune";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 18;
		skin13.Name["en_US"] = "Bewitching Miss Fortune";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 20;
		skin14.Name["en_US"] = "Prestige Bewitching Miss Fortune";
		skin14.Chromas = false;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 21;
		skin15.Name["en_US"] = "Ruined Miss Fortune";
		skin15.Chromas = true;
		heroSkins.push_back(skin15);

		Skin skin16;
		skin16.Id = 31;
		skin16.Name["en_US"] = "Battle Bunny Miss Fortune";
		skin16.Chromas = true;
		heroSkins.push_back(skin16);

		Skin skin17;
		skin17.Id = 33;
		skin17.Name["en_US"] = "Prestige Bewitching Miss Fortune (2022)";
		skin17.Chromas = false;
		heroSkins.push_back(skin17);

		SkinsDB["MissFortune"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Abyssal Nautilus";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Subterranean Nautilus";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "AstroNautilus";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Warden Nautilus";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Worldbreaker Nautilus";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Conqueror Nautilus";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 9;
		skin7.Name["en_US"] = "Shan Hai Scrolls Nautilus";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		SkinsDB["Nautilus"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Winter Wonder Neeko";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Star Guardian Neeko";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 11;
		skin3.Name["en_US"] = "Prestige Star Guardian Neeko";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 12;
		skin4.Name["en_US"] = "Shan Hai Scrolls Neeko";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 21;
		skin5.Name["en_US"] = "Prestige Star Guardian Neeko (2022)";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		SkinsDB["Neeko"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Frozen Terror Nocturne";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Void Nocturne";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Ravager Nocturne";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Haunting Nocturne";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Eternum Nocturne";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Cursed Revenant Nocturne";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Old God Nocturne";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Hextech Nocturne";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		SkinsDB["Nocturne"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Snow Bunny Nidalee";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Leopard Nidalee";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "French Maid Nidalee";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pharaoh Nidalee";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Bewitching Nidalee";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Headhunter Nidalee";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Warring Kingdoms Nidalee";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Challenger Nidalee";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Super Galaxy Nidalee";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 11;
		skin10.Name["en_US"] = "Dawnbringer Nidalee";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 18;
		skin11.Name["en_US"] = "Cosmic Huntress Nidalee";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 27;
		skin12.Name["en_US"] = "DWG Nidalee";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Nidalee"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Gothic Orianna";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sewn Chaos Orianna";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Bladecraft Orianna";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "TPA Orianna";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Winter Wonder Orianna";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Heartseeker Orianna";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Dark Star Orianna";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Victorious Orianna";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 11;
		skin9.Name["en_US"] = "Pool Party Orianna";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 20;
		skin10.Name["en_US"] = "Orbeeanna";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Orianna"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Forsaken Olaf";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Glacial Olaf";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Brolaf";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pentakill Olaf";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Marauder Olaf";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Butcher Olaf";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "SKT T1 Olaf";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Dragonslayer Olaf";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 25;
		skin9.Name["en_US"] = "Sentinel Olaf";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 35;
		skin10.Name["en_US"] = "Pentakill III: Lost Chapter Olaf";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Olaf"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sasquatch Nunu & Willump";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Workshop Nunu & Willump";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Grungy Nunu & Willump";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Nunu & Willump Bot";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Demolisher Nunu & Willump";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "TPA Nunu & Willump";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Zombie Nunu & Willump";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Papercraft Nunu & Willump";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 16;
		skin9.Name["en_US"] = "Space Groove Nunu & Willump";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 26;
		skin10.Name["en_US"] = "Nunu & Beelump";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Nunu"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Thunder Lord Ornn";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Elderwood Ornn";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Ornn"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Myrmidon Pantheon";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Ruthless Pantheon";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Perseus Pantheon";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Full Metal Pantheon";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Glaive Warrior Pantheon";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dragonslayer Pantheon";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Zombie Slayer Pantheon";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Baker Pantheon";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 16;
		skin9.Name["en_US"] = "Pulsefire Pantheon";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 25;
		skin10.Name["en_US"] = "Ruined Pantheon";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 26;
		skin11.Name["en_US"] = "Prestige Ascended Pantheon";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Pantheon"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Battle Boss Qiyana";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "True Damage Qiyana";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 10;
		skin3.Name["en_US"] = "Prestige True Damage Qiyana";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 11;
		skin4.Name["en_US"] = "Battle Queen Qiyana";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 20;
		skin5.Name["en_US"] = "Shockblade Qiyana";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 21;
		skin6.Name["en_US"] = "Prestige True Damage Qiyana (2022)";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		SkinsDB["Qiyana"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Noxus Poppy";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Lollipoppy";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Blacksmith Poppy";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Ragdoll Poppy";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Battle Regalia Poppy";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Scarlet Hammer Poppy";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Star Guardian Poppy";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Snow Fawn Poppy";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "Hextech Poppy";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 16;
		skin10.Name["en_US"] = "Astronaut Poppy";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 24;
		skin11.Name["en_US"] = "Bewitching Poppy";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Poppy"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sand Wraith Pyke";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 9;
		skin2.Name["en_US"] = "Blood Moon Pyke";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 16;
		skin3.Name["en_US"] = "PROJECT: Pyke";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 25;
		skin4.Name["en_US"] = "PsyOps Pyke";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 34;
		skin5.Name["en_US"] = "Sentinel Pyke";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 44;
		skin6.Name["en_US"] = "Ashen Knight Pyke";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Pyke"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Phoenix Quinn";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Woad Scout Quinn";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Corsair Quinn";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Heartseeker Quinn";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warden Quinn";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Quinn"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Cosmic Dawn Rakan";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sweetheart Rakan";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SSG Rakan";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "iG Rakan";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Star Guardian Rakan";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 9;
		skin6.Name["en_US"] = "Elderwood Rakan";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 18;
		skin7.Name["en_US"] = "Arcana Rakan";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Rakan"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "King Rammus";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Chrome Rammus";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Molten Rammus";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Freljord Rammus";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Ninja Rammus";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Full Metal Rammus";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Guardian of the Sands Rammus";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Sweeper Rammus";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 16;
		skin9.Name["en_US"] = "Hextech Rammus";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 17;
		skin10.Name["en_US"] = "Astronaut Rammus";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Rammus"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Battle Queen Rell";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		SkinsDB["Rell"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Admiral Glasc";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		SkinsDB["Renata"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Eternum Rek'Sai";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pool Party Rek'Sai";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 9;
		skin3.Name["en_US"] = "Blackfrost Rek'Sai";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 17;
		skin4.Name["en_US"] = "Elderwood Rek'Sai";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["RekSai"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Galactic Renekton";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Outback Renekton";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Bloodfury Renekton";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Rune Wars Renekton";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Scorched Earth Renekton";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Pool Party Renekton";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Prehistoric Renekton";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "SKT T1 Renekton";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Renektoy";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 17;
		skin10.Name["en_US"] = "Hextech Renekton";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 18;
		skin11.Name["en_US"] = "Blackfrost Renekton";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 26;
		skin12.Name["en_US"] = "PROJECT: Renekton";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Renekton"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Redeemed Riven";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Crimson Elite Riven";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Battle Bunny Riven";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Championship Riven";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Dragonblade Riven";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Arcade Riven";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Championship Riven 2016";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Dawnbringer Riven";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 18;
		skin9.Name["en_US"] = "Pulsefire Riven";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 20;
		skin10.Name["en_US"] = "Valiant Sword Riven";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 22;
		skin11.Name["en_US"] = "Prestige Valiant Sword Riven";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 23;
		skin12.Name["en_US"] = "Spirit Blossom Riven";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 34;
		skin13.Name["en_US"] = "Sentinel Riven";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 44;
		skin14.Name["en_US"] = "Battle Bunny Prime Riven";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 45;
		skin15.Name["en_US"] = "Prestige Valiant Sword Riven (2022)";
		skin15.Chromas = false;
		heroSkins.push_back(skin15);

		SkinsDB["Riven"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sabretusk Sejuani";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Darkrider Sejuani";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Traditional Sejuani";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Bear Cavalry Sejuani";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Poro Rider Sejuani";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Beast Hunter Sejuani";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Sejuani Dawnchaser";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Firecracker Sejuani";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "Hextech Sejuani";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 16;
		skin10.Name["en_US"] = "PROJECT: Sejuani";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Sejuani"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Young Ryze";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Tribal Ryze";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Uncle Ryze";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Triumphant Ryze";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Professor Ryze";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Zombie Ryze";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Dark Crystal Ryze";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Pirate Ryze";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Ryze Whitebeard";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "SKT T1 Ryze";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 11;
		skin11.Name["en_US"] = "Championship Ryze";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 13;
		skin12.Name["en_US"] = "Guardian of the Sands Ryze";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 20;
		skin13.Name["en_US"] = "Arcana Ryze";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Ryze"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Rumble in the Jungle";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Bilgerat Rumble";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Super Galaxy Rumble";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Badlands Baron Rumble";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 13;
		skin5.Name["en_US"] = "Space Groove Rumble";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Rumble"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "PsyOps Samira";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Space Groove Samira";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		SkinsDB["Samira"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Headhunter Rengar";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Night Hunter Rengar";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SSW Rengar";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 8;
		skin4.Name["en_US"] = "Mecha Rengar";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 15;
		skin5.Name["en_US"] = "Pretty Kitty Rengar";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 23;
		skin6.Name["en_US"] = "Guardian of the Sands Rengar";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 30;
		skin7.Name["en_US"] = "Sentinel Rengar";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Rengar"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Mecha Kingdoms Sett";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 8;
		skin2.Name["en_US"] = "Obsidian Dragon Sett";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 9;
		skin3.Name["en_US"] = "Prestige Obsidian Dragon Sett";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 10;
		skin4.Name["en_US"] = "Pool Party Sett";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 19;
		skin5.Name["en_US"] = "Firecracker Sett";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Sett"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Mad Hatter Shaco";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Royal Shaco";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Nutcracko";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Workshop Shaco";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Asylum Shaco";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Masked Shaco";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Wild Card Shaco";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Dark Star Shaco";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "Arcanist Shaco";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 23;
		skin10.Name["en_US"] = "Crime City Nightmare Shaco";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Shaco"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "K/DA ALL OUT Seraphine Indie";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "K/DA ALL OUT Seraphine Rising Star";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "K/DA ALL OUT Seraphine Superstar";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Graceful Phoenix Seraphine";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Seraphine"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "True Damage Senna";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 9;
		skin2.Name["en_US"] = "Prestige True Damage Senna";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 10;
		skin3.Name["en_US"] = "High Noon Senna";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 16;
		skin4.Name["en_US"] = "PROJECT: Senna";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Senna"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Ironscale Shyvana";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Boneclaw Shyvana";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Darkflame Shyvana";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Ice Drake Shyvana";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Championship Shyvana";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Super Galaxy Shyvana";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 8;
		skin7.Name["en_US"] = "Ruined Shyvana";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Shyvana"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Frozen Shen";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Yellow Jacket Shen";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Surgeon Shen";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Blood Moon Shen";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warlord Shen";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "TPA Shen";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "Pulsefire Shen";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Infernal Shen";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 22;
		skin9.Name["en_US"] = "PsyOps Shen";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 40;
		skin10.Name["en_US"] = "Shockblade Shen";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Shen"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Riot Squad Singed";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Hextech Singed";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Surfer Singed";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Mad Scientist Singed";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Augmented Singed";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Snow Day Singed";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "SSW Singed";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Black Scourge Singed";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Beekeeper Singed";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Resistance Singed";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Singed"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Warrior Princess Sivir";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Spectacular Sivir";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Huntress Sivir";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Bandit Sivir";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "PAX Sivir";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Snowstorm Sivir";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Warden Sivir";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Victorious Sivir";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Neo PAX Sivir";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Pizza Delivery Sivir";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 16;
		skin11.Name["en_US"] = "Blood Moon Sivir";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 25;
		skin12.Name["en_US"] = "Odyssey Sivir";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 34;
		skin13.Name["en_US"] = "Cafe Cuties Sivir";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Sivir"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Hextech Sion";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Barbarian Sion";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Lumberjack Sion";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Warmonger Sion";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Mecha Zero Sion";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 14;
		skin6.Name["en_US"] = "Worldbreaker Sion";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 22;
		skin7.Name["en_US"] = "Blackfrost Sion";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		SkinsDB["Sion"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Sandscourge Skarner";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Earthrune Skarner";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Battlecast Alpha Skarner";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Guardian of the Sands Skarner";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Cosmic Sting Skarner";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Skarner"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dryad Soraka";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Divine Soraka";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Celestine Soraka";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Reaper Soraka";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Order of the Banana Soraka";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Program Soraka";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Star Guardian Soraka";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Pajama Guardian Soraka";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Winter Wonder Soraka";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 15;
		skin10.Name["en_US"] = "Dawnbringer Soraka";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 16;
		skin11.Name["en_US"] = "Nightbringer Soraka";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 17;
		skin12.Name["en_US"] = "Prestige Star Guardian Soraka";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 18;
		skin13.Name["en_US"] = "Cafe Cuties Soraka";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Soraka"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Muse Sona";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pentakill Sona";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Silent Night Sona";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Guqin Sona";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Arcade Sona";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "DJ Sona";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Sweetheart Sona";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 9;
		skin8.Name["en_US"] = "Odyssey Sona";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 17;
		skin9.Name["en_US"] = "PsyOps Sona";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 26;
		skin10.Name["en_US"] = "Pentakill III: Lost Chapter Sona";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Sona"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Northern Front Swain";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Bilgewater Swain";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Tyrant Swain";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Dragon Master Swain";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 11;
		skin5.Name["en_US"] = "Hextech Swain";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 12;
		skin6.Name["en_US"] = "Crystal Rose Swain";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Swain"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Lunar Wraith Sylas";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 8;
		skin2.Name["en_US"] = "Freljord Sylas";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 13;
		skin3.Name["en_US"] = "PROJECT: Sylas";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 14;
		skin4.Name["en_US"] = "Prestige PROJECT: Sylas";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 24;
		skin5.Name["en_US"] = "Battle Wolf Sylas";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Sylas"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Justicar Syndra";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Atlantean Syndra";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Queen of Diamonds Syndra";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Snow Day Syndra";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "SKT T1 Syndra";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Star Guardian Syndra";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Pool Party Syndra";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Withered Rose Syndra";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 25;
		skin9.Name["en_US"] = "Bewitching Syndra";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Syndra"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Master Chef Tahm Kench";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Urf Kench";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Coin Emperor Tahm Kench";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 11;
		skin4.Name["en_US"] = "Arcana Tahm Kench";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		SkinsDB["TahmKench"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Freljord Taliyah";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "SSG Taliyah";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Pool Party Taliyah";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		SkinsDB["Taliyah"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Renegade Talon";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Crimson Elite Talon";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Dragonblade Talon";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "SSW Talon";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blood Moon Talon";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 12;
		skin6.Name["en_US"] = "Enduring Sword Talon";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 20;
		skin7.Name["en_US"] = "Talon Blackwood";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 29;
		skin8.Name["en_US"] = "Withered Rose Talon";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Talon"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Emerald Taric";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Armor of the Fifth Age Taric";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Bloodstone Taric";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Pool Party Taric";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 9;
		skin5.Name["en_US"] = "Taric Luminshield";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Taric"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Happy Elf Teemo";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Recon Teemo";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Badger Teemo";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Astronaut Teemo";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Cottontail Teemo";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Super Teemo";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Panda Teemo";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Omega Squad Teemo";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 14;
		skin9.Name["en_US"] = "Little Devil Teemo";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 18;
		skin10.Name["en_US"] = "Beemo";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 25;
		skin11.Name["en_US"] = "Spirit Blossom Teemo";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 27;
		skin12.Name["en_US"] = "Prestige Spirit Blossom Teemo";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 37;
		skin13.Name["en_US"] = "Firecracker Teemo";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Teemo"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Riot Girl Tristana";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Earnest Elf Tristana";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Firefighter Tristana";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Guerilla Tristana";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Buccaneer Tristana";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Rocket Girl Tristana";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Dragon Trainer Tristana";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "Bewitching Tristana";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Omega Squad Tristana";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 24;
		skin10.Name["en_US"] = "Little Demon Tristana";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 33;
		skin11.Name["en_US"] = "Pengu Cosplay Tristana";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 40;
		skin12.Name["en_US"] = "Hextech Tristana";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 41;
		skin13.Name["en_US"] = "Firecracker Tristana";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		SkinsDB["Tristana"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Lil' Slugger Trundle";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Junkyard Trundle";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Traditional Trundle";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Constable Trundle";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Worldbreaker Trundle";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dragonslayer Trundle";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Trundle"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Highland Tryndamere";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "King Tryndamere";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Viking Tryndamere";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Demonblade Tryndamere";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Sultan Tryndamere";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Warring Kingdoms Tryndamere";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Nightmare Tryndamere";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Beast Hunter Tryndamere";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Chemtech Tryndamere";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Blood Moon Tryndamere";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 18;
		skin11.Name["en_US"] = "Nightbringer Tryndamere";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Tryndamere"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Deep Terror Thresh";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Championship Thresh";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Blood Moon Thresh";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "SSW Thresh";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Dark Star Thresh";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "High Noon Thresh";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 13;
		skin7.Name["en_US"] = "Pulsefire Thresh";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Prestige Pulsefire Thresh";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 15;
		skin9.Name["en_US"] = "FPX Thresh";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 17;
		skin10.Name["en_US"] = "Spirit Blossom Thresh";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 27;
		skin11.Name["en_US"] = "Unbound Thresh";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 28;
		skin12.Name["en_US"] = "Steel Dragon Thresh";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 38;
		skin13.Name["en_US"] = "Prestige Pulsefire Thresh (2022)";
		skin13.Chromas = false;
		heroSkins.push_back(skin13);

		SkinsDB["Thresh"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "PAX Twisted Fate";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Jack of Hearts Twisted Fate";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "The Magnificent Twisted Fate";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Tango Twisted Fate";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "High Noon Twisted Fate";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Musketeer Twisted Fate";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Underworld Twisted Fate";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Red Card Twisted Fate";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Cutpurse Twisted Fate";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Blood Moon Twisted Fate";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 11;
		skin11.Name["en_US"] = "Pulsefire Twisted Fate";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 13;
		skin12.Name["en_US"] = "Odyssey Twisted Fate";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 23;
		skin13.Name["en_US"] = "DWG Twisted Fate";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 25;
		skin14.Name["en_US"] = "Crime City Nightmare Twisted Fate";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		SkinsDB["TwistedFate"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Black Belt Udyr";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Primal Udyr";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Spirit Guard Udyr";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Definitely Not Udyr";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Dragon Oracle Udyr";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		SkinsDB["Udyr"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Kingpin Twitch";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Whistler Village Twitch";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Medieval Twitch";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Crime City Twitch";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Vandal Twitch";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Pickpocket Twitch";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "SSW Twitch";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Omega Squad Twitch";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Ice King Twitch";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 27;
		skin10.Name["en_US"] = "Twitch Shadowfoot";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 36;
		skin11.Name["en_US"] = "Dragonslayer Twitch";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Twitch"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Vindicator Vayne";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Aristocrat Vayne";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Dragonslayer Vayne";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Heartseeker Vayne";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "SKT T1 Vayne";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Arclight Vayne";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 10;
		skin7.Name["en_US"] = "Soulstealer Vayne";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 11;
		skin8.Name["en_US"] = "PROJECT: Vayne";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 12;
		skin9.Name["en_US"] = "Firecracker Vayne";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 13;
		skin10.Name["en_US"] = "Prestige Firecracker Vayne";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 14;
		skin11.Name["en_US"] = "Spirit Blossom Vayne";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 15;
		skin12.Name["en_US"] = "FPX Vayne";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		Skin skin13;
		skin13.Id = 25;
		skin13.Name["en_US"] = "Sentinel Vayne";
		skin13.Chromas = true;
		heroSkins.push_back(skin13);

		Skin skin14;
		skin14.Id = 32;
		skin14.Name["en_US"] = "Battle Bat Vayne";
		skin14.Chromas = true;
		heroSkins.push_back(skin14);

		Skin skin15;
		skin15.Id = 33;
		skin15.Name["en_US"] = "Prestige Firecracker Vayne (2022)";
		skin15.Chromas = false;
		heroSkins.push_back(skin15);

		SkinsDB["Vayne"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "White Mage Veigar";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Curling Veigar";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Veigar Greybeard";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Leprechaun Veigar";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Baron Von Veigar";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Superb Villain Veigar";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Bad Santa Veigar";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Final Boss Veigar";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Omega Squad Veigar";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 13;
		skin10.Name["en_US"] = "Elderwood Veigar";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 23;
		skin11.Name["en_US"] = "Furyhorn Cosplay Veigar";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 32;
		skin12.Name["en_US"] = "Astronaut Veigar";
		skin12.Chromas = true;
		heroSkins.push_back(skin12);

		SkinsDB["Veigar"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Giant Enemy Crabgot";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Butcher Urgot";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Battlecast Urgot";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 9;
		skin4.Name["en_US"] = "High Noon Urgot";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 15;
		skin5.Name["en_US"] = "Pajama Guardian Cosplay Urgot";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Urgot"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Blight Crystal Varus";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Arclight Varus";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Arctic Ops Varus";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Heartseeker Varus";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Varus Swiftbolt";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Dark Star Varus";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Conqueror Varus";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 9;
		skin8.Name["en_US"] = "Infernal Varus";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 16;
		skin9.Name["en_US"] = "PROJECT: Varus";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 17;
		skin10.Name["en_US"] = "Cosmic Hunter Varus";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Varus"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Battlecast Vel'Koz";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Arclight Vel'Koz";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Definitely Not Vel'Koz";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Infernal Vel'Koz";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 11;
		skin5.Name["en_US"] = "Blackfrost Vel'Koz";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Velkoz"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Dawnbringer Vex";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		SkinsDB["Vex"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Lunar Beast Viego";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Dissonance of Pentakill Viego";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		SkinsDB["Viego"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Neon Strike Vi";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Officer Vi";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Debonair Vi";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Demon Vi";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warring Kingdoms Vi";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 11;
		skin6.Name["en_US"] = "PROJECT: Vi";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 12;
		skin7.Name["en_US"] = "Heartbreaker Vi";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 20;
		skin8.Name["en_US"] = "PsyOps Vi";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 29;
		skin9.Name["en_US"] = "Arcane Vi";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		SkinsDB["Vi"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Thunder Lord Volibear";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Northern Storm Volibear";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Runeguard Volibear";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Captain Volibear";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "El Rayo Volibear";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "The Thousand-Pierced Bear";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Duality Dragon Volibear";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 9;
		skin8.Name["en_US"] = "Prestige Duality Dragon Volibear";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		SkinsDB["Volibear"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Cosmic Dusk Xayah";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Sweetheart Xayah";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SSG Xayah";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Star Guardian Xayah";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 8;
		skin5.Name["en_US"] = "Elderwood Xayah";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 17;
		skin6.Name["en_US"] = "Brave Phoenix Xayah";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 26;
		skin7.Name["en_US"] = "Prestige Brave Phoenix Xayah";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 28;
		skin8.Name["en_US"] = "Arcana Xayah";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Xayah"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Full Machine Viktor";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Prototype Viktor";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Creator Viktor";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Death Sworn Viktor";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "PsyOps Viktor";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		SkinsDB["Viktor"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Runeborn Xerath";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Battlecast Xerath";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Scorched Earth Xerath";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Guardian of the Sands Xerath";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Dark Star Xerath";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 12;
		skin6.Name["en_US"] = "Arcana Xerath";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Xerath"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Grey Warwick";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Urf the Manatee";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Big Bad Warwick";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Tundra Hunter Warwick";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Feral Warwick";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Firefang Warwick";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Hyena Warwick";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Marauder Warwick";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 9;
		skin9.Name["en_US"] = "Urfwick";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 10;
		skin10.Name["en_US"] = "Lunar Guardian Warwick";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 16;
		skin11.Name["en_US"] = "PROJECT: Warwick";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		Skin skin12;
		skin12.Id = 35;
		skin12.Name["en_US"] = "Old God Warwick";
		skin12.Chromas = false;
		heroSkins.push_back(skin12);

		SkinsDB["Warwick"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Count Vladimir";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Marquis Vladimir";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Nosferatu Vladimir";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Vandal Vladimir";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blood Lord Vladimir";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Soulstealer Vladimir";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Academy Vladimir";
		skin7.Chromas = false;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 8;
		skin8.Name["en_US"] = "Dark Waters Vladimir";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 14;
		skin9.Name["en_US"] = "Nightbringer Vladimir";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 21;
		skin10.Name["en_US"] = "Cosmic Devourer Vladimir";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 30;
		skin11.Name["en_US"] = "Cafe Cuties Vladimir";
		skin11.Chromas = true;
		heroSkins.push_back(skin11);

		SkinsDB["Vladimir"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "High Noon Yasuo";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "PROJECT: Yasuo";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Blood Moon Yasuo";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 9;
		skin4.Name["en_US"] = "Nightbringer Yasuo";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 10;
		skin5.Name["en_US"] = "Odyssey Yasuo";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 17;
		skin6.Name["en_US"] = "Battle Boss Yasuo";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 18;
		skin7.Name["en_US"] = "True Damage Yasuo";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 35;
		skin8.Name["en_US"] = "Prestige True Damage Yasuo";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 36;
		skin9.Name["en_US"] = "Spirit Blossom Yasuo";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 54;
		skin10.Name["en_US"] = "Truth Dragon Yasuo";
		skin10.Chromas = false;
		heroSkins.push_back(skin10);

		Skin skin11;
		skin11.Id = 55;
		skin11.Name["en_US"] = "Dream Dragon Yasuo";
		skin11.Chromas = false;
		heroSkins.push_back(skin11);

		SkinsDB["Yasuo"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Commando Xin Zhao";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Imperial Xin Zhao";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Viscero Xin Zhao";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Winged Hussar Xin Zhao";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Warring Kingdoms Xin Zhao";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Secret Agent Xin Zhao";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 13;
		skin7.Name["en_US"] = "Dragonslayer Xin Zhao";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 20;
		skin8.Name["en_US"] = "Cosmic Defender Xin Zhao";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 27;
		skin9.Name["en_US"] = "Marauder Xin Zhao";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 36;
		skin10.Name["en_US"] = "Firecracker Xin Zhao";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["XinZhao"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Spirit Blossom Yone";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 10;
		skin2.Name["en_US"] = "Battle Academia Yone";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 19;
		skin3.Name["en_US"] = "Dawnbringer Yone";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		SkinsDB["Yone"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Undertaker Yorick";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pentakill Yorick";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Arclight Yorick";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Meowrick";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 12;
		skin5.Name["en_US"] = "Resistance Yorick";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 21;
		skin6.Name["en_US"] = "Pentakill III: Lost Chapter Yorick";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Yorick"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = true;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Special Weapon Zac";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pool Party Zac";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 6;
		skin3.Name["en_US"] = "SKT T1 Zac";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 7;
		skin4.Name["en_US"] = "Battlecast Zac";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Zac"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Battle Principal Yuumi";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 11;
		skin2.Name["en_US"] = "Heartseeker Yuumi";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 19;
		skin3.Name["en_US"] = "Yuubee";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 28;
		skin4.Name["en_US"] = "Bewitching Yuumi";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		SkinsDB["Yuumi"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Mad Scientist Ziggs";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Major Ziggs";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Pool Party Ziggs";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Snow Day Ziggs";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Master Arcanist Ziggs";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Battle Boss Ziggs";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Odyssey Ziggs";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 14;
		skin8.Name["en_US"] = "Sugar Rush Ziggs";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 23;
		skin9.Name["en_US"] = "Hextech Ziggs";
		skin9.Chromas = false;
		heroSkins.push_back(skin9);

		Skin skin10;
		skin10.Id = 24;
		skin10.Name["en_US"] = "BZZZiggs";
		skin10.Chromas = true;
		heroSkins.push_back(skin10);

		SkinsDB["Ziggs"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Shockblade Zed";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "SKT T1 Zed";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "PROJECT: Zed";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 10;
		skin4.Name["en_US"] = "Championship Zed";
		skin4.Chromas = true;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 11;
		skin5.Name["en_US"] = "Death Sworn Zed";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 13;
		skin6.Name["en_US"] = "Galaxy Slayer Zed";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 15;
		skin7.Name["en_US"] = "PsyOps Zed";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 30;
		skin8.Name["en_US"] = "Prestige PROJECT: Zed";
		skin8.Chromas = false;
		heroSkins.push_back(skin8);

		Skin skin9;
		skin9.Id = 31;
		skin9.Name["en_US"] = "Debonair Zed";
		skin9.Chromas = true;
		heroSkins.push_back(skin9);

		SkinsDB["Zed"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Withered Rose Zeri";
		skin1.Chromas = true;
		heroSkins.push_back(skin1);

		SkinsDB["Zeri"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Old Saint Zilean";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Groovy Zilean";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "Shurima Desert Zilean";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Time Machine Zilean";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Blood Moon Zilean";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Sugar Rush Zilean";
		skin6.Chromas = true;
		heroSkins.push_back(skin6);

		SkinsDB["Zilean"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Cyber Pop Zoe";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Pool Party Zoe";
		skin2.Chromas = true;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 9;
		skin3.Name["en_US"] = "Star Guardian Zoe";
		skin3.Chromas = true;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 18;
		skin4.Name["en_US"] = "Arcanist Zoe";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 19;
		skin5.Name["en_US"] = "Prestige Arcanist Zoe";
		skin5.Chromas = false;
		heroSkins.push_back(skin5);

		SkinsDB["Zoe"] = heroSkins;
	}

	{
		std::vector<Skin> heroSkins;
		Skin skin0;
		skin0.Id = 0;
		skin0.Name["en_US"] = "default";
		skin0.Chromas = false;
		heroSkins.push_back(skin0);

		Skin skin1;
		skin1.Id = 1;
		skin1.Name["en_US"] = "Wildfire Zyra";
		skin1.Chromas = false;
		heroSkins.push_back(skin1);

		Skin skin2;
		skin2.Id = 2;
		skin2.Name["en_US"] = "Haunted Zyra";
		skin2.Chromas = false;
		heroSkins.push_back(skin2);

		Skin skin3;
		skin3.Id = 3;
		skin3.Name["en_US"] = "SKT T1 Zyra";
		skin3.Chromas = false;
		heroSkins.push_back(skin3);

		Skin skin4;
		skin4.Id = 4;
		skin4.Name["en_US"] = "Dragon Sorceress Zyra";
		skin4.Chromas = false;
		heroSkins.push_back(skin4);

		Skin skin5;
		skin5.Id = 5;
		skin5.Name["en_US"] = "Coven Zyra";
		skin5.Chromas = true;
		heroSkins.push_back(skin5);

		Skin skin6;
		skin6.Id = 6;
		skin6.Name["en_US"] = "Prestige Coven Zyra";
		skin6.Chromas = false;
		heroSkins.push_back(skin6);

		Skin skin7;
		skin7.Id = 7;
		skin7.Name["en_US"] = "Crystal Rose Zyra";
		skin7.Chromas = true;
		heroSkins.push_back(skin7);

		Skin skin8;
		skin8.Id = 16;
		skin8.Name["en_US"] = "Crime City Nightmare Zyra";
		skin8.Chromas = true;
		heroSkins.push_back(skin8);

		SkinsDB["Zyra"] = heroSkins;
	}





}