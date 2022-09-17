#pragma once
#include "Utils.h"
#include "Enums.h"
#include "Vector.h"
#include "Offset.h"
#include "League/SpellBook.h"
#include "BuffManager.h"
#include "League/HeroInventory.h"

class SpellDataResource
{
public:
	union
	{
		DEFINE_MEMBER_N(float MissileSpeed, 0x460)
			DEFINE_MEMBER_N(float Width, 0x49C)
			DEFINE_MEMBER_N(float Radius, 0x40C)
	};
};

class SpellData
{
public:
	union
	{
		DEFINE_MEMBER_N(std::string Name, 0x18)
		DEFINE_MEMBER_N(SpellDataResource* Resource, 0x40)
	};
};

class SpellInfo {
public:
	union {
			DEFINE_MEMBER_0(SpellData* BasicAttackSpellData)
			DEFINE_MEMBER_N(kSpellSlot		Slot, Offset::SpellInfo::Slot)
			DEFINE_MEMBER_N(float			StartTime, Offset::SpellInfo::StartTime)
			DEFINE_MEMBER_N(int				SpellIndex, Offset::SpellInfo::SpellIndex)
			DEFINE_MEMBER_N(unsigned int	Level, Offset::SpellInfo::Level)
			DEFINE_MEMBER_N(int	source_id, Offset::SpellInfo::source_id)
			DEFINE_MEMBER_N(unsigned int	SourceNetworkID, Offset::SpellInfo::SourceNetworkID)
			DEFINE_MEMBER_N(Vector3			StartPosition, Offset::SpellInfo::StartPosition)
			DEFINE_MEMBER_N(Vector3			EndPosition, Offset::SpellInfo::EndPosition)
				DEFINE_MEMBER_N(Vector3			EndPosition2, Offset::SpellInfo::EndPosition2)
			DEFINE_MEMBER_N(bool			HasTarget, Offset::SpellInfo::HasTarget)
			DEFINE_MEMBER_N(DWORD			TargetArray, 0xB8)
			DEFINE_MEMBER_N(DWORD			TargetSize, 0xC0)
	};

	unsigned int targetIndex()
	{
		auto target_info = this->TargetArray + 0x10 * this->TargetSize;
		//console.Print("%d", target_info);
		return *(int*)((DWORD)target_info - 0x10);
	}

};

class AIManager
{
public:
	union
	{
		DEFINE_MEMBER_N(Vector3 NavStartPos, Offset::AIManager::NavStart)
			DEFINE_MEMBER_N(Vector3 NavEndPos, Offset::AIManager::NavEnd)
			DEFINE_MEMBER_N(Vector3 ServerPos, Offset::AIManager::ServerPos)
			DEFINE_MEMBER_N(Vector3 Velocity, Offset::AIManager::Velocity)
			DEFINE_MEMBER_N(bool Moving, Offset::AIManager::Moving)
			DEFINE_MEMBER_N(bool Dashing, Offset::AIManager::Dashing)
			DEFINE_MEMBER_N(float DashingSpeed, Offset::AIManager::DashingSpeed)
			DEFINE_MEMBER_N(Vector3* NavArray, Offset::AIManager::NavArray)
			DEFINE_MEMBER_N(Vector3* NavArrayEnd, Offset::AIManager::NavArrayEnd)
	};

	int pathSize()
	{
		return ((DWORD)this->NavArrayEnd - (DWORD)this->NavArray) / (sizeof(Vector3));
	}

	float pathLength()
	{
		float length = 0;
		for (int i = 0; i < this->pathSize(); ++i)
			length += this->NavArray[i + 1].distanceTo(this->NavArray[i]);

		return length;
	}

	std::list<Vector3> navPath()
	{

	}
};

