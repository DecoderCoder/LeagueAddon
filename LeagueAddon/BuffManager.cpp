#include "BuffManager.h"
//#include <olectl.h> 
//#include <stddef.h> 

ULONG BuffScript::buffHash()
{
	return *reinterpret_cast<ULONG*>(reinterpret_cast<ULONG>(this + static_cast<int>(Offset::BuffManager::BuffHash)));
}

std::string BuffEntry::name()
{
	//DWORD aux = *(DWORD*)((int)this + Offset::BuffManager::BuffName);
	//if (aux == NULL)
	//	return "invalidBuff";
	//if (*(DWORD*)(aux + Offset::BuffManager::BuffName2) == NULL)
	//	return "invalidBuff";
	//return (char*)(aux + Offset::BuffManager::BuffName2);

	const ULONG pBuffScript = *reinterpret_cast<ULONG*>(reinterpret_cast<ULONG>(this) + 0x8);
	if (!(pBuffScript > 0x1000))
		return xorstr("invalidBuff");
	auto name = reinterpret_cast<char*>(pBuffScript + static_cast<int>(Offset::BuffManager::BuffName2));
	return std::string(name);
}

int BuffEntry::stacksAlt()
{
	return *(int*)(reinterpret_cast<ULONG>(this) + static_cast<int>(Offset::BuffManager::BuffCountAlt));
	//return (*reinterpret_cast<int*>(reinterpret_cast<ULONG>(this) + static_cast<int>(Offset::BuffManager::BuffCountAlt2)) - *reinterpret_cast<int*>(reinterpret_cast<ULONG>(this) + static_cast<int>(Offset::BuffManager::BuffCountAlt))) >> 3;
}

int BuffEntry::getCount()
{
	return *(int*)(reinterpret_cast<ULONG>(this) + static_cast<int>(Offset::BuffManager::BuffCount));
}

bool BuffManager::hasBuff(std::string name)
{
	for (auto b : this->entries()) {
		if (b->name() == name) {
			return true;
		}
	}
	return false;
}

BuffEntry* BuffManager::getBuff(std::string name)
{
	for (auto buff : this->entries()) {
		if (buff->name() == name)
			return buff;
	}
	return nullptr;
}

bool BuffManager::buffStrainer(BuffEntry* buff)
{
	if (!buff || static_cast<int>(buff->type) == -1)
		return false;
	if (reinterpret_cast<ULONG>(buff) == 0 || !(reinterpret_cast<ULONG>(buff) > 0x1000))
		return false;
	if ((int)(*(float*)(DEFINE_RVA(Offset::Data::GameTime))) > buff->endTime)
		return false;
	if (strcmp(buff->name().c_str(), xorstr("invalidBuff")) == 0 || buff->stacksAlt() == 0)
		return false;
	return true;
}

std::vector<BuffEntry*> BuffManager::entries()
{
	std::vector<BuffEntry*> buffs = *reinterpret_cast<std::vector<BuffEntry*>*>(this + static_cast<int>(Offset::BuffManager::ArrayStart));
	return filterVector(buffs, buffStrainer);
}