#include "Utils.h"
#include <chrono>
#include <sstream>
#include <fstream>
#include "Offset.h"

using namespace std;

bool IsLeagueInForeground()
{
	//return true;
	return !*(bool*)((*(int*)DEFINE_RVA(Offset::Data::HudInstance)) + 0x69);

	/*TCHAR title[500];
	GetWindowText(GetForegroundWindow(), title, 500);
	return wcscmp(title, L"League of Legends (TM) Client") == 0;*/
}

HWND GetHwndProc()
{
	HWND g_hWindow = GetTopWindow(NULL);
	DWORD currentPID = GetCurrentProcessId();
	do
	{
		char title[256];
		if ((GetWindowTextA(g_hWindow, title, 256) > 0) && (IsWindowVisible(g_hWindow)))
		{
			DWORD procId;
			GetWindowThreadProcessId(g_hWindow, &procId);

			if (procId == currentPID)
			{
				return g_hWindow;
			}
		}

		g_hWindow = GetNextWindow(g_hWindow, GW_HWNDNEXT);
	} while (g_hWindow);
	return NULL;
}

std::string to_hex(int i) {
	std::stringstream stream;
	stream << std::hex << i;
	std::string result(stream.str());

	return result;
}

bool firstTime = true;

void Utils::Log(string text) {
	auto time = chrono::system_clock::now();
	auto time_t = chrono::system_clock::to_time_t(time);
	ofstream myfile;
	if (firstTime) {
		myfile.open("Log.txt");
		firstTime = !firstTime;
	}
	else {
		myfile.open("Log.txt", ios_base::app);
	}

	myfile << "[" + string(strtok(ctime(&time_t), "\n")) + "] " + text + "\n";
	myfile.close();

	//lastLog = text;

	//std::this_thread::sleep_for(100ms);
}

bool Utils::IsBadReadPtr(void* p, int size) {

	for (int i = 0; i < size / 0x80; i++) {
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		if (::VirtualQuery((LPCVOID)((DWORD)p + i * 0x80), &mbi, sizeof(mbi)))
		{
			DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool b = !(mbi.Protect & mask);
			// check the page is not a guard page
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			{
				Utils::Log(">> IsBadReadPtr: Error: [" + to_string(i) + "] " + to_hex((DWORD)p + i * 0x100) + " (" + to_string((int)((DWORD)p + i * 0x100)) + ")");
				return true;
			}


		}
	}
	return false;
}

std::string ToLower(std::string str)
{
	std::string strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

std::wstring ToLower(std::wstring str)
{
	std::wstring strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

bool StringContains(std::string strA, std::string strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::string::npos)
		return true;

	return false;
}

bool StringContains(std::wstring strA, std::wstring strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::wstring::npos)
		return true;

	return false;
} 