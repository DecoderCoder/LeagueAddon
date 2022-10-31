#pragma once
#include <string>
#include <algorithm>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <functional>
#include <list>
#include <iostream>
#include <ranges>
#include <sstream>
#include "xorstr.h"
#include <iomanip>

using namespace std;

#define baseAddr (DWORD)GetModuleHandleA(NULL)
#define DEFINE_RVA(address) (baseAddr + (DWORD)address)
#define STR_MERGE_IMPL(x, y)                x##y
#define STR_MERGE(x,y)                        STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)                        BYTE STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x)                    x;
#define DEFINE_MEMBER_N(x,offset)            struct { MAKE_PAD((DWORD)offset); x; };
#define GetAsyncKeyStateN(key)	GetAsyncKeyState(key) & 0x8000

bool IsLeagueInForeground();
HWND GetHwndProc();

template< typename T >
std::string to_hex(T i)
{
	std::stringstream stream;
	stream
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}

template <class Functor>
class Not
{
public:
	Not(Functor& f) : func(f) {}

	template <typename ArgType>
	bool operator()(ArgType& arg) { return !func(arg); }

private:
	Functor& func;
};

template<typename T, typename B>
T filter(T list, B pred) {
	T ret;
	std::remove_copy_if(
		list.begin(),
		list.end(),
		std::back_insert_iterator<T>(ret),
		Not<B>(pred)
	);

	return ret;
}

template<typename T, typename B>
T filterVector(T vector, B pred) {
	T ret;
	std::remove_copy_if(
		vector.begin(),
		vector.end(),
		std::back_insert_iterator<T>(ret),
		Not<B>(pred)
	);

	return ret;
}

template<typename T>
struct min_max
{
	T min;
	T max;

	void emplace_value(T& val)
	{
		if (val > max)
			max = val;

		if (val < min)
			min = val;
	}
	float percentage(T& val) { return (float(val - min)) / float(max - min); }
};

template <typename Function> Function CallVirtual(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];

	return (Function)(dwAddress);
}

bool StringContains(std::string strA, std::string strB, bool ignore_case = false);
bool StringContains(std::wstring strA, std::wstring strB, bool ignore_case = false);
bool StringCompare(std::string strA, std::string strB, bool ignore_case = false);
bool StringCompare(std::wstring strA, std::wstring strB, bool ignore_case = false);

template <typename Type>
struct encrypted_buffer
{
	BYTE get_is_filled() { return *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x0); }
	BYTE get_length_xor32() { return *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x1); }
	BYTE get_length_xor8() { return *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x2); }

	BYTE* get_key_array()
	{
		if constexpr (sizeof(Type) % 4 == 0)
			return reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x4);
		else
			return reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x3);
	}

	BYTE get_index()
	{
		if constexpr (sizeof(Type) % 4 == 0)
			return *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x8);
		else
			return *reinterpret_cast<BYTE*>(reinterpret_cast<DWORD>(this) + 0x4);
	}

	Type* get_values_array()
	{
		if constexpr (sizeof(Type) % 4 == 0)
			return reinterpret_cast<Type*>(reinterpret_cast<DWORD>(this) + 0xC);
		else
			return reinterpret_cast<Type*>(reinterpret_cast<DWORD>(this) + 0x5);
	}
	Type decrypt()
	{
		Type result = get_values_array()[get_index()];

		if (const auto length_xor32 = get_length_xor32()) {
			for (auto i = 0; i < length_xor32; ++i) {
				reinterpret_cast<PDWORD>(&result)[i] ^= ~(reinterpret_cast<PDWORD>(get_key_array())[i]);
			}
		}

		if (const auto length_xor8 = get_length_xor8()) {
			for (auto i = (sizeof(Type) - length_xor8); i < sizeof(Type); ++i) {
				reinterpret_cast<PBYTE>(&result)[i] ^= ~(get_key_array()[i]);
			}
		}


		return result;
	}
};

inline float GetDamageWithArmor(float Damage, float Armor) {
	if (Armor >= 0) {
		return Damage * (100 / (100 + Armor));
	}
	else {
		return Damage * (2 - (100 / (100 + Armor)));
	}
}

namespace Utils {
	//extern string lastLog;
	void Log(std::string text);
	//void ScriptLog(string text);
	//Vector2 WorldToScreen(const Vector3& pos);
	//bool IsLeagueInForeground();
	bool IsBadReadPtr(void* p, int size);
}

std::string ToLower(std::string str);
std::wstring ToLower(std::wstring str);

inline char* GetStr(DWORD offset) {
	if (*(int*)(offset + 0x10) > 15)
		return (char*)(*(DWORD*)offset);
	else
		return (char*)offset;
}

inline string QWERDF[6] = { "Q", "W", "E", "R", "D", "F" };