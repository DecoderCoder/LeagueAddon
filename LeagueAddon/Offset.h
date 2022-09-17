#pragma once

namespace Offset {
	namespace Data {
		static constexpr int LocalPlayer = 0x3141554; // 51 8B 0D ? ? ? ? 85 C9 74 21 
		static constexpr int GameTime = 0x313AFF0; // F3 0F 11 05 ? ? ? ? 8B 49
		static constexpr int ObjectManager = 0x18A503C; // A1 ?? ?? ?? ?? C7 40 ?? ?? ?? ?? ?? C3
		static constexpr int HudInstance = 0x18A50FC; //8B ? ? ? ? ? 6A 00 8B ? ? E8 ? ? ? ? B0 01 ?
		static constexpr int Camera = 0x31388E4; // E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8D 43 24

		static constexpr int ManagerTemplate_Minions = 0x24F2850; // 8B 0D ? ? ? ? E8 ? ? ? ? EB 09 56 E8 ? ? ? ? 83 C4 04 8B CB C7 44 24 ? ? ? ? ?  First
		static constexpr int ManagerTemplate_Turrets = 0x3139D5C; // 51 53 55 56 8B F1 8D 44 24 0C 89 35 ? ? ? ? 
		static constexpr int ManagerTemplate_Heroes = 0x18A50D0; // 83 EC 64 A1 ? ? ? ? 33 C4 89 44 24 60 8B 44 24 68 83 CA FF 
		static constexpr int ManagerTemplate_Missiles = 0x3142288; // 8D 4E 04 C7 05 ? ? ? ? ? ? ? ? 


		static constexpr int AntiCheatCheck = 0x1889E80;

		static constexpr int PingObject = 0x314A28C; // ida v23, what is that - idk
		static constexpr int ChatInstance = 0x31422E4; // 8B 0D ? ? ? ? 8A D8 85 C9
		static constexpr int IsChatOpenOffset = 0x760;

		static constexpr int W2SInstance = 0x24F282C; // https://imgur.com/a/UB8EeBO
		static constexpr int W2SInstanceOffset = 0x268;
	}

	namespace Function {
		static constexpr int GetFirstObject = 0x269980; // E8 ?? ?? ?? ?? 8B F0 85 F6 74 4D 57
		static constexpr int GetNextObject = 0x275CC0; // 8B 44 24 04 56 8B 71 18

		static constexpr int GetAttackDelay = 0x282A10; // 8B 44 24 04 51 F3
		static constexpr int GetAttackCastDelay = 0x282910; // 83 EC 0C 53 8B 5C 24 14 8B CB 56 57 8B 03 FF 90
		static constexpr int GetBoundingRadius = 0x14EF90; // 83 EC 08 56 8B F1 83 BE ? ? ? ? ? 74 69
		static constexpr int GetBasicAttack = 0x14E780; // E8 ? ? ? ? 6A 40 56 8B D8 

		static constexpr int IsMinion = 0x184380; // 56 8B 74 24 08 57 BF ? ? ? ? 85 F6 74 52 // E8 ? ? ? ? 83 C4 04 84 C0 74 6C
		static constexpr int IsTurret = 0x178650; // not used
		static constexpr int IsAlive = 0x16AC50; // not used

		static constexpr int IssueOrder = 0x159940; // E8 ? ? ? ? 8D 46 11 
		static constexpr int CastSpell = 0x5E4130; // not used

		static constexpr int OnProcessSpell = 0x517FB0; // E8 ? ? ? ? 8B 9D ? ? ? ? 89 44 24 28 -- pattern not working
		// E8 ? ? ? ? 8B AF ? ? ? ? 89 44 24 24 -- pattern not working
		// E8 ? ? ? ? 85 C0 0F 94 44 24 ?		-- pattern work

		static constexpr int PrintChat = 0x5DFEF0; // E8 ? ? ? ? 6A 00 68 ? ? ? ? E8 ? ? ? ? 83 C4 04
		static constexpr int GetPing = 0x34F9A0; // 8B C8 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 4D E0

