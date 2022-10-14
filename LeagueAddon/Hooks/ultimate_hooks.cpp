#define USE_ZIDYS 0
#define DEBUG_RELLOCATION 0

#include "ultimate_hooks.h"
#include "makesyscall.h"
#include <thread>
#include "../Utils.h"
#include "psapi.h"
#include "winternl.h"
#if USE_ZIDYS || DEBUG_RELLOCATION
#include "Zydis/Zydis.h"
#endif
std::vector<HookEntries> hookEntries;
UltimateHooks UltHook;
DWORD RtlInterlockedCompareExchange64Offst;
std::vector<EzHook> EzHooks;

//относительная кодировка E9 jmp используется следующим образом :
//
//CURRENT_RVA: jmp(DESTINATION_RVA - CURRENT_RVA - 5[sizeof(E9 xx xx xx xx)])


DWORD UltimateHooks::AddEzHook(DWORD target, size_t hookSize, DWORD hook) {
	if (hookSize > 1024 || hookSize < 5)
		return -1;
	DWORD oldProt;

	EzHook ezHook;
	ezHook.hooked = false;
	ezHook.hookSize = hookSize;
	ezHook.hookFunction = hook;

	//if (*(BYTE*)(target) == 0xE9) {
	//	target = target + *(DWORD*)(target + 1) + 5;
	//	ezHook.JMP = true;
	//	hookSize = 6;
	//}

	DWORD allocation = (DWORD)VirtualAlloc(NULL, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocation == 0)
		return -1;

	ezHook.origFunc = allocation;

	VirtualProtect((void*)target, hookSize, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy((void*)allocation, (void*)target, hookSize);

	*(BYTE*)(allocation + hookSize) = 0xE9;
	*(DWORD*)(allocation + hookSize + 0x1) = (target + hookSize) - (allocation + hookSize) - 5; // if jmp is 5 byte size

	*(BYTE*)(target) = 0xE9;
	*(DWORD*)(target + 0x1) = hook - target - 5;

	ezHook.hooked = true;


	ezHook.address = target;
	EzHooks.push_back(ezHook);
	//MessageBoxA(0, to_hex((int)allocation).c_str(), to_hex((int)hook).c_str(), 0);
	VirtualProtect((void*)target, hookSize, oldProt, &oldProt);
	return allocation;

}

DWORD UltimateHooks::RemoveEzHook(DWORD origFunction) {
	for (auto obj : EzHooks) {
		if (obj.origFunc == origFunction) {
			DWORD oldProt;
			VirtualProtect((void*)obj.address, obj.hookSize, PAGE_EXECUTE_READWRITE, &oldProt);
			memcpy((void*)obj.address, (void*)obj.origFunc, obj.hookSize);
			VirtualProtect((void*)obj.address, obj.hookSize, oldProt, &oldProt);
			return true;
		}
	}
	return false;
}

bool inRange(unsigned low, unsigned high, unsigned x)
{
	return  ((x - low) <= (high - low));
}

bool memory_readable(void* ptr, size_t byteCount)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		return false;

	if (mbi.State != MEM_COMMIT)
		return false;

	if (mbi.Protect == PAGE_NOACCESS || mbi.Protect == PAGE_EXECUTE)
		return false;

	// This checks that the start of memory block is in the same "region" as the
	// end. If it isn't you "simplify" the problem into checking that the rest of 
	// the memory is readable.
	size_t blockOffset = (size_t)((char*)ptr - (char*)mbi.AllocationBase);
	size_t blockBytesPostPtr = mbi.RegionSize - blockOffset;

	if (blockBytesPostPtr < byteCount)
		return memory_readable((char*)ptr + blockBytesPostPtr,
			byteCount - blockBytesPostPtr);

	return true;
}

