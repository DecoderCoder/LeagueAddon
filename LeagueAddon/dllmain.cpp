#include <thread>
#include "Utils.h"
#include "Hooks/Hooks.h"
#include "AddonEngine.h"
#include "EventManager/EventManager.h"
#include "psapi.h"

HMODULE g_module;
uintptr_t initThreadHandle;
PVOID exceptionHandler;

bool hooked = true;

void OnExit() noexcept {
	Utils::Log("OnExit");
	RemoveVectoredExceptionHandler(exceptionHandler);
	//EventManager::Trigger(EventManager::EventType::OnUnLoad);
	if (hooked) {
		Hooks::RemoveHooks();
		hooked = false;
	}

	// render.Free();
	 //config.save(g_module);
	Utils::Log("OnExit: Ok");
	CloseHandle((HANDLE)initThreadHandle);
}

MODULEINFO GetModuleInfo(HMODULE m)
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

DWORD cheatAddr = (DWORD)GetModuleHandleA("LeagueAddon.dll");
DWORD cheatEndAddr = (DWORD)(cheatAddr + GetModuleInfo(GetModuleHandleA("LeagueAddon.dll")).SizeOfImage);

LONG CALLBACK TopLevelHandler(EXCEPTION_POINTERS* info)
{

	if (info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{

		if (info->ExceptionRecord->ExceptionAddress > (PVOID)cheatAddr && info->ExceptionRecord->ExceptionAddress < (PVOID)cheatEndAddr) {
			Utils::Log("Cheat Addr : " + to_hex(cheatAddr));
			Utils::Log("Error Addr : " + to_hex((int)info->ExceptionRecord->ExceptionAddress));
			Utils::Log("Error Addr : " + to_hex((int)((int)info->ExceptionRecord->ExceptionAddress - cheatAddr)));
			Utils::Log("IDA   Addr : " + to_hex((int)((int)info->ExceptionRecord->ExceptionAddress - cheatAddr + 0x10000000)));
		}
		//Utils::Log("Global Error Addr : " + to_hex((int)info->ExceptionRecord->ExceptionAddress));
		//{
		//	HMODULE hMods[1024];
		//	DWORD cbNeeded;
		//	unsigned int i;

		//	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
		//	{
		//		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		//		{
		//			CHAR szModName[MAX_PATH];

		//			// Get the full path to the module's file.

		//			if (GetModuleFileNameExA(GetCurrentProcess(), hMods[i], szModName,
		//				sizeof(szModName) / sizeof(TCHAR)))
		//			{
		//				MODULEINFO mInfo = GetModuleInfo(hMods[i]);

		//				string str;
		//				str.append(szModName);
		//				if (mInfo.EntryPoint < info->ExceptionRecord->ExceptionAddress && (DWORD)mInfo.EntryPoint + mInfo.SizeOfImage >(DWORD)info->ExceptionRecord->ExceptionAddress) {

		//					Utils::Log("GetModuleFileNameExA : " + str);
		//					Utils::Log("EntryPoint           : " + to_hex((int)mInfo.EntryPoint));
		//					Utils::Log("ErrorAddress         : " + to_hex((int)((DWORD)info->ExceptionRecord->ExceptionAddress)));
		//					Utils::Log("IDA ErrorAddress         : " + to_hex((int)((DWORD)info->ExceptionRecord->ExceptionAddress - (int)mInfo.EntryPoint + 0x10000000)));
		//					Utils::Log("EndPoint             : " + to_hex((int)((DWORD)mInfo.EntryPoint + mInfo.SizeOfImage)));
		//				}


		//			}
		//		}
		//	}
		//}
		
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

DWORD WINAPI MainThread(LPVOID param) {
	Utils::Log(">> LEAGUE ADDON INJECTED <<");
	//#if _DEBUG
	Utils::Log(">> LEAGUE ADDON ADDR: " + to_hex((int)GetModuleHandleA("LeagueAddon.dll")) + " <<");
	//#endif

	exceptionHandler = AddVectoredExceptionHandler(TRUE, TopLevelHandler);


	while (Function::GameTime() < 2) {
		std::this_thread::sleep_for(1ms);
	}
	int* recall = (int*)&((GameObject*)*(DWORD*)DEFINE_RVA(Offset::Data::LocalPlayer))->RecallState;
	while ((*(int*)DEFINE_RVA(Offset::Data::LocalPlayer) == 0 || (*recall == (int)kRecallState::None || *recall == (int)kRecallState::Yuumi_W_Ally)) && !GetAsyncKeyState(VK_INSERT)) {
		std::this_thread::sleep_for(1ms);
	}

	std::this_thread::sleep_for(1s);

	Hooks::ApplyHooks();

	AddonEngine::Initialize();

	while (!GetAsyncKeyState(VK_END)) {
		EventManager::Trigger(EventManager::EventType::OnThread);
		Sleep(1);
	}
	return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	g_module = hModule;
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::atexit(OnExit);
		initThreadHandle = _beginthreadex(nullptr, 0, (_beginthreadex_proc_type)MainThread, hModule, 0, nullptr);
		FreeLibrary(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		// 
		break;
	case DLL_PROCESS_DETACH:
		OnExit();
		break;
	}
	return TRUE;
}