		static constexpr int WorldToScreen = 0xA86470; //8B 11 8D 41 04 50 
		// E8 ? ? ? ? 83 C4 08 83 F8 01 75 16 
		// E8 ? ? ? ? 83 C4 08 84 C0 75 3D 

		static constexpr int CharacterDataStack__Update = 0x105ED0; // 83 EC 1C 56 57 8D 44 24 ? 8B F1 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50
		static constexpr int CharacterDataStack__Push = 0x116BE0; // 83 EC 74 53 55 56 57 8B F9 8B 47 04 39 07

		static constexpr int IsNotWall = 0xA19FF0; // E8 ? ? ? ? 33 C9 83 C4 0C 84
	}

	namespace SpellSlot {
		static constexpr int Level = 0x1C;
		static constexpr int Time = 0x24;
		static constexpr int Charges = 0x54;
		static constexpr int TimeCharge = 0x74;
		static constexpr int Damage = 0x94;
		static constexpr int SpellInfo = 0x120; // 0x144				
		static constexpr int SmiteTimer = 0x64;
		static constexpr int SmiteCharges = 0x54; //0x58;
	}

	namespace SpellData {
		static constexpr int SpellName = 0x6C; // 0x78;
		static constexpr int CooldownTime = 0x0288;

		static constexpr int MissileName = 0x78;
	}

	namespace SpellInfo {
		static constexpr int SpellData = 0x40;
		static constexpr int Slot = 0x4;
		static constexpr int StartTime = 0x8;
		static constexpr int SpellIndex = 0xC;
		static constexpr int Level = 0x58;
		static constexpr int source_id = 0x64;
		static constexpr int SourceNetworkID = 0x6C;
		static constexpr int StartPosition = 0x7C;
		static constexpr int EndPosition = 0x88;
		static constexpr int EndPosition2 = 0x94;
		static constexpr int HasTarget = 0xB4;
		static constexpr int TargetID = 0x74;
	}

	namespace MissileData {
		static constexpr int SpellInfo = 0x0260;
		static constexpr int SrcIdx = 0x2DC;
		static constexpr int DestIdx = 0x330;
		static constexpr int StartPos = 0x02DC;
		static constexpr int EndPos = 0x02E8;
	}

	namespace Minimap {
		static constexpr int Object = 0x3109FEC; // 55 8B EC 83 E4 F0 81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 8B 0D ? ? ? ? 0F 57 C0 8B 45 08 89 44 24 08 53 56 57 0F 11 84 24 ? ? ? ? 0F 11 84 24 ? ? ? ? 85 C9 74 1D 8D 84 24 ? ? ? ? 50 E8 ? ? ? ? 0F 10 84 24 ? ? ? ?  mov     ecx, dword_329A27C

		static constexpr int ObjectHud = 0x120;
		static constexpr int HudPos = 0x44;
		static constexpr int HudSize = 0x4C;
		static constexpr int HudWorldSize = 0x18;
	}

	namespace AIManager
	{
		static constexpr int NavStart = 0x1cc;
		static constexpr int NavEnd = 0x224;
		static constexpr int ServerPos = 0x2EC;
		static constexpr int Velocity = 0x2F8;
		static constexpr int Moving = 0x1C0;
		static constexpr int CurrentSegment = 0x1c4;
		static constexpr int Dashing = 0x214;
		static constexpr int DashingSpeed = 0x1f8;
		static constexpr int NavArray = 0x1e4;
		static constexpr int NavArrayEnd = 0x1e8;
	};

