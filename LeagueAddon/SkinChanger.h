#pragma once
#include "EventManager/EventManager.h"
#include "ObjectManager.h"
#include "Render.h"
#include <map>

namespace SkinChanger {
	class Skin {
	public:
		std::map<string, string> Name;
		int Id;
		bool Chromas;
	};

	inline string currentLang = "en_US";

	inline std::map<string, std::vector<Skin>> SkinsDB; // string = ChampionName
	inline std::map<GameObject*, int> heroesSkins;

	void changeSkin2Player(GameObject* object, int skin);

	void Initialize();

	void OnThread();
	void OnMenu();
	void OnDraw();

	void LoadSkinsDB();
}