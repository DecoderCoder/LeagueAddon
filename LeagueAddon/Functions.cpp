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

void MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2) {
	int size = row1 * col2;
	for (int i = 0; i < row1; i++) {
		for (int j = 0; j < col2; j++) {
			float sum = 0.f;
			for (int k = 0; k < col1; k++)
				sum = sum + a[i * col1 + k] * b[k * col2 + j];
			out[i * col2 + j] = sum;
		}
	}
}

Vector3 Function::WorldToScreen(Vector3* pos)
{
	float    viewMatrix[16];
	float    projMatrix[16];
	float    viewProjMatrix[16];

	{
		memcpy(viewMatrix, (void*)DEFINE_RVA(Offset::Data::ViewProjMatrices), 16 * sizeof(float));
		memcpy(projMatrix, (void*)DEFINE_RVA(Offset::Data::ViewProjMatrices + 0x40), 16 * sizeof(float));
		MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
	}


	Vector3 out = { 0.f, 0.f, 0.f };
	Vector2 screen = { (float)Render::RenderWidth, (float)Render::RenderHeight };
	static Vector4 clipCoords;
	clipCoords.x = pos->x * viewProjMatrix[0] + pos->y * viewProjMatrix[4] + pos->z * viewProjMatrix[8] + viewProjMatrix[12];
	clipCoords.y = pos->x * viewProjMatrix[1] + pos->y * viewProjMatrix[5] + pos->z * viewProjMatrix[9] + viewProjMatrix[13];
	clipCoords.z = pos->x * viewProjMatrix[2] + pos->y * viewProjMatrix[6] + pos->z * viewProjMatrix[10] + viewProjMatrix[14];
	clipCoords.w = pos->x * viewProjMatrix[3] + pos->y * viewProjMatrix[7] + pos->z * viewProjMatrix[11] + viewProjMatrix[15];

	if (clipCoords.w < 1.0f)
		clipCoords.w = 1.f;

	Vector3 M;
	M.x = clipCoords.x / clipCoords.w;
	M.y = clipCoords.y / clipCoords.w;
	M.z = clipCoords.z / clipCoords.w;

	out.x = (screen.x / 2.f * M.x) + (M.x + screen.x / 2.f);
	out.y = -(screen.y / 2.f * M.y) + (M.y + screen.y / 2.f);


	return out;
}

//Vector3 Function::WorldToScreen(Vector3* in)
//{
//	Vector3 out;
//	FuncType::WorldToScreen w2sFunc = (FuncType::WorldToScreen)DEFINE_RVA(Offset::Function::WorldToScreen);
//	DWORD* V4 = (DWORD*)((*(DWORD*)(DEFINE_RVA(Offset::Data::W2SInstance))) + Offset::Data::W2SInstanceOffset);
//
//	bool ret = w2sFunc(V4, in, &out);
//	//w2sFunc(in, &out);	
//	return out;
//}

DWORD Gadget = DEFINE_RVA(0x510D);

int Function::GetBaseDrawPosition(GameObject* Object, Vector3* Position) {
	DWORD BaseDrawPositionAddr = DEFINE_RVA(Offset::Function::GetBaseDrawPosition);//GetBaseDrawPosition

	__asm {
		//	mov esi, [hudInstance30]
		push retnHere
		push Position
		mov ecx, Object
		push Gadget
		jmp BaseDrawPositionAddr
		retnHere :
	}

}

void Function::NewCastSpell(int slot, int castType, float x, float y)
{
	if (x > 0 && x < Render::RenderWidth && y > 0 && y < Render::RenderHeight)
		Input::Move(x, y);
	if (Settings::Global::useNewCastSpell)
	{
		typedef void(__thiscall* fnnewcastspell)(DWORD hudinstance, int spellIndex, int castType, float a4);
		static fnnewcastspell CastSpell = (fnnewcastspell)(DEFINE_RVA(Offset::Function::NewCastSpell));

		DWORD HUDInputLogic = *(DWORD*)(*(DWORD*)DEFINE_RVA(Offset::Data::HudInstance) + 0x34);

		DWORD CastSpellAddr = DEFINE_RVA(Offset::Function::NewCastSpell);//IssueOrder	
		bool Detected;
		__asm {
			push retnHere
			push 0.0f
			push castType
			push slot
			//push HUDInputLogic			
			mov ecx, HUDInputLogic
			push Gadget
			jmp CastSpellAddr
			retnHere :
			//mov al, 1
		}

		//CastSpell(HUDInputLogic, slot, castType, 0.0f);
	}
	else {
		Input::PressKey(Settings::Binding::GetSpellSlotBinding(slot));
	}	
}

bool Function::GetHPBarPosition(GameObject* Object, Vector3* out) {
	//There is a problem with this call
	Vector3 drawPosition;
	Function::GetBaseDrawPosition(Object, &drawPosition);


	Vector3 screen;
	if (!Function::World2Screen(&drawPosition, &screen))
	{
		return false;
	}

	float delta = abs(drawPosition.y - Object->Position.y);
	delta = delta / 1920 * Render::RenderWidth;
	delta *= 1.1f;
	if (!Function::World2Screen(&drawPosition, out))
	{
		return false;
	}

	out->x -= 45;
	out->y -= delta;
	out->z = 0;

	//out->x = out->x / Render::RenderWidth * 1920;
	//out->y = out->y / Render::RenderHeight * 1080;
	return true;
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

	DWORD IssueOrderAddr = DEFINE_RVA(Offset::Function::IssueOrder);//IssueOrder	
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
//
//int* Function::CastSpell(DWORD* spellbook, DWORD* spelldatainst, int spellslot, Vector3* Origin, Vector3* Target, DWORD networkid) {
//	FuncType::fnCastSpell castSpell = (FuncType::fnCastSpell)(baseAddr + Offset::Function::NewCastSpell);
//	bool Detected;
//	auto res = castSpell(spellbook, spelldatainst, spellslot, Origin, Target, networkid);
//	Detected = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(get_peb()) + 0xA00);
//	if (Detected == true) {
//		MessageBoxA(0, "Detected", "PEB", 0);
//	}
//
//	return res;
//}



int Function::GetPing() {
	FuncType::fnGetPing getPingFunc = (FuncType::fnGetPing)(baseAddr + Offset::Function::GetPing);
	auto res = getPingFunc(*(char**)(baseAddr + Offset::Data::PingObject));
	return res;
}

void Function::SendPing(Vector3* Pos, int NetworkID, PingType pingType)
{
	typedef void(__thiscall* fnSendPing)(DWORD pThis, Vector2* worldPos, Vector2* screenPos, int NetworkID, int pingType);
	fnSendPing ping = (fnSendPing)DEFINE_RVA(Offset::Function::SendPing);
	DWORD pingInstance = *(DWORD*)(*(DWORD*)DEFINE_RVA(Offset::Data::HudInstance) + 0x8);
	Vector3 w2s = Function::WorldToScreen(Pos);

	Vector2 w2s2 = Vector2(w2s.x, w2s.z);
	Vector2 pos2 = Vector2(Pos->x, Pos->z);

	ping(pingInstance, &pos2, &w2s2, NetworkID, (int)pingType);
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
	bool* cameraLock = (bool*)(camera + 0x2A);
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

bool Function::IsWall(Vector3* position, unsigned __int16 uk)
{
	typedef bool(__cdecl* fnIsNotWall)(Vector3* position, unsigned __int16 uk);
	return !((fnIsNotWall)(DEFINE_RVA(Offset::Function::IsNotWall)))(position, (unsigned __int16)uk);
}

bool Function::IsWall(Vector3 position) {
	return IsWall(&position);
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
	Push(hero, model, skin, 0, true, false, false, false, true, false, -1, "\x00", 0, "\x00", 0, false, 1);
}

void Function::CharacterDataStack_Update(void* hero, bool change) noexcept
{
	static const auto Update{ reinterpret_cast<void(__thiscall*)(void*, bool)>(std::uintptr_t(DEFINE_RVA(Offset::Function::CharacterDataStack__Update))) };
	Update(hero, change);
}

void Function::PrintChat(string text, PrintChatType color) {
	// 0xFFFFFF - Log
	// 0   - Log without time

	FuncType::fnPrintChat printFunc = (FuncType::fnPrintChat)DEFINE_RVA(Offset::Function::PrintChat);
	printFunc(DEFINE_RVA(Offset::Data::ChatInstance), text.c_str(), (int)color);
}

void Function::SendChat(string text, int color) {
	// 0xFFFFFF - Log
	// 0   - Log without time

	FuncType::fnPrintChat printFunc = (FuncType::fnPrintChat)DEFINE_RVA(Offset::Function::SendChat);
	printFunc(DEFINE_RVA(Offset::Data::ChatInstance), text.c_str(), color);
}