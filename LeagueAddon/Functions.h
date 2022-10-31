#pragma once
#include "./Structs.h"
#include "Render.h"

namespace FuncType {
	typedef int* (__thiscall* fnIssueOrder)(GameObject* Object, int Order, Vector3* Position, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown);
	typedef int* (__thiscall* fnCastSpell)(DWORD* spellbook, DWORD* spelldatainst, int spellslot, Vector3* Origin, Vector3* Target, DWORD networkid);

	typedef int* (__thiscall* fnNewIssueOrder)(GameObject* Object, int Order, Vector3* Position, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown);

	typedef float(__cdecl* fnGetAttackCastDelay)(GameObject* pObj);
	typedef float(__cdecl* fnGetAttackDelay)(GameObject* pObj);

	/*typedef int* (__thiscall* fnCastSpell)(DWORD spellbook_addr, SpellSlot* spellslot, int SlotID, Vector* targetpos, Vector* startpos, DWORD NetworkID);*/
	typedef void(__thiscall* fnPrintChat)(DWORD ChatClient, const char* Message, int Color);
	typedef int(__thiscall* fnGetPing)(char* pObj);
	typedef void(__cdecl* fnDrawCircle)(Vector3* position, float range, int* color, int a4, float a5, int a6, float alpha);
	typedef bool(__thiscall* WorldToScreen)(DWORD* th, Vector3* vectorIn, Vector3* vectorOut);
	typedef int(__cdecl* fnPing)(char* a1, int a2, int a3);
	typedef bool(__cdecl* fnIsHero)(GameObject* pObj);
	typedef int(__thiscall* fnGetBaseDrawPosition)(GameObject* pObj, Vector3* position);
	typedef float(__thiscall* fnGetBoundingRadius)(GameObject* pObj);


	//typedef bool(__cdecl* fnIsMissile)(GameObject* pObj);
	typedef bool(__cdecl* fnIsMinion)(GameObject* pObj);
	typedef bool(__cdecl* fnIsTurret)(GameObject* pObj);
	//typedef bool(__cdecl* fnIsInhibitor)(GameObject* pObj);
	typedef bool(__cdecl* fnIsTroyEnt)(GameObject* pObj);
	//typedef bool(__cdecl* fnIsNexus)(GameObject* pObj);
	//typedef float(__cdecl* fnGetAttackCastDelay)(GameObject* pObj);
	//typedef float(__cdecl* fnGetAttackDelay)(GameObject* pObj);
	typedef bool(__thiscall* fnIsAlive)(GameObject* pObj);
	//typedef bool(__thiscall* fnIsTargetable)(GameObject* pObj);
	//typedef bool(__thiscall* fnGetPing)(GameObject* pObj);

	typedef GameObject* (__thiscall* fnGetFirstObject)(char* objManager);
	typedef GameObject* (__thiscall* fnGetNextObject)(char* objManager, GameObject* obj);
};

namespace Function {
	Vector3* GetMouseWorldPosition();
	bool IsChatOpen();
	Vector2 WorldToScreen(Vector3* in);
	bool World2Screen(Vector3* in, Vector3* out);
	bool IsAlive(GameObject* object);
	float GetBoundingRadius(GameObject* object);
	float GetAttackCastDelay(GameObject* object);
	float GetAttackDelay(GameObject* object);
	SpellData* GetBasicAttack(GameObject* unit);

	bool IsMinion(GameObject* Object);
	bool IsTurret(GameObject* Object);
	void BlockInput(bool block);
	void LockCamera(bool lock);

	void PrintChat(string text, PrintChatType color = PrintChatType::WithTime);
	void SendChat(string text, int color = 0xFFFFFF);

	bool IsWall(Vector3* position, unsigned __int16 uk = 0);
	bool IsWall(Vector3 position);

	int* IssueOrder(GameObject* Object, int Order, Vector3* Position, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown);
	int GetBaseDrawPosition(GameObject* Object, Vector3* Position);
	bool GetHPBarPosition(GameObject* Object, Vector3* out);
	int* CastSpell(DWORD* spellbook, DWORD* spelldatainst, int spellslot, Vector3* Origin, Vector3* Target, DWORD networkid);
	Vector2 WorldToMap(const Vector3& pos);
	int GetPing();
	float GameTime();

	float GameTimeTick();


	int GetSelected();
	void SetSelected(int IndexID);

	void CharacterDataStack_Push(void* hero, const char* model, std::int32_t skin) noexcept;

	void CharacterDataStack_Update(void* hero, bool change) noexcept;
}