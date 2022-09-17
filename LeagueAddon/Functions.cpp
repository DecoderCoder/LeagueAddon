#include "Functions.h"

//int Function::GetPing() {
//	FuncType::fnGetPing getPingFunc = (FuncType::fnGetPing)(baseAddr + Offset::GetPing);
//	auto res = getPingFunc(*(char**)(baseAddr + Offset::PingObject));
//	return res;
//}

__declspec(naked) void* __cdecl get_peb() {
	__asm {
		mov eax, fs:0x18
		mov eax, [eax + 0x30]
		retn
	}
}

//Vector3* Function::GetMouseWorldPosition()
//{
//	Vector3* pos = (Vector3*)(*(DWORD*)(baseAddr + Offset::Data::HudInstance) + 0x11C);
//	if (pos->x < 0 || pos->x > 15000 ) {
//		pos = (Vector3*)(*(DWORD*)(baseAddr + Offset::Data::HudInstance) + 0x19C);
//		MessageBoxA(0, to_hex((int)pos).c_str(), to_hex(pos->x).c_str(), 0);
//	}
//
//	if (pos->x == 0)
//		pos = (Vector3*)(*(DWORD*)(baseAddr + Offset::Data::HudInstance) + 0x31C);
//	return pos;
//}

Vector3* Function::GetMouseWorldPosition()
{
	DWORD MousePtr = DEFINE_RVA(Offset::Data::HudInstance);
	auto aux1 = *(DWORD*)MousePtr;
	aux1 += 0x14;
	auto aux2 = *(DWORD*)aux1;
	aux2 += 0x1C;

	Vector3* temp = (Vector3*)aux2;

	/*temp.x = *(float*)(aux2 + 0x0);
	temp.y = *(float*)(aux2 + 0x4);
	temp.z = *(float*)(aux2 + 0x8);*/

	return temp;
}

bool Function::IsChatOpen() {
	return *(bool*)(*(int*)(baseAddr + Offset::Data::ChatInstance) + Offset::Data::IsChatOpenOffset);
}



bool Function::World2Screen(Vector3* in, Vector3* out) {
	FuncType::WorldToScreen w2sFunc = (FuncType::WorldToScreen)DEFINE_RVA(Offset::Function::WorldToScreen);
	DWORD* V4 = (DWORD*)((*(DWORD*)(DEFINE_RVA(Offset::Data::W2SInstance))) + Offset::Data::W2SInstanceOffset);

	bool ret = w2sFunc(V4, in, out);
	/*if (out->y < -GetSystemMetrics(SM_CYSCREEN)) {
		out->Negate();
	}*/
	return ret;
}

Vector2 Function::WorldToScreen(Vector3* in)
{
	FuncType::WorldToScreen w2sFunc = (FuncType::WorldToScreen)DEFINE_RVA(Offset::Function::WorldToScreen);
	Vector3 out;
	//w2sFunc(in, &out);
	Vector2 out2 = Vector2(out.x, out.y);
	delete out;
	return out2;
}

int* Function::IssueOrder(GameObject* Object, int Order, Vector3* Position, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown) {
	// Move - 0x13 | Order - 2
	// Attack - 0x14 | Attack - 3
	//char* hudInstance30 = *(char**)(*(char**)(baseAddr + Offset::HudInstance) + 0x30);
	char* antiCheatCheck = (char*)baseAddr + Offset::Data::AntiCheatCheck;

	switch (Order) {
	case 2:
		*antiCheatCheck = 0x13;
		break;
	case 3:
		*antiCheatCheck = 0x14;
		break;
	default:
		return 0;
		break;
	}
	memset(antiCheatCheck + 0x1, 0, 0x3);

	DWORD IssueOrderAddr = baseAddr + Offset::Function::IssueOrder;//IssueOrder
	DWORD Gadget = baseAddr + 0x510D;
	bool Detected;
	__asm {
		//	mov esi, [hudInstance30]
		push retnHere
		push Unknown
		push IsMinion
		push IsAttackMove
		push Target
		push Position
		push Order
		mov ecx, Object
		push Gadget
		jmp IssueOrderAddr
		retnHere :
		mov al, 1
	}
	Detected = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(get_peb()) + 0xA00);
	if (Detected == true) {
		Utils::Log("PEB DETECTED !!! RISK OF BAN");
		__asm {
			jmp antiCheatCheck
		}

	}
	switch (Order) {
	case 2:
		*antiCheatCheck = 0x13;
		break;
	case 3:
		*antiCheatCheck = 0x14;
		break;
	}
	memset(antiCheatCheck + 0x1, 0, 0x3);
	return NULL;
}

int* Function::CastSpell(DWORD* spellbook, DWORD* spelldatainst, int spellslot, Vector3* Origin, Vector3* Target, DWORD networkid) {
	FuncType::fnCastSpell castSpell = (FuncType::fnCastSpell)(baseAddr + Offset::Function::CastSpell);
	bool Detected;
	auto res = castSpell(spellbook, spelldatainst, spellslot, Origin, Target, networkid);
	Detected = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(get_peb()) + 0xA00);
	if (Detected == true) {
		MessageBoxA(0, "Detected", "PEB", 0);
	}

	return res;
}



int Function::GetPing() {
	FuncType::fnGetPing getPingFunc = (FuncType::fnGetPing)(baseAddr + Offset::Function::GetPing);
	auto res = getPingFunc(*(char**)(baseAddr + Offset::Data::PingObject));
	return res;
}