	namespace GameObject {
		static constexpr int Index = 0x08; // 12.12
		static constexpr int NetworkID = 0xB4;
		static constexpr int TeamID = 0x34; // 12.12
		static constexpr int Position = 0x1DC; // 12.12
		static constexpr int SpellBook = 0x2950; // 12.12
		static constexpr int Visibility = 0x274; // 12.12
		static constexpr int Dead = 0x218; // Real Dead offset - 0x4
		static constexpr int Health = 0xE74; // 12.12
		static constexpr int Shield = 0xEB4;  // 11.23
		static constexpr int Targetable = 0xD04; // 12.12
		static constexpr int Invulnearable = 0x3D4; // 12.12
		static constexpr int MaxHealth = 0xE84; // 12.12
		static constexpr int Mana = 0x29C; // 12.12
		static constexpr int MaxMana = 0x2AC; // 12.12
		static constexpr int Armor = 0x137C; // 12.12
		static constexpr int BonusArmor = 0x1380; // 12.12
		static constexpr int AttackRange = 0x139C; // 12.12
		static constexpr int BaseAttackDamage = 0x1354; // 12.12
		static constexpr int BonusAttackDamage = 0x12CC; // 12.12
		static constexpr int AttackSpeedMulti = 0x1350; // 12.12
		static constexpr int MoveSpeed = 0x1394; // 12.12
		static constexpr int ChampionName = 0x2D3C; // 12.12
		static constexpr int CombatType = 0x20A8; // 12.12

		static constexpr int MagicPenFlat = 0x1270;
		static constexpr int ArmorPen = MagicPenFlat + 0x4;
		static constexpr int MagicPenMod = MagicPenFlat + 0x8;
		static constexpr int Lethality = MagicPenFlat + 0x1C;

		static constexpr int CharacterDataStack = 0x2D30; // 8D 8E ? ? ? ? FF 74 24 4C

		static constexpr int RecallState = 0xD90;


		//static constexpr int MinionPos = 0x34C; // 11.23

		//static constexpr int Armor = 0x1294; // 11.23

		static constexpr int MagicRes = 0x1384; // 11.23


		static constexpr int AbilityPower = 0x1750; // 11.23
		static constexpr int AbilityPowerMult = 0x1760; // 11.23

		//static constexpr int MagicPenFlat = 0x118C;
		//static constexpr int ArmorPen = MagicPenFlat + 0x4;
		//static constexpr int MagicPenMod = MagicPenFlat + 0x8;
		//static constexpr int Lethality = MagicPenFlat + 0x1C;

		//static constexpr int BonusAtk = 0x11E4; // 11.23
		//static constexpr int BaseAtk = 0x126C; // 11.23
		//static constexpr int AtkRange = 0x12B4; // 11.23




		//static constexpr int Nickname = 0x0054; // 11.23

		//static constexpr int Level = 0x329C; // 11.23
		//static constexpr int Summoner1 = 0x3760; // Pointer to First Summoner  // 12.5
		//static constexpr int Summoner2 = 0x376C; // Pointer to Second Summoner // 12.5

		//static constexpr int SpellBookSpellSlots = 0x488;
	}

	namespace BuffManager {
		//https://www.unknowncheats.me/forum/3473574-post6910.html
		static constexpr int Instance = 0x2310;

		static constexpr int ArrayStart = 0x10;
		static constexpr int ArrayEnd = 0x14;
		static constexpr int BuffHash = 0x88;
		static constexpr int BuffScript = 0x8;
		static constexpr int BuffEntryBuff = 0x8;
		static constexpr int BuffType = 0x4;
		static constexpr int StartTime = 0xC;
		static constexpr int EndTime = 0x10;
		static constexpr int BuffCount = 0x74;
		static constexpr int BuffCountAlt = 0x24;
		static constexpr int BuffCountAlt2 = 0x28;
		static constexpr int BuffName = 0x8;
		static constexpr int BuffName2 = 0x4;
		static constexpr int BuffEntryBuffNodeStart = 0x20;
		static constexpr int BuffEntryBuffNodeCurrent = 0x24;

	}

	namespace Inventory
	{
		static constexpr int Instance = 0x3568;
		static constexpr int ItemSlotStart = 0x8;
		static constexpr int ItemSlot = 0xC;
		static constexpr int ItemSlotSpellName = 0x10;
		static constexpr int ItemSlotInfo = 0x20;
		static constexpr int ItemSlotStacks = 0x2C;
		static constexpr int ItemSlotPrice = 0x28;
		static constexpr int ItemSlotInfoId = 0x68;
		static constexpr int ItemSlotInfoPercentCritChanceMod = 0x160;
	}

}