class GameObject {
public:
	union {
		DEFINE_MEMBER_0(DWORD* VTable)
			DEFINE_MEMBER_N(int				Index, Offset::GameObject::Index)
			DEFINE_MEMBER_N(int				Team, Offset::GameObject::TeamID)
			DEFINE_MEMBER_N(unsigned int	NetworkID, Offset::GameObject::NetworkID)
			DEFINE_MEMBER_N(Vector3			Position, Offset::GameObject::Position)
			DEFINE_MEMBER_N(bool			IsVisible, Offset::GameObject::Visibility)
			//DEFINE_MEMBER_N(int				Dead, Offset::GameObject::Dead)
			DEFINE_MEMBER_N(bool			IsTargetable, Offset::GameObject::Targetable)
			//DEFINE_MEMBER_N(bool			IsInvulnearable, Offset::GameObject::Invulnearable)
			DEFINE_MEMBER_N(float			Health, Offset::GameObject::Health)
			DEFINE_MEMBER_N(float			Shield, Offset::GameObject::Shield)
			DEFINE_MEMBER_N(float			MaxHealth, Offset::GameObject::MaxHealth)
			DEFINE_MEMBER_N(float			Mana, Offset::GameObject::Mana)
			DEFINE_MEMBER_N(float			Mana, Offset::GameObject::MaxMana)
			DEFINE_MEMBER_N(float			Armor, Offset::GameObject::Armor)
			DEFINE_MEMBER_N(float			MagicRes, Offset::GameObject::MagicRes)
			DEFINE_MEMBER_N(float			BonusArmor, Offset::GameObject::BonusArmor)
			DEFINE_MEMBER_N(float			AttackRange, Offset::GameObject::AttackRange)
			DEFINE_MEMBER_N(float			AttackSpeedMulti, Offset::GameObject::AttackSpeedMulti)
			DEFINE_MEMBER_N(float			BaseAttackDamage, Offset::GameObject::BaseAttackDamage)
			DEFINE_MEMBER_N(float			BonusAttackDamage, Offset::GameObject::BonusAttackDamage)
			DEFINE_MEMBER_N(float			FlatPhysicalDamageMod, Offset::GameObject::BonusAttackDamage)
			DEFINE_MEMBER_N(float			BaseAbilityDamage, Offset::GameObject::AbilityPower)
			DEFINE_MEMBER_N(float			AbilityDamageMult, Offset::GameObject::AbilityPowerMult)
			DEFINE_MEMBER_N(float			ArmorPen, Offset::GameObject::Lethality)
			DEFINE_MEMBER_N(float			ArmorPenMod, Offset::GameObject::ArmorPen)
			DEFINE_MEMBER_N(float			MagicPenFlat, Offset::GameObject::MagicPenFlat)
			DEFINE_MEMBER_N(float			MagicPenMod, Offset::GameObject::MagicPenMod)
			DEFINE_MEMBER_N(void* CharacterDataStack, Offset::GameObject::CharacterDataStack)
			DEFINE_MEMBER_N(int* Skin, 0x2CB0)
			//DEFINE_MEMBER_N(float			FlatMagicDamageMod, Offset::GameObject::FlatMagicDamageMod)
			DEFINE_MEMBER_N(float			MovementSpeed, Offset::GameObject::MoveSpeed)
			DEFINE_MEMBER_N(char* ChampionName, Offset::GameObject::ChampionName)
			//DEFINE_MEMBER_N(SpellData* MissileSpellInfo, Offset::GameObject::MissileSpellInfo)
			//DEFINE_MEMBER_N(int				MissileSrcInx, Offset::GameObject::MissileSrcIndex)
			//DEFINE_MEMBER_N(Vector3			MissileStartPos, Offset::GameObject::MissileStartPos)
			//DEFINE_MEMBER_N(Vector3			MissileEndPos, Offset::GameObject::MissileEndPos)
			//DEFINE_MEMBER_N(int				MissileDestIdx, Offset::GameObject::MissileDestIndex)
			DEFINE_MEMBER_N(CSpellBook		SpellBook, Offset::GameObject::SpellBook)
			DEFINE_MEMBER_N(HeroInventory		Inventory, Offset::Inventory::Instance)
			//DEFINE_MEMBER_N(CharacterData* BaseCharacterData, Offset::GameObject::BaseCharacterData)
			DEFINE_MEMBER_N(BuffManager BuffManager, Offset::BuffManager::Instance)
			DEFINE_MEMBER_N(CombatType combat, Offset::GameObject::CombatType)
			DEFINE_MEMBER_N(int RecallState, Offset::GameObject::RecallState);

	};