Vector2 Function::WorldToMap(const Vector3& pos) {
	const DWORD MinimapObjectHud = *(DWORD*)((*(DWORD*)(baseAddr + Offset::Minimap::Object)) + Offset::Minimap::ObjectHud);
	const Vector2 worldSize = *(Vector2*)(MinimapObjectHud + Offset::Minimap::HudWorldSize);
	const Vector2 wPos = *(Vector2*)(MinimapObjectHud + Offset::Minimap::HudPos);
	const Vector2 wSize = *(Vector2*)(MinimapObjectHud + Offset::Minimap::HudSize);
	Vector2 result = { pos.x / worldSize.x, pos.z / worldSize.y };
	result.x = wPos.x + result.x * wSize.x;
	result.y = wPos.y + wSize.y - (result.y * wSize.y);

	return result;
}

float Function::GameTime()
{
	return *(float*)(DEFINE_RVA(Offset::Data::GameTime));
}

float Function::GameTimeTick()
{
	return *(float*)(DEFINE_RVA(Offset::Data::GameTime)) * 1000;
}

bool Function::IsAlive(GameObject* object) {
	//return !(*(bool(__thiscall**)(DWORD*))(*((DWORD*)object + 153) + 8))((DWORD*)object + 153);
	return !reinterpret_cast<encrypted_buffer<bool>*>((DWORD)object + Offset::GameObject::Dead)->decrypt();
	//return object->IsDead->decrypt();
	//return ((FuncType::fnIsAlive)(DEFINE_RVA(Offset::Function::IsAlive)))(object) && object->Health > 0.0f;
}

float Function::GetBoundingRadius(GameObject* object) {
	return ((FuncType::fnGetBoundingRadius)(DEFINE_RVA(Offset::Function::GetBoundingRadius)))(object);
}

float Function::GetAttackCastDelay(GameObject* object) {
	return ((FuncType::fnGetAttackCastDelay)(DEFINE_RVA(Offset::Function::GetAttackCastDelay)))(object);
}

float Function::GetAttackDelay(GameObject* object) {
	return ((FuncType::fnGetAttackDelay)(DEFINE_RVA(Offset::Function::GetAttackDelay)))(object);
}

SpellData* Function::GetBasicAttack(GameObject* unit)
{
	typedef SpellData** (__thiscall* fnGetBasicAttack)(GameObject* target, unsigned slot);
	auto ret = reinterpret_cast<fnGetBasicAttack>(DEFINE_RVA(Offset::Function::GetBasicAttack)
		)(unit, 0x41);
	if (ret) {
		return *ret;
	}
	return nullptr;
}

void Function::BlockInput(bool block) {
	DWORD hudInstance = *(DWORD*)DEFINE_RVA(Offset::Data::HudInstance);
	DWORD inputLogic = *(DWORD*)(hudInstance + 0x24);
	DWORD aux = *(DWORD*)inputLogic;

	int* blockInput = (int*)(aux + 0x18);
	*blockInput = block ? 510 : 511;
}

void Function::LockCamera(bool lock) {
	DWORD camera = *(DWORD*)DEFINE_RVA(Offset::Data::Camera);
	bool *cameraLock = (bool*)(camera + 0x2A);
	*cameraLock = lock;
}

bool Function::IsMinion(GameObject* Object)
{
	typedef bool(__cdecl* fnMinion)(GameObject* pObj);
	return ((fnMinion)(DEFINE_RVA(Offset::Function::IsMinion)))(Object);
}

bool Function::IsTurret(GameObject* Object)
{
	typedef bool(__cdecl* fnIsTurret)(GameObject* pObj);
	return ((fnIsTurret)(DEFINE_RVA(Offset::Function::IsTurret)))(Object);
}

bool Function::IsWall(Vector3* position)
{
	typedef bool(__cdecl* fnIsNotWall)(Vector3* position, unsigned __int16 uk);
	return !((fnIsNotWall)(DEFINE_RVA(Offset::Function::IsNotWall)))(position, (unsigned __int16)0);
}

int Function::GetSelected() {
	auto hudmgr = *(DWORD*)DEFINE_RVA(Offset::Data::HudInstance);
	auto hud = *(DWORD*)(hudmgr + 0x30);
	int retaddr = *(int*)(hud + 0x18);
	return retaddr;
}

void Function::SetSelected(int SetSelected) {
	auto hudmgr = *(DWORD*)DEFINE_RVA(Offset::Data::HudInstance);
	auto hud = *(DWORD*)(hudmgr + 0x30);
	*(int*)(hud + 0x18) = SetSelected;
}

void Function::CharacterDataStack_Push(void* hero, const char* model, std::int32_t skin) noexcept
{
	static const auto Push{ reinterpret_cast<int(__thiscall*)(void*, const char* model, std::int32_t skinid, std::int32_t, bool update_spells, bool dont_update_hud, bool, bool, bool change_particle, bool, char, const char*, std::int32_t, const char*, std::int32_t, bool, std::int32_t)>(std::uintptr_t(::GetModuleHandle(nullptr)) + Offset::Function::CharacterDataStack__Push) };
	Push(hero, model, skin, 0, false, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
}

void Function::CharacterDataStack_Update(void* hero, bool change) noexcept
{
	static const auto Update{ reinterpret_cast<void(__thiscall*)(void*, bool)>(std::uintptr_t(DEFINE_RVA(Offset::Function::CharacterDataStack__Update))) };
	Update(hero, change);
}