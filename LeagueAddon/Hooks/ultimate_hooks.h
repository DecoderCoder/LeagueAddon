#pragma once
#include <Windows.h>
#include <vector>
#include <VersionHelpers.h>
#include "../Utils.h"

#pragma warning(4:4596)

#define VP_Offset_old (0x170)
#define VP_Offset (0x180)

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

struct HookDetails {
	DWORD hookAddress;
	DWORD addressToHook;
};

struct HookEntries
{
	std::vector<HookDetails> hookDetails;
	DWORD addressToHookOldProtect;
	DWORD addressToHookMbiStart;
	DWORD addressToHookMbiEnd;
	DWORD addressToHookMbiSize;
	DWORD allocatedAddressStart;
	DWORD allocatedAddressEnd;
	DWORD allocatedAddressSize;
	DWORD addressToHookoffsetFromStart;
};

struct EzHook {
	bool hooked;
	bool JMP;
	DWORD address;
	size_t hookSize;

	DWORD hookFunction;
	DWORD origFunc;
};

LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);

class UltimateHooks {
public:
	template <class fnType>
	bool DEPAddHook(DWORD Address, DWORD hk_Address, fnType& OldAddress, size_t Size, PVOID& Allocation, uint8_t Offset) {
		Utils::Log("> DEPAddHook");
		Utils::Log("> DEPAddHook: VAllocate");
		DWORD NewOnprocessSpellAddr = VirtualAllocateRegion(Allocation, Address, Size);
		Utils::Log("> DEPAddHook: VAllocate: Ok");
		Utils::Log("> DEPAddHook: FFAllocate");
		CopyRegion((DWORD)Allocation, Address, Size);
		Utils::Log("> DEPAddHook: FFAllocate: CopyRegion: Ok");		
		FixFuncRellocation(Address, (Address + Size), (DWORD)Allocation, Size);
		Utils::Log("> DEPAddHook: FFAllocate: Ok");
		OldAddress = (fnType)(NewOnprocessSpellAddr);
		auto res = addHook(Address, (DWORD)hk_Address, Offset);
		Utils::Log("> DEPAddHook: Ok");
		//MessageBoxA(0, to_hex((int)Allocation).c_str(), to_hex((int)Address).c_str(), 0);
		return res;
	}
	bool deinit();
	DWORD RestoreRtlAddVectoredExceptionHandler();
	DWORD RestoreZwQueryInformationProcess();
	DWORD RestoreNtProtectVirtualMemory();
	bool RestoreSysDll(string name);
	
	DWORD AddEzHook(DWORD target, size_t hookSize, DWORD hook);
	DWORD RemoveEzHook(DWORD target);

	ULONG ProtectVirtualMemory(PVOID addr, PSIZE_T size, ULONG protect, PULONG old);
private:
	bool IsDoneInit = false;
	PVOID VEH_Handle = nullptr;

	DWORD VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	void CopyRegion(DWORD dest, DWORD source, size_t size);
	void FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size);
	bool addHook(DWORD address, DWORD hkAddress, size_t offset);
	bool Hook(DWORD original_fun, DWORD hooked_fun, size_t offset);
	void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset);
};
extern UltimateHooks UltHook;