	AIManager* GetAIManager() {
		typedef AIManager* (__thiscall* OriginalFn)(PVOID);
		return CallVirtual<OriginalFn>((PVOID)this, 150)((PVOID)this);


		//return reinterpret_cast<AIManager * (__thiscall*)(GameObject*)>(this->VTable[149])(this);
	}

	float GetAbilityPower() {
		return this->BaseAbilityDamage + (this->BaseAbilityDamage * this->AbilityDamageMult);
	}

	float GetBoundingRadius() {
		typedef float(__thiscall* fnGetBoundingRadius)(GameObject* pObj);
		fnGetBoundingRadius gbr = (fnGetBoundingRadius)(DEFINE_RVA(Offset::Function::GetBoundingRadius));
		return gbr(this);
		//return Function::GetBoundingRadius(this);
		//return reinterpret_cast<float(__thiscall*)(GameObject*)>(this->VTable[35])(this);
	}

	bool IsInvulnearable() {
		return *(BYTE*)((DWORD)this + Offset::GameObject::Invulnearable) % 0x2 == 1;
	}

	bool isValid()
	{
		return this->Position.IsValid();
	}

	bool IsAllyTo(GameObject* Obj) {
		return Obj->Team == this->Team;
	}
	bool IsNeutral() {
		return this->Team == 300;
	}
	bool IsEnemyTo(GameObject* Obj) {
		if (this->IsNeutral() || Obj->IsNeutral())
			return false;
		return !IsAllyTo(Obj);
	}
	std::string GetChampionName() {
		if (this->ChampionName == "") {
			return 0;
		}
		return std::string(this->ChampionName);
	}

	std::string GetNickName() {
		return std::string(this->ChampionName);
	}

	bool IsInRange(GameObject* object, float distance, bool takeBoundingRadius)
	{
		float rangeSqr;
		if (takeBoundingRadius)
		{
			float objectBoundingBox = object->GetBoundingRadius() / 1.5f;
			float sourceBoundingBox = this->GetBoundingRadius();
			float boundingBoxTotalDistanceSqr = objectBoundingBox * objectBoundingBox + sourceBoundingBox * sourceBoundingBox;
			float realRange = distance + objectBoundingBox + sourceBoundingBox;
			rangeSqr = realRange * realRange;
			return this->Position.DistanceSquared(object->Position) <= rangeSqr;
		}
		else {
			rangeSqr = distance * distance;
			return this->Position.DistanceSquared(object->Position) < rangeSqr;
		}
	}

	bool IsInRange(Vector3 position)
	{
		float rangeSqr = this->AttackRange * this->AttackRange;
		return this->Position.DistanceSquared(position) < rangeSqr;
	}

	bool IsMelee()
	{
		return this->combat == CombatType::Melee;
	}

};

template<typename T>
struct SEntityList {
	char pad[0x4];
	T** entities;
	size_t size;
	size_t max_size;
};

class MissileObject {
public:
	union {
		DEFINE_MEMBER_N(int				DestIdx, Offset::MissileData::DestIdx);

		DEFINE_MEMBER_N(CSpellInfo			SpellInfo, Offset::MissileData::SpellInfo);
		DEFINE_MEMBER_N(int					SrcIdx, Offset::MissileData::SrcIdx);
		DEFINE_MEMBER_N(Vector3				CurrentPos, 0xD8);
		DEFINE_MEMBER_N(Vector3				StartPos, Offset::MissileData::StartPos);		
		DEFINE_MEMBER_N(Vector3				EndPos, Offset::MissileData::EndPos);
	};
};