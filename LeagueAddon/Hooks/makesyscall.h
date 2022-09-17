#pragma once
#include <Windows.h>
#include <iostream>


using namespace std;

template <typename T = NTSTATUS>
class makesyscall
{
public:
	makesyscall(BYTE sysindex1, BYTE sysindex2, BYTE sysindex3, BYTE sysindex4, LPCSTR lpFuncName, DWORD offsetToFunc,
		BYTE retCode, BYTE ret1, BYTE ret2);
	~makesyscall();
public:
	template <typename... Args>
	T operator()(Args ... arg);
private:
	uint32_t GetSyscallIndex(const BYTE* pFuncAddr);
	void CreateShellSysCall(BYTE sysindex1, BYTE sysindex2, BYTE sysindex3, BYTE sysindex4, LPCSTR lpFuncName,
		DWORD offsetToFunc, BYTE retCode, BYTE ret1, BYTE ret2);
private:
	char* m_pShellCode;
};

template <typename T>
template <typename... Args>
T makesyscall<T>::operator()(Args ... arg)
{
	Utils::Log("> makesyscall: Init");
	if (!m_pShellCode)
		return T(0);

	typedef T(__stdcall* mFunc)(Args ...);
	mFunc pFunc = mFunc(m_pShellCode);
	Utils::Log("> makesyscall: Call func");

	auto pFuncRes = pFunc(arg...);
	Utils::Log("> makesyscall: Ok");
	return pFuncRes;
}


template <typename T>
makesyscall<T>::makesyscall(BYTE sysindex1, BYTE sysindex2, BYTE sysindex3, BYTE sysindex4, LPCSTR lpFuncName,
	DWORD offsetToFunc, BYTE retCode, BYTE ret1, BYTE ret2)
{
	//PBYTE pFuncAddr = (PBYTE)GetProcAddress(GetModuleHandleA("ntdll.dll"), lpFuncName);
	CreateShellSysCall(sysindex1, sysindex2, sysindex3, sysindex4, lpFuncName, offsetToFunc, retCode, ret1, ret2);
}

template <typename T>
makesyscall<T>::~makesyscall()
{
	if (m_pShellCode)
		VirtualFree(m_pShellCode, NULL, MEM_RELEASE);
}

template <typename T>
uint32_t makesyscall<T>::GetSyscallIndex(const BYTE* pFuncAddr)
{
	if (!pFuncAddr)
		return 0;

	for (int8_t i = 0; i < 0xF; ++i)
	{
		if (*(pFuncAddr + i) == 0xB8)
		{
			return *PUINT(pFuncAddr + i + 1);
		}
	}

	return 0;
}

template <typename T>
void makesyscall<T>::CreateShellSysCall(BYTE sysindex1, BYTE sysindex2, BYTE sysindex3, BYTE sysindex4,
	LPCSTR lpFuncName, DWORD offsetToFunc, BYTE retCode, BYTE ret1, BYTE ret2)
{

	if (!sysindex1 && !sysindex2 && !sysindex3 && !sysindex4)
		return;

#ifdef _WIN64
	BYTE ShellCode[]
	{
		0x4C, 0x8B, 0xD1,					//mov r10, rcx 
		0xB8, 0x00, 0x00, 0x00, 0x00,		        //mov eax, SysCallIndex
		0x0F, 0x05,					        //syscall
		0xC3								//ret				
	};

	m_pShellCode = (char*)VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!m_pShellCode)
		return;

	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	*(BYTE*)(m_pShellCode + 4) = sysindex1;
	*(BYTE*)(m_pShellCode + 5) = sysindex2;
	*(BYTE*)(m_pShellCode + 6) = sysindex3;
	*(BYTE*)(m_pShellCode + 7) = sysindex4;

#elif _WIN32
	BYTE ShellCode[]
	{
		0xB8, 0x00, 0x00, 0x00, 0x00, //mov eax, SysCallIndex
		0xBA, 0x00, 0x00, 0x00, 0x00, //mov edx, [function]
		0xFF, 0xD2, //call edx
		0xC2, 0x00, 0x00 //ret
	};
	m_pShellCode = static_cast<char*>(VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE));
	if (!m_pShellCode)
		return;
	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));
	*(uintptr_t*)(m_pShellCode + 6) = static_cast<uintptr_t>((DWORD)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), lpFuncName) + offsetToFunc);
	*(BYTE*)(m_pShellCode + 1) = sysindex1;
	*(BYTE*)(m_pShellCode + 2) = sysindex2;
	*(BYTE*)(m_pShellCode + 3) = sysindex3;
	*(BYTE*)(m_pShellCode + 4) = sysindex4;

	*(BYTE*)(m_pShellCode + 12) = retCode;
	*(BYTE*)(m_pShellCode + 13) = ret1;
	*(BYTE*)(m_pShellCode + 14) = ret2;
#endif
}