#include "Hooks.h"
#include "DirectX.h"
#include "../EventManager/EventManager.h"
#include "../Structs.h"
#include "../Input.h"
#include "winternl.h"
#include "makesyscall.h"
//#include "minhook.h"
//
//#pragma comment(lib, "minhook.x32.lib")

typedef BOOL(__stdcall* fGetCursorPos) (LPPOINT lpPoint);
uint64_t oGetCursorPos = NULL;

typedef int(__thiscall* fOnProcessSpell)(void* spellBook, void* spellData);
fOnProcessSpell oOnProcessSpell = NULL;

PVOID new_on_process_spell;

bool OnProcessSpellHooked = false;


//PLH::CapstoneDisassembler dis(PLH::Mode::x86);
//std::unique_ptr<PLH::x86Detour> detourGCP; // GetCursorPos
//std::unique_ptr<PLH::x86Detour> detourQIP; // NtQueryInformationProcess 

int __fastcall hOnProcessSpell(void* spellBook, void* edx, SpellInfo* CastInfo)
{
	//MH_Initialize();
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
	//MessageBoxA(0, "NT_QUERY_INFO_PROCESS", "", 0);
	return 0;
}

//DWORD(WINAPI* hNtQueryInformationProcess)(/*IN */HANDLE ProcessHandle,
//	/*IN */PROCESSINFOCLASS ProcessInformationClass,
//	/*OUT */PVOID ProcessInformation,
//	/*IN */ULONG ProcessInformationLength,
//	/*OUT */PULONG ReturnLength /*OPTIONAL*/) {
//
//}

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

DWORD NtQueryProcessInformationPtr = 0;
DWORD hNtQueryProcessInformationPtr = 0;

LPVOID lpJmpRealloc = nullptr;
LPVOID heavensGateBackup = nullptr;

DWORD changedPtr = 0;
DWORD ret1, ret2;
DWORD Backup_Eax, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength;
void __declspec(naked) hk_NtQueryInformationProcess()
{
	//__asm {
	//	mov Backup_Eax, eax
	//}
	//__asm {
	//	mov eax, [esp + 0x0]
	//	mov ret2, [esp + 0x4]
	//}
	Utils::Log("ret1  " + to_hex((int)ret1));
	Utils::Log("ret2  " + to_hex((int)ret2));
	//__asm {
	//	mov ret1, esp + 0x0
	//	mov ret2, esp + 0x4
	//}
	//__asm {
	//	mov eax, Backup_Eax
	//}
	Utils::Log("ret11 " + to_hex((int)ret1));
	Utils::Log("ret22 " + to_hex((int)ret2));

	Utils::Log("hk_NtQueryInformationProcess 7");
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp + 0x8]
		mov ProcessHandle, eax
		mov eax, [esp + 0xC]
		mov ProcessInformationClass, eax
		mov eax, [esp + 0x10]
		mov ProcessInformation, eax
		mov eax, [esp + 0x14]
		mov ProcessInformationLength, eax
		mov eax, [esp + 0x18]
		mov ReturnLength, eax
		mov eax, Backup_Eax
		pushad
		mov eax, Backup_Eax
	}

	//Utils::Log("hk_NtQueryInformationProcess");
	//Utils::Log("ProcessHandle			: " + to_string(ProcessHandle));
	//Utils::Log("ProcessInformationClass : " + to_hex((int)ProcessInformationClass));

	if (ProcessInformationClass == 0x07) {
		Utils::Log("ProcessInformationClass 7");
		Utils::Log("ProcessInformation - " + to_string(*(int*)ProcessInformation));
		*(int*)ProcessInformation = 0;
	}
	if (ProcessInformationClass == 31) {
		Utils::Log("ProcessInformationClass 31");
		Utils::Log("ProcessInformation - " + to_string(*(int*)ProcessInformation));
		*(int*)ProcessInformation = 1;
	}
	if (ProcessInformationClass == 30) {
		Utils::Log("ProcessInformationClass 30");
		Utils::Log("ProcessInformation - " + to_string(*(int*)ProcessInformation));
		*(int*)ProcessInformation = 0;
	}
	//ProcessInformation = (DWORD)&changedPtr;
	__asm popad
	__asm jmp lpJmpRealloc
}
DWORD Backup_Eax2, SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength2;
typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
	BOOLEAN DebuggerEnabled;
	BOOLEAN DebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG ProcessId;
	UCHAR ObjectTypeNumber;
	UCHAR Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

void __declspec(naked) hk_NtQuerySystemInformation()
{

	__asm {
		mov Backup_Eax2, eax
		mov eax, [esp + 0x8]
		mov SystemInformationClass, eax
		mov eax, [esp + 0xC]
		mov SystemInformation, eax
		mov eax, [esp + 0x10]
		mov SystemInformationLength, eax
		mov eax, [esp + 0x14]
		mov ReturnLength2, eax
		mov eax, Backup_Eax2
		pushad
	}

	if (SystemInformationClass == 0x23) {
		Utils::Log("hk_NtQuerySystemInformation 0x23");
		Utils::Log("SystemInformationClass: " + to_string(SystemInformationClass));
		_SYSTEM_KERNEL_DEBUGGER_INFORMATION* sys;
		sys = (_SYSTEM_KERNEL_DEBUGGER_INFORMATION*)SystemInformation;
		Utils::Log("DebuggerEnabled   : " + string(sys->DebuggerEnabled ? "true" : "false"));
		Utils::Log("DebuggerNotPresent: " + string(sys->DebuggerNotPresent ? "true" : "false"));
		sys->DebuggerEnabled = false;
		sys->DebuggerNotPresent = true;
	}

	if (SystemInformationClass == 0x2C) {
		Utils::Log("hk_NtQuerySystemInformation 0x2C");
		//Utils::Log("SystemInformationClass: " + to_string(SystemInformationClass));
		_SYSTEM_HANDLE_INFORMATION* sys;
		sys = (_SYSTEM_HANDLE_INFORMATION*)SystemInformation;
		Utils::Log("sys->ProcessId   : " + to_string(sys->ProcessId));
		Utils::Log("sys->Handle      : " + to_string(sys->Handle));
		sys->Handle = 0;
		sys->ProcessId = 0;

	}


	//Utils::Log("ProcessHandle			: " + to_string(ProcessHandle));
	//Utils::Log("ProcessInformationClass : " + to_hex((int)ProcessInformationClass));

	//if (ProcessInformationClass == 7) {
	//	Utils::Log("hk_NtQueryInformationProcess 7 - ");
	//	//Utils::Log("ProcessInformation - " + to_string(*ProcessInformation));
	//	*ProcessInformation = 0;
	//}
	//if (ProcessInformationClass == 31) {
	//	Utils::Log("hk_NtQueryInformationProcess 31 - ");
	//	//Utils::Log("ProcessInformation - " + to_string(*ProcessInformation));
	//	*ProcessInformation = 1;
	//}
	//if (ProcessInformationClass == 30) {
	//	Utils::Log("hk_NtQueryInformationProcess 30 - ");
	//	//Utils::Log("ProcessInformation - " + to_string(*ProcessInformation));
	//	*ProcessInformation = 0;
	//}
	//ProcessInformation = &changedPtr;
	__asm popad
	__asm jmp lpJmpRealloc
}

const DWORD_PTR __declspec(naked) GetGateAddress()
{
	__asm
	{
		mov eax, dword ptr fs : [0xC0]
		ret
	}
}

void __declspec(naked) hk_Wow64Trampoline()
{
	__asm
	{
		cmp eax, 0x19 //64bit Syscall id of NtRVM
		je hk_NtQueryInformationProcess
		cmp eax, 0x36 //64bit Syscall id of NtRVM
		je hk_NtQuerySystemInformation
		jmp lpJmpRealloc
	}
}

const LPVOID CreateNewJump()
{
	DWORD_PTR Gate = GetGateAddress();
	lpJmpRealloc = VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	memcpy(lpJmpRealloc, (void*)Gate, 9);
	return lpJmpRealloc;
}

const void WriteJump(const DWORD_PTR dwWow64Address, const void* pBuffer, size_t ulSize)
{
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)dwWow64Address, 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	(void)memcpy((void*)dwWow64Address, pBuffer, ulSize);
	VirtualProtect((LPVOID)dwWow64Address, 0x1000, dwOldProtect, &dwOldProtect);
}


const void EnableWow64Redirect()
{
	LPVOID Hook_Gate = &hk_Wow64Trampoline;

	char trampolineBytes[] =
	{
		0x68, 0xDD, 0xCC, 0xBB, 0xAA,       /*push 0xAABBCCDD*/
		0xC3,                               /*ret*/
		0xCC, 0xCC, 0xCC                    /*padding*/
	};
	heavensGateBackup = VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	Utils::Log("heavensGateBackup : " + to_hex((int)heavensGateBackup));
	Utils::Log("heavensGate       : " + to_hex((int)GetGateAddress()));
	Utils::Log("lpJmpRealloc      : " + to_hex((int)lpJmpRealloc));
	Utils::Log("");
	//MessageBoxA(0, (to_hex((int)heavensGateBackup) + " " + to_hex((int)lpJmpRealloc)).c_str(), to_hex((int)GetGateAddress()).c_str(), 0);
	memcpy(heavensGateBackup, (LPVOID)GetGateAddress(), sizeof(trampolineBytes));
	memcpy(&trampolineBytes[1], &Hook_Gate, 4);
	WriteJump(GetGateAddress(), trampolineBytes, sizeof(trampolineBytes));
}

typedef BOOL(WINAPI* fnIsDebuggerPresent)();
uint64_t oIsDebuggerPresent = 0;
BOOL WINAPI hIsDebuggerPresent() {
	fnIsDebuggerPresent o = (fnIsDebuggerPresent)oIsDebuggerPresent;
	Utils::Log("IsDebuggerPresent called, orig value: " + string(o() ? "true" : "false"));
	return false;
}

int __fastcall  hGetPing(void* This, void* edx) {

	return 90;
}

uint64_t oGetPing = 0;
PVOID new_oGetPing;

void Hooks::ApplyHooks() {
	if (GetSystemDEPPolicy() != DEP_SYSTEM_POLICY_TYPE::DEPPolicyAlwaysOff)
		SetProcessDEPPolicy(PROCESS_DEP_ENABLE);

	Hooks::DirectX::HookDX();
	//Hooks::DirectX::HookInput();

	void* targetGCP = &GetCursorPos;
	/*detourGCP.reset(new PLH::x86Detour((char*)targetGCP, (char*)&hGetCursorPos, &Input::oGetCursorPos, dis));
	detourGCP->hook();*/
	//MessageBoxA(0, "before", ("ddd: " + to_hex(DEFINE_RVA(Offset::Function::GetPing))).c_str(), 0);
	//UltHook.DEPAddHook(DEFINE_RVA(Offset::Function::GetPing), reinterpret_cast<DWORD>(hGetPing), oGetPing, 0x59, new_oGetPing, 1);
	//ddd.reset(new PLH::x86Detour((char*)DEFINE_RVA(Offset::Function::GetPing), (char*)&hGetPing, &oGetPing, dis));
	//ddd->hook();
	//MessageBoxA(0, "after", "ddd", 0);
	if (GetSystemDEPPolicy() != DEP_SYSTEM_POLICY_TYPE::DEPPolicyAlwaysOff) {
		UltHook.RestoreRtlAddVectoredExceptionHandler();
		//UltHook.RestoreNtProtectVirtualMemory();
		//UltHook.RestoreZwQueryInformationProcess();
		OnProcessSpellHooked = UltHook.DEPAddHook(DEFINE_RVA(Offset::Function::OnProcessSpell), reinterpret_cast<DWORD>(hOnProcessSpell), oOnProcessSpell, 0x59, new_on_process_spell, 1);
		//UltHook.RestoreSysDll("ntdll.dll");
		//auto ntdll = GetModuleHandleA("ntdll.dll");
		//auto ntq = GetProcAddress(ntdll, "NtQueryInformationProcess");

		//void* target = &IsDebuggerPresent;

		//ddd.reset(new PLH::x86Detour((char*)target, (char*)&hIsDebuggerPresent, &oIsDebuggerPresent, dis));
		//ddd->hook();


		//CreateNewJump();
		//EnableWow64Redirect();

		//MessageBoxA(0, to_hex(c).c_str(), "_NtQueryInformationProcess", 0);

		//
		//MessageBoxA(0, UltHook.RestoreSysDll("kernel32.dll") ? "Good" : "Bad", "kernel32", 0);
		//

		//	
	}

	Hooked = true;
}

void Hooks::RemoveHooks() {
	//Utils::Log("ddd->unHook: " + string(ddd->unHook() ? "Ok" : "Error"));
	//Utils::Log("detourGCP->unHook: " + string(detourGCP->unHook() ? "Ok" : "Error"));
	Utils::Log("Hooks::DirectX::UnHookDX: " + string(Hooks::DirectX::UnHookDX() ? "Ok" : "Error"));
	Utils::Log("UltHook.deinit: " + string(UltHook.deinit() ? "Ok" : "Error"));
	Hooked = false;
	DWORD dwOldProtect = 0;
	//VirtualProtect((LPVOID)GetGateAddress(), 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	//memcpy((LPVOID)GetGateAddress(), heavensGateBackup, 9);
	//VirtualProtect((LPVOID)GetGateAddress(), 0x1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
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