#include "Hooks.h"
#include "DirectX.h"
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Input.h"
#include "winternl.h"
#include "makesyscall.h"

typedef BOOL(__stdcall* fGetCursorPos) (LPPOINT lpPoint);

typedef int(__thiscall* fOnProcessSpell)(void* spellBook, void* spellData);
fOnProcessSpell oOnProcessSpell = NULL;

PVOID new_on_process_spell;

bool OnProcessSpellHooked = false;

int __fastcall hOnProcessSpell(void* spellBook, void* edx, SpellInfo* CastInfo)
{
	if (spellBook == nullptr || CastInfo == nullptr)
		return oOnProcessSpell(spellBook, CastInfo);
	EventManager::Trigger(EventManager::EventType::OnProcessSpell, spellBook, CastInfo);
	return oOnProcessSpell(spellBook, CastInfo);
}


uint64_t Input::oGetCursorPos = NULL;
//
typedef NTSTATUS(*_NtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
NTSTATUS hNtQueryInformationProcess(IN HANDLE ProcessHandle, IN PROCESSINFOCLASS  ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength) {
	Utils::Log("NT_QUERY_INFO_PROCESS");
	return 0;
}

BOOL WINAPI hGetCursorPos(LPPOINT lpPoint)
{
	auto org = ((fGetCursorPos)Input::oGetCursorPos)(lpPoint);
	if (Input::mMouseX > 0 && Input::mMouseY > 0 && Input::mMouseX < Render::RenderWidth && Input::mMouseY < Render::RenderHeight) {
		lpPoint->x = Input::mMouseX;
		lpPoint->y = Input::mMouseY;
		Input::mMouseX = -1;
		Input::mMouseY = -1;
	}

	return org;
}

PVOID leoHandler;

int __fastcall  hGetPing(void* This, void* edx) {

	return 90;
}

uint64_t oGetPing = 0;
PVOID new_oGetPing;

void Hooks::ApplyHooks() {
	if (GetSystemDEPPolicy() != DEP_SYSTEM_POLICY_TYPE::DEPPolicyAlwaysOff)
		SetProcessDEPPolicy(PROCESS_DEP_ENABLE);


	//Hooks::DirectX::HookInput();

	DWORD targetGCP = (DWORD)&GetCursorPos;

	//MessageBoxA(0, to_hex(targetGCP).c_str(), "&GetCursorPos", 0);

	if (*(BYTE*)&GetCursorPos == 0xE9) {
		targetGCP = ((DWORD)targetGCP + *(DWORD*)((DWORD)targetGCP + 1)) + 5;
		//MessageBoxA(0, to_hex(targetGCP).c_str(), "&GetCursorPos", 0);
		Input::oGetCursorPos = UltHook.AddEzHook(targetGCP, 6, (DWORD)&hGetCursorPos);
	}
	else
		Input::oGetCursorPos = UltHook.AddEzHook((DWORD)&GetCursorPos, 5, (DWORD)&hGetCursorPos);
	if (GetSystemDEPPolicy() != DEP_SYSTEM_POLICY_TYPE::DEPPolicyAlwaysOff) {
		UltHook.RestoreRtlAddVectoredExceptionHandler();
		//UltHook.RestoreNtProtectVirtualMemory();
		//UltHook.RestoreZwQueryInformationProcess();
		OnProcessSpellHooked = UltHook.DEPAddHook(DEFINE_RVA(Offset::Function::OnProcessSpell), reinterpret_cast<DWORD>(hOnProcessSpell), oOnProcessSpell, 0x59, new_on_process_spell, 1);
		Hooks::DirectX::HookDX();
		//UltHook.RestoreSysDll("ntdll.dll");
	}

	Hooked = true;
}

void Hooks::RemoveHooks() {
	if (Input::oGetCursorPos)
		UltHook.RemoveEzHook(Input::oGetCursorPos);

	Utils::Log("Hooks::DirectX::UnHookDX: " + string(Hooks::DirectX::UnHookDX() ? "Ok" : "Error"));
	Utils::Log("UltHook.deinit: " + string(UltHook.deinit() ? "Ok" : "Error"));
	Hooked = false;
	DWORD dwOldProtect = 0;
}

void Hooks::DirectX::HookDX() {
	if (DirectXHook::SupportDX11()) {
		DirectXHook::HookDX11();
	}
	if (DirectXHook::SupportDX9()) {
		DirectXHook::HookDX9();
	}
}

void Hooks::DirectX::HookInput()
{
	DirectXHook::HookInput();
}


bool Hooks::DirectX::UnHookDX() {
	return DirectXHook::unHook();
}