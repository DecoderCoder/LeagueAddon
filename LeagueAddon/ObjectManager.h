#pragma once
#include <list>
#include "Structs.h"

class ObjectManager {
public:
	//static GameObject* GetFirstObject()
	//{
	//	typedef GameObject* (__thiscall* fnGetFirst)(void*);
	//	return ((fnGetFirst)(DEFINE_RVA(Offset::Function::GetFirstObject)))(*(void**)(DEFINE_RVA(Offset::Data::ObjectManager)));
	//}
	//static GameObject* GetNextObject(GameObject* object)
	//{
	//	typedef GameObject* (__thiscall* fnGetNext)(void*, GameObject*);
	//	return ((fnGetNext)(DEFINE_RVA(Offset::Function::GetNextObject)))(*(void**)(DEFINE_RVA(Offset::Data::ObjectManager)), object);
	//}

	static GameObject* GetObjectByIndex(int Index)
	{
		for (auto obj : HeroList()) {
			if (obj->Index == Index)
				return obj;
		}
		for (auto obj : TurretList()) {
			if (obj->Index == Index)
				return obj;
		}
		for (auto obj : MinionList()) {
			if (obj->Index == Index)
				return obj;
		}
		return nullptr;
	}

	static std::list<GameObject*> GetAllObjects() {
		std::list<GameObject*> ObjectList;
		for (auto obj : HeroList()) {
			ObjectList.push_back(obj);
		}
		for (auto obj : TurretList()) {
			ObjectList.push_back(obj);
		}
		for (auto obj : MinionList()) {
			ObjectList.push_back(obj);
		}

		return ObjectList;
	}

	static GameObject* GetObjectByNetworkID(int NetworkID)
	{
		for (auto obj : HeroList()) {
			if (obj->NetworkID == NetworkID)
				return obj;
		}
		for (auto obj : TurretList()) {
			if (obj->NetworkID == NetworkID)
				return obj;
		}
		for (auto obj : MinionList()) {
			if (obj->NetworkID == NetworkID)
				return obj;
		}
		return nullptr;
	}

	static std::list<GameObject*> MinionList() {
		std::list<GameObject*> ObjectList;

		auto Object_list = *reinterpret_cast<SEntityList<GameObject>**>(DEFINE_RVA(Offset::Data::ManagerTemplate_Minions));
		for (size_t i = 0; i < Object_list->size; i++) {
			auto Object = Object_list->entities[i];
			ObjectList.push_back(Object);
		}
		return ObjectList;
	}

	static std::list<GameObject*> TurretList() {
		std::list<GameObject*> ObjectList;
		auto Object_list = *reinterpret_cast<SEntityList<GameObject>**>(DEFINE_RVA(Offset::Data::ManagerTemplate_Turrets));
		for (size_t i = 0; i < Object_list->size; i++) {
			auto Object = Object_list->entities[i];
			ObjectList.push_back(Object);
		}
		return ObjectList;
	}

	static std::list<GameObject*> HeroList() {
		std::list<GameObject*> ObjectList;
		auto Object_list = *reinterpret_cast<SEntityList<GameObject>**>(DEFINE_RVA(Offset::Data::ManagerTemplate_Heroes));
		for (size_t i = 0; i < Object_list->size; i++) {
			auto Object = Object_list->entities[i];
			ObjectList.push_back(Object);
		}
		return ObjectList;
	}

	static std::list<MissileSpellInfo*> MissileList() {

		std::list<MissileSpellInfo*> ObjectList;

		int missiles_count = *(int*)(*(int*)DEFINE_RVA(Offset::Data::ManagerTemplate_Missiles) + 0x8);
		if (missiles_count == 1) {
			auto obj = *(MissileSpellInfo**)(**(int**)(*(int*)DEFINE_RVA(Offset::Data::ManagerTemplate_Missiles) + 0x4) + 0x14);
			//MessageBoxA(0, to_hex((int)obj).c_str(), "", 0);
			ObjectList.push_back(obj);
			return ObjectList;
		}
		auto missiles_mgr = *(MissileManager**)(*(int*)DEFINE_RVA(Offset::Data::ManagerTemplate_Missiles) + 0x4);
		for (auto& i : missiles_mgr->missile_map)
		{
			DWORD network_id = i.first;
			ObjectList.push_back((MissileSpellInfo*)i.second);
		}
		return ObjectList;
	}

	static GameObject* GetLocalPlayer() {
		return (GameObject*)*(DWORD*)DEFINE_RVA(Offset::Data::LocalPlayer);
	}

	static GameObject* FindObjectByIndex(std::list<GameObject*> heroList, int casterIndex)
	{
		for (GameObject* a : heroList)
		{
			if (casterIndex == a->Index)
				return a;
		}
		return nullptr;
	}

	static MissileSpellInfo* FindObjectByIndex(std::list<MissileSpellInfo*> heroList, int casterIndex)
	{
		for (MissileSpellInfo* a : heroList)
		{
			if (casterIndex == a->Index)
				return a;
		}
		return nullptr;
	}

	static GameObject* FindObjectByIndex(int casterIndex)
	{

		for (auto obj : HeroList()) {
			if (obj->Index == casterIndex)
				return obj;
		}
		for (auto obj : TurretList()) {
			if (obj->Index == casterIndex)
				return obj;
		}
		for (auto obj : MinionList()) {
			if (obj->Index == casterIndex)
				return obj;
		}
		return nullptr;
		/*GameObject* target = nullptr;
		auto Object = GetFirstObject();
		while (Object) {
			if (casterIndex == Object->Index)
				target = Object;
			Object = GetNextObject(Object);
		}
		return target;*/
	}
};

inline GameObject* Local = nullptr;