LONG __stdcall LeoHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && pExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8)
	{
		for (HookEntries hs : hookEntries)
		{
			for (HookDetails hd : hs.hookDetails)
			{
				if (hd.addressToHook == pExceptionInfo->ContextRecord->Eip) {
					pExceptionInfo->ContextRecord->Eip = hd.hookAddress;
					return EXCEPTION_CONTINUE_EXECUTION;
				}
			}
			if (inRange(hs.addressToHookMbiStart - 0x1000, hs.addressToHookMbiEnd + 0x1000, pExceptionInfo->ContextRecord->Eip)) {
				int offset = pExceptionInfo->ContextRecord->Eip - hs.addressToHookMbiStart;
				pExceptionInfo->ContextRecord->Eip = static_cast<DWORD>(hs.allocatedAddressStart + offset);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

bool UltimateHooks::deinit()
{
	DWORD old;
	if (VEH_Handle)
	{
		if (RemoveVectoredExceptionHandler(VEH_Handle))
		{

			for (HookEntries hs : hookEntries)
			{
				for (HookDetails hd : hs.hookDetails)
				{
					auto addr = (PVOID)hd.addressToHook;
					auto size = static_cast<SIZE_T>(static_cast<int>(1));
					if (NT_SUCCESS(
						makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", RtlInterlockedCompareExchange64Offst, 0xC2, 0x14,
							0x00)(GetCurrentProcess(), &addr, &size, hs.addressToHookOldProtect, &old)))
					{
					}
				}
			}
			hookEntries.clear();
			return true;
		}
	}

	return false;
}

ULONG UltimateHooks::ProtectVirtualMemory(PVOID addr, PSIZE_T size, ULONG protect, PULONG old) {
	typedef ULONG(WINAPI* TNtProtectVirtualMemory) (HANDLE, PVOID, PSIZE_T, ULONG, PULONG);
	TNtProtectVirtualMemory xNtProtectVirtualMemory;
	xNtProtectVirtualMemory = (TNtProtectVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtProtectVirtualMemory");
	HMODULE jo = GetModuleHandleA("ntdll.dll");
	return xNtProtectVirtualMemory(GetCurrentProcess(), addr, size, protect, old);
}

DWORD UltimateHooks::RestoreNtProtectVirtualMemory() {

	Utils::Log("> RestoreNtProtectVirtualMemory");
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD NtProtectVirtualMemoryAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "NtProtectVirtualMemory"));
	Utils::Log("> RestoreNtProtectVirtualMemory: NtProtectVirtualMemoryAddr");
	BYTE ZwQIP[] = {
		0xB8, 0x50, 0x00, 0x00, 0x00
	};

	//memcpy((void*)NtProtectVirtualMemoryAddr, &ZwQIP, sizeof(ZwQIP));

	int i = 0;
	DWORD old = 0;
	Utils::Log("> RestoreNtProtectVirtualMemory: NtProtectVirtualMemoryAddr: " + to_hex(NtProtectVirtualMemoryAddr));
	VirtualProtect((LPVOID)NtProtectVirtualMemoryAddr, sizeof(ZwQIP), 0x40, &old);
	Utils::Log("> RestoreNtProtectVirtualMemory: Old protect: " + to_hex(old));
	Utils::Log("> RestoreNtProtectVirtualMemory: ZwQIP start");
	for (BYTE _byte : ZwQIP) {
		Utils::Log("> RestoreNtProtectVirtualMemory: new byte: " + to_hex(_byte));
		Utils::Log("> RestoreNtProtectVirtualMemory: old byte: " + to_hex(*(BYTE*)(NtProtectVirtualMemoryAddr + i)));
		*(BYTE*)(NtProtectVirtualMemoryAddr + i) = _byte;
		i++;
		Utils::Log("> RestoreNtProtectVirtualMemory: " + to_string(i));
	}
	VirtualProtect((LPVOID)NtProtectVirtualMemoryAddr, sizeof(ZwQIP), old, &old);
	Utils::Log("> RestoreNtProtectVirtualMemory: Ok");
	return NtProtectVirtualMemoryAddr;
}

bool UltimateHooks::RestoreSysDll(string name)
{
	bool restored = false;
	HANDLE process = GetCurrentProcess();
	MODULEINFO mi = {};
	HMODULE ntdllModule = GetModuleHandleA(name.c_str());

	GetModuleInformation(process, ntdllModule, &mi, sizeof(mi));
	LPVOID ntdllBase = (LPVOID)mi.lpBaseOfDll;
	HANDLE ntdllFile = CreateFileA(("c:\\windows\\system32\\" + name).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	HANDLE ntdllMapping = CreateFileMapping(ntdllFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
	LPVOID ntdllMappingAddress = MapViewOfFile(ntdllMapping, FILE_MAP_READ, 0, 0, 0);

	PIMAGE_DOS_HEADER hookedDosHeader = (PIMAGE_DOS_HEADER)ntdllBase;
	PIMAGE_NT_HEADERS hookedNtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)ntdllBase + hookedDosHeader->e_lfanew);

	for (WORD i = 0; i < hookedNtHeader->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER hookedSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)IMAGE_FIRST_SECTION(hookedNtHeader) + ((DWORD_PTR)IMAGE_SIZEOF_SECTION_HEADER * i));

		if (!strcmp((char*)hookedSectionHeader->Name, (char*)".text")) {
			DWORD oldProtection = 0;
			bool isProtected = VirtualProtect((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProtection);
			memcpy((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), (LPVOID)((DWORD_PTR)ntdllMappingAddress + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize);
			isProtected = VirtualProtect((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize, oldProtection, &oldProtection);
			restored = true;
		}
	}

	CloseHandle(process);
	CloseHandle(ntdllFile);
	CloseHandle(ntdllMapping);
	FreeLibrary(ntdllModule);
	return restored;
}

DWORD UltimateHooks::RestoreRtlAddVectoredExceptionHandler() {
	Utils::Log("> RestoreRtlAddVectoredExceptionHandler");
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD RtlAddVectoredExceptionHandlerAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler"));

	DWORD RtlInterlockedCompareExchange64Addr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "RtlInterlockedCompareExchange64"));

	if (*(BYTE*)(RtlInterlockedCompareExchange64Addr + VP_Offset) == (BYTE)0xFF && *(BYTE*)(RtlInterlockedCompareExchange64Addr + VP_Offset + 0x1) == 0x25)
		RtlInterlockedCompareExchange64Offst = VP_Offset;
	else
		RtlInterlockedCompareExchange64Offst = VP_Offset_old;

	BYTE RtlAVE[] = {
		0x8B, 0xFF, 0x55, 0x8B, 0xEC
	};

	DWORD oldProt;
	auto addr = (PVOID)RtlAddVectoredExceptionHandlerAddr;
	auto size = static_cast<SIZE_T>(5);
	Utils::Log("> RestoreRtlAddVectoredExceptionHandler: RtlInterlockedCompareExchange64 + " + to_hex(RtlInterlockedCompareExchange64Offst));
	/*(DWORD)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), lpFuncName)*/
	auto A = makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", RtlInterlockedCompareExchange64Offst, 0xC2, 0x14, 0x00);
	auto B = A(GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &oldProt);
	Utils::Log("> RestoreRtlAddVectoredExceptionHandler: RtlInterlockedCompareExchange64 + " + to_hex(RtlInterlockedCompareExchange64Offst) + ": Ok");
	if (B >= 0)
	{
		int i = 0;
		for (BYTE _byte : RtlAVE) {
			*(BYTE*)(RtlAddVectoredExceptionHandlerAddr + i) = _byte;
			i++;
		}
		NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", RtlInterlockedCompareExchange64Offst, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, oldProt, &oldProt));
	}
	Utils::Log("> RestoreRtlAddVectoredExceptionHandler: Ok");
	return RtlAddVectoredExceptionHandlerAddr;
}
DWORD UltimateHooks::RestoreZwQueryInformationProcess() {
	Utils::Log("> RestoreZwQueryInformationProcess");
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwQueryInformationProcess"));
	Utils::Log("> RestoreZwQueryInformationProcess: ZwQueryInformationProcessAddr");
	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00
	};
	//*((uint64_t*)&ZwQIP[1]) = (uint64_t)&hNtQueryProcessInformation;
	int i = 0;
	DWORD old = -1;
	DWORD oldest = -1;
	ULONG ret = -1;
	Utils::Log("> RestoreZwQueryInformationProcess: ZwQueryInformationProcessAddr: " + to_hex(ZwQueryInformationProcessAddr));
	ret = VirtualProtect((LPVOID)ZwQueryInformationProcessAddr, sizeof(ZwQIP), 0x40, &old);
	oldest = old;
	if (ret != 0) {
		memcpy((void*)ZwQueryInformationProcessAddr, &ZwQIP, sizeof(ZwQIP));
		Utils::Log("> RestoreZwQueryInformationProcess: Old protect: " + to_hex(old));
		Utils::Log("> RestoreZwQueryInformationProcess: ZwQIP start");
		for (BYTE _byte : ZwQIP) {
			VirtualProtect((LPVOID)ZwQueryInformationProcessAddr, sizeof(ZwQIP), 0x40, &old);
			Utils::Log("> RestoreZwQueryInformationProcess: Old protect: " + to_hex(old));
			Utils::Log("> RestoreZwQueryInformationProcess: new byte: " + to_hex(_byte));
			Utils::Log("> RestoreZwQueryInformationProcess: old byte: " + to_hex(*(BYTE*)(ZwQueryInformationProcessAddr + i)));
			*(BYTE*)(ZwQueryInformationProcessAddr + i) = _byte;
			i++;
			Utils::Log("> RestoreZwQueryInformationProcess: " + to_string(i));
		}
		//VirtualProtect((LPVOID)&ZwQueryInformationProcessAddr, sizeof(ZwQIP), oldest, &old);
		Utils::Log("> RestoreZwQueryInformationProcess: Ok");
	}
	else {
		Utils::Log("> RestoreZwQueryInformationProcess: Access denied");
	}

	return ZwQueryInformationProcessAddr;
}

DWORD UltimateHooks::VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size) {
	NewFunction = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	return (DWORD)NewFunction;
}

void UltimateHooks::CopyRegion(DWORD dest, DWORD source, size_t size) {
	Utils::Log("> CopyRegion: Dest:         0x" + to_hex(dest));
	Utils::Log("> CopyRegion: Source:       0x" + to_hex(source));
	Utils::Log("> CopyRegion: Size:         0x" + to_hex(size));
	Utils::Log("> CopyRegion: SourceOffset: 0x" + to_hex(source - (DWORD)GetModuleHandle(NULL)));
	DWORD oldP;
check:
	VirtualProtect((LPVOID)dest, size, PAGE_EXECUTE_READWRITE, &oldP);
	if (oldP != PAGE_EXECUTE_READWRITE) {
		Utils::Log(" > Check if CopyRegion possible");
		VirtualProtect((LPVOID)dest, size, oldP, &oldP);
		std::this_thread::sleep_for(100ms);
		goto check;
	}

	{ // Dest
		while (true) {
			if (memory_readable((void*)source, size)) {
				Utils::Log("> Check CopyRegion Destination accessability: True");
				break;
			}
			else {
				Utils::Log("> Check CopyRegion Destination accessability: False");
				std::this_thread::sleep_for(1000ms);
			}
		}

		while (true) {
			if (!Utils::IsBadReadPtr((void*)source, (int)size)) {
				Utils::Log("> Check CopyRegion Destination IsBadReadPtr: False");
				break;
			}
			else {
				Utils::Log("> Check CopyRegion Destination IsBadReadPtr: True");
				std::this_thread::sleep_for(1000ms);
			}
		}
	}
	{ // Source
		while (true) {
			if (memory_readable((void*)source, size)) {
				Utils::Log("> Check CopyRegion Source accessability: True");
				break;
			}
			else {
				Utils::Log("> Check CopyRegion Source accessability: False");
				std::this_thread::sleep_for(1000ms);
			}
		}

		while (true) {
			if (!Utils::IsBadReadPtr((void*)source, (int)size)) {
				Utils::Log("> Check CopyRegion Source IsBadReadPtr: False");
				break;
			}
			else {
				Utils::Log("> Check CopyRegion Source IsBadReadPtr: True");
				std::this_thread::sleep_for(1000ms);
			}
		}
	}

	(void)memcpy((void*)dest, (PVOID)source, size);
	Utils::Log("> CopyRegion: Ok");
}

void UltimateHooks::FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size)
{
	//tried without zydis, but its very big work
	for (int i = 0; i < size; i++) {
		if (*(BYTE*)(NewFnAddress + i) == 0xE8) { // Very Bad realisation of searching CALL instruction
			DWORD oldOffset = 0xFFFFFFFF - *(DWORD*)(NewFnAddress + i + 1) - 4;
			DWORD funcPtr = (OldFnAddress + i) - oldOffset;
			DWORD newOffset = funcPtr - (NewFnAddress + i) - 5;
			*(DWORD*)(NewFnAddress + i + 1) = newOffset;
			Utils::Log("FixFuncRellocation: " + to_string(i));
			i += 4;
		}
	}
	for (int i = 0; i < size; i++) {
		if (*(BYTE*)(NewFnAddress + i) == 0xE9) { // Very Bad realisation of searching JMP instruction
			DWORD oldOffset = 0xFFFFFFFF - *(DWORD*)(NewFnAddress + i + 1) - 4;
			DWORD funcPtr = (OldFnAddress + i) - oldOffset;
			DWORD newOffset = funcPtr - (NewFnAddress + i) - 5;

			Utils::Log("FixFuncRellocation: " + to_string(i));
			i += 4;
		}
	}

	/* Utils::Log("> FixFuncRellocation");
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

	ZyanU32 runtime_address = NewFnAddress;
	ZyanUSize offset = 0;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;
	Utils::Log("> FixFuncRellocation: Init: Ok");
	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
	{
		Utils::Log("> FixFuncRellocation: Decode buffer");
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
			runtime_address);

		std::string mnemonic(buffer);

		if (mnemonic.find("call 0x") != std::string::npos) {
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			{
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);

						if (mnemonic1.find("call 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(5, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			{
				DWORD calc1 = (runtime_address - originalCall + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					{
						DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc1 = (runtime_address - hex + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
							}

						}
					}
				}
			}
			fixedAddressesCount++;
		}
		else if (mnemonic.find("int3") != std::string::npos) {
			*(BYTE*)(NewFnAddress + offset) = 0x90;
			fixedAddressesCount++;
		}
		else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
			DWORD originalCall = 0;
			{
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);
						if (mnemonic1.find("jmp 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(4, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			{
				DWORD calcx = originalCall - (OldFnAddress + offset);
				DWORD calcy = calcx + (OldFnAddress + offset);
				DWORD calc = calcy - runtime_address - 0x5;

				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					{
						DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc = calcx - 0x5;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
							}

						}
					}
				}
			}
			fixedAddressesCount++;
		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}*/
}

bool UltimateHooks::addHook(DWORD address, DWORD hkAddress, size_t offset)
{
	if (Hook(address, hkAddress, offset))
	{
		return true;
	}
	return false;
}

bool UltimateHooks::Hook(DWORD original_fun, DWORD hooked_fun, size_t offset)
{
	Utils::Log("> Hook");
	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	if (!VirtualQuery(reinterpret_cast<void*>(original_fun), &mbi, sizeof(mbi))) {
		return false;
	}

	HookEntries hs;
	HookDetails hd;

	hd.addressToHook = original_fun;
	hd.hookAddress = hooked_fun;
	hs.addressToHookOldProtect = mbi.Protect;

	std::vector<HookEntries> _hookEntries;
	bool isFound = false;

	for (HookEntries hs : hookEntries) {
		if (hs.addressToHookMbiStart == (DWORD)mbi.BaseAddress) {
			bool isExisting = false;
			for (HookDetails hd : hs.hookDetails) {
				if (original_fun == hd.addressToHook) {
					isExisting = true;
				}
			}
			if (!isExisting) {
				isFound = true;
				hs.hookDetails.push_back(hd);
			}
		}

		_hookEntries.push_back(hs);
	}
	if (isFound) {
		hookEntries = _hookEntries;
		return true;
	}
	Utils::Log("> Hook: Allocating");
	hs.addressToHookMbiStart = ((DWORD)mbi.BaseAddress);
	hs.addressToHookMbiEnd = ((DWORD)mbi.BaseAddress) + 0x1000;
	hs.addressToHookMbiSize = 0x1000;
	PVOID NewRegionPVOID = nullptr;
	Utils::Log("> Hook: VirtualAllocateRegion");
	Utils::Log("> Hook: VirtualAllocateRegion: mbi.BaseAddress: " + to_hex((int)mbi.BaseAddress));
	int i = 0;

	DWORD NewRegion = VirtualAllocateRegion(NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000) + 0x1000;
	Utils::Log("> Hook: VirtualAllocateRegion: NewRegion: " + to_hex(NewRegion));
	Utils::Log("> Hook: VirtualAllocateRegion: Ok");
	Utils::Log("> Hook: CopyRegion: " + to_hex(((DWORD)mbi.BaseAddress - 0x1000)));


	while (*(int*)(mbi.BaseAddress) == 0x0) {
		Utils::Log("> Hook: Waiting to hook mbi.BaseAddress");
	}

	CopyRegion((DWORD)NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000);

	Utils::Log("> Hook: CopyRegion: Ok");
	Utils::Log("> Hook: FixRellocation");

	FixRellocation(((DWORD)mbi.BaseAddress - 0x1000), ((DWORD)mbi.BaseAddress - 0x1000) + 0x3000, (DWORD)NewRegionPVOID, 0x3000, offset);
	Utils::Log("> Hook: FixRellocation: Ok");
	Utils::Log("> Hook: Allocating: Ok");
	hs.allocatedAddressStart = NewRegion;
	hs.allocatedAddressEnd = NewRegion + 0x1000;
	hs.allocatedAddressSize = 0x1000;
	hs.addressToHookoffsetFromStart = original_fun - ((DWORD)mbi.BaseAddress);
	hs.hookDetails.push_back(hd);

	for (HookEntries he : hookEntries) {
		if ((he.addressToHookMbiStart == hs.addressToHookMbiStart) &&
			(he.addressToHookMbiEnd == hs.addressToHookMbiEnd))
		{
			return true;
		}
	}

	if (!IsDoneInit)
	{
		Utils::Log("> Hook: IsDoneInit");
		VEH_Handle = AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(LeoHandler));
		IsDoneInit = true;
		Utils::Log("> Hook: IsDoneInit: Ok");
	}

	if (VEH_Handle)
	{
		Utils::Log("> Hook: VEH_Handle");
		auto addr = (PVOID)original_fun;
		auto size = static_cast<SIZE_T>(static_cast<int>(1));
		if (NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", RtlInterlockedCompareExchange64Offst, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, PAGE_READONLY, &hs.addressToHookOldProtect)))
		{
			hookEntries.push_back(hs);
			Utils::Log("> Hook: VEH_Handle: Ok");
			Utils::Log("> Hook: Ok");
			return true;
		}
	}
	return false;
}

MODULEINFO GetModuleInfo2(HMODULE m)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule;
	if (m)
		hModule = m;
	else
		hModule = GetModuleHandle(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}



void UltimateHooks::FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset)
{
	Utils::Log("FixRellocation: OldFnAddress: " + to_hex(OldFnAddress));
	Utils::Log("FixRellocation: NewFnAddress: " + to_hex(NewFnAddress));

#if (!(USE_ZIDYS) || DEBUG_RELLOCATION)
	long RellocationOffset = OldFnAddress - NewFnAddress;
	Utils::Log("RellocationOffset: " + to_string(RellocationOffset));

	OldFnAddress += (DWORD)_offset;
	for (int i = 0; i < size; i++) {
		DWORD runtime_addr = (NewFnAddress + i);
		BYTE* runtime = (BYTE*)runtime_addr;
		bool isCall = *(BYTE*)runtime_addr == 0xE8;
		bool isJmp = *(BYTE*)runtime_addr == 0xE9;

		if (isCall || isJmp) { // Very Bad realisation of searching CALL instruction
			long oldOffset = *(long*)(runtime_addr + 1);
			//Utils::Log("Old offset: " + to_string(oldOffset));
			//if (oldOffset >= 0x1000 || oldOffset <= -1000) {
			DWORD funcPtr = (OldFnAddress + i) + oldOffset + 5;
			DWORD calcx = funcPtr - (OldFnAddress);

			//if ((long)(OldFnAddress + i) - oldOffset <= 0)
			//	continue;

			BYTE* function = (BYTE*)funcPtr;



			if (runtime[-1] == 0x80) // if sub instruction
				continue;
			if (runtime[-1] == 0x83) // if sub instruction
				continue;
			if (runtime[-1] == 0x8B) // if mov instruction
				continue;
			if (runtime[-2] == 0x8D) // if lea instruction
				continue;
			if (runtime[-2] == 0x81) // if add instruction
				continue;
			if (runtime[-2] == 0x0F) // if subps instruction
				continue;
			if (runtime[-4] == 0xF3) // if movss instruction
				continue;
			if (runtime[-1] == 0xC1) // if shr instruction
				continue;
			if (runtime[-1] == 0xD0) // if shr instruction
				continue;

			long newOffset = funcPtr - runtime_addr - 6;

			Utils::Log("FixRellocation: " + to_hex(runtime_addr) + "  |  " + string(*(BYTE*)runtime_addr == 0xE9 ? "JMP" : "CALL") + "  |  oldOffset: " + to_string(oldOffset) + "  |  newOffset: " + to_string(newOffset));
			
			if ((funcPtr >= OldFnAddress + 0x1000) && (funcPtr <= OldFnAddressEnd - 0x1000)) {
				//MessageBoxA(0, ("oldOffset: " + to_hex(runtime_addr) + "\n\noldOffset: " + to_string(oldOffset)).c_str(), "My Rellocation", 0);
				Utils::Log("FixRellocation: " + to_hex(runtime_addr) + "  |  " + string(isCall ? "CALL1" : "JMP1"));
				continue;
			}



#if !(DEBUG_RELLOCATION)
			* (DWORD*)(NewFnAddress + i + 1) = newOffset;
#endif
			i += 3;
		}
	}
	OldFnAddress -= (DWORD)_offset;
#endif

#if USE_ZIDYS || DEBUG_RELLOCATION
	Utils::Log("\n\nZydis Rellocation: " + to_hex(RellocationOffset));

	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

	ZyanU32 runtime_address = NewFnAddress + _offset;
	ZyanUSize offset = _offset;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;

	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
	{
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
			runtime_address);

		std::string mnemonic(buffer);
		if (mnemonic.find("call 0x") != std::string::npos) {
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			{
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);

						if (mnemonic1.find("call 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(5, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			{

				DWORD calc1 = (runtime_address - originalCall + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				long oldOffset = *(DWORD*)(runtime_address + 1);
				long newOffset = calc;
				*(DWORD*)(runtime_address + 1) = calc;
				Utils::Log("FixRellocation: " + to_hex(runtime_address) + "  |  CALL" + "  |  oldOffset: " + to_string(oldOffset) + "  |  newOffset: " + to_string(newOffset));
				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					{
						DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress + 0x1000) && (hex <= OldFnAddressEnd - 0x1000)) {
							//MessageBoxA(0, to_hex(runtime_address + 1).c_str(), "JMP Before", 0);
							DWORD calc1 = (runtime_address - hex + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(runtime_address + 1) = calc;
							//MessageBoxA(0, to_hex(runtime_address + 1).c_str(), "JMP After", 0);
							Utils::Log("FixRellocation: " + to_hex(runtime_address) + "  |  CALL1");
							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
							}

						}
					}
				}
			}
			fixedAddressesCount++;
		}
		else if (mnemonic.find("int3") != std::string::npos) {
			*(BYTE*)(NewFnAddress + offset) = 0x90;
			fixedAddressesCount++;
		}
		else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			{
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);
						if (mnemonic1.find("jmp 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(4, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			{

				DWORD calcx = originalCall - (OldFnAddress + offset);
				DWORD calcy = calcx + (OldFnAddress + offset);
				DWORD calc = calcy - runtime_address - 0x5;
				long oldOffset = *(DWORD*)(runtime_address + 1);
				long newOffset = calc;
				*(DWORD*)(runtime_address + 1) = calc;
				Utils::Log("FixRellocation: " + to_hex(runtime_address) + "  |  JMP" + "  |  oldOffset: " + to_string(oldOffset) + "  |  newOffset: " + to_string(newOffset));
				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					{
						DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress + 0x1000) && (hex <= OldFnAddressEnd - 0x1000)) {
							//MessageBoxA(0, to_hex(runtime_address).c_str(), "JMP Before", 0);
							DWORD calc = calcx - 0x5;
							*(DWORD*)(runtime_address + 1) = calc;
							//MessageBoxA(0, to_hex(runtime_address).c_str(), "JMP After", 0);
							//Utils::Log("FixRellocation JMP2 - " + to_hex(runtime_address));
							Utils::Log("FixRellocation: " + to_hex(runtime_address) + "  |  JMP1");
							MessageBoxA(0, ("oldOffset: " + to_hex(runtime_address)).c_str(), "UltHook", 0);
							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
								char buffer[256];

								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);
								std::string mnemonic(buffer);
							}

						}
					}
				}

			}
			fixedAddressesCount++;

		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}
#endif

	MessageBoxA(0, "finished", "Relloc", 0);
}