#pragma once

namespace Offset {
	namespace Data {
		static constexpr int LocalPlayer = 0x3143DA0; // 51 8B 0D ? ? ? ? 85 C9 74 21 
		static constexpr int GameTime = 0x313D244; // F3 0F 11 05 ? ? ? ? 8B 49
		static constexpr int ObjectManager = 0x18A6ED8; // A1 ?? ?? ?? ?? C7 40 ?? ?? ?? ?? ?? C3
		static constexpr int HudInstance = 0x18A6FA4; //8B ? ? ? ? ? 6A 00 8B ? ? E8 ? ? ? ? B0 01 ?
		static constexpr int Camera = 0x24F4600; // E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8D 43 24

		static constexpr int ViewProjMatrices = 0x31696A0; // B9 ? ? ? ? 0F 10 00 

		static constexpr int ManagerTemplate_Minions = 0x24F45E0; // 8B 0D ? ? ? ? E8 ? ? ? ? EB 09 56 E8 ? ? ? ? 83 C4 04 8B CB C7 44 24 ? ? ? ? ?  First
		static constexpr int ManagerTemplate_Turrets = 0x313BA44; //  8B 35 ? ? ? ? 8B 76 18  // 51 53 55 56 8B F1 8D 44 24 0C 89 35 ? ? ? ? 
		static constexpr int ManagerTemplate_Heroes = 0x18A6F70; // 83 EC 64 A1 ? ? ? ? 33 C4 89 44 24 60 8B 44 24 68 83 CA FF 
		static constexpr int ManagerTemplate_Missiles = 0x3143DF0; // 8D 4E 04 C7 05 ? ? ? ? ? ? ? ? 

		//int Offsets::HeroList = 0x18A0014; //8B 15 ? ? ? ? 0F 44 C1
		//int Offsets::MinionList = 0x24ED788; //A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 85 C0 74 32
		//int Offsets::TurretList = 0x3134C94; //8B 35 ? ? ? ? 8B 76 18
		//int Offsets::InhibList = 0x313D514; //A1 ?? ?? ?? ?? 53 55 56 8B 70 04 8B 40 08

		//static constexpr int InhibList = 0x313D514; //A1 ?? ?? ?? ?? 53 55 56 8B 70 04 8B 40 08

		static constexpr int AntiCheatCheck = 0x188BCA0;

		static constexpr int PingObject = 0x314868C; // ida v23, what is that - idk
		static constexpr int ChatInstance = 0x3143E4C; // 8B 0D ? ? ? ? 8A D8 85 C9
		static constexpr int IsChatOpenOffset = 0x6D8;

		static constexpr int W2SInstance = 0x25095AC; // https://imgur.com/a/UB8EeBO
		static constexpr int W2SInstanceOffset = 0x268; // OFFSET, W2SInstanceOffset, "FF 74 24 3C 8D 89 ? ? ? ?", 6, 2

		//static constexpr int SendPingInstance = 0x3147424;
	}

	namespace Function {
		//static constexpr int GetFirstObject = 0x269980; // E8 ?? ?? ?? ?? 8B F0 85 F6 74 4D 57
		//static constexpr int GetNextObject = 0x275CC0; // 8B 44 24 04 56 8B 71 18

		static constexpr int GetAttackDelay = 0x292610; // 8B 44 24 04 51 F3
		static constexpr int GetAttackCastDelay = 0x292510; // 83 EC 0C 53 8B 5C 24 14 8B CB 56 57 8B 03 FF 90
		static constexpr int GetBoundingRadius = 0x15ABE0; // 83 EC 08 56 8B F1 83 BE ? ? ? ? ? 74 69
		static constexpr int GetBasicAttack = 0x15A320; // E8 ? ? ? ? 6A 40 56 8B D8 

		static constexpr int IsMinion = 0x1A1500; // 56 8B 74 24 08 57 BF ? ? ? ? 85 F6 74 52 // E8 ? ? ? ? 83 C4 04 84 C0 74 6C
		static constexpr int IsTurret = 0x1A15F0; // E8 ? ? ? ? 83 C4 04 84 C0 75 7A
		//static constexpr int IsAlive = 0x16AC50; // not used

		static constexpr int IssueOrder = 0x166F70; // E8 ? ? ? ? 8D 46 11 
		static constexpr int GetBaseDrawPosition = 0x154370; // E8 ?? ?? ?? ?? EB ?? 8B 01 8B 40

		static constexpr int NewCastSpell = 0x641400; // 53 8B 1D ?? ?? ?? ?? 55 56 57 8B F9 85

		static constexpr int OnProcessSpell = 0x51CCE0;
		// E8 ? ? ? ? 8B 9D ? ? ? ? 89 44 24 28 -- pattern not work
		// E8 ? ? ? ? 8B AF ? ? ? ? 89 44 24 24 -- pattern not working
		// E8 ? ? ? ? 85 C0 0F 94 44 24 ?		-- pattern  working
		// E8 ? ? ? ? 8B 9D ? ? ? ? 89 44 24 28 -- pattern work

		static constexpr int PrintChat = 0x60D740; // E8 ? ? ? ? 6A 00 68 ? ? ? ? E8 ? ? ? ? 83 C4 04
		static constexpr int SendChat = 0x65A8D0; // A1 ? ? ? ? 56 6A FF
		static constexpr int GetPing = 0x338DB0; // 8B C8 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 4D E0    // E8 ? ? ? ? 8D 7C 24 20 
		static constexpr int SendPing = 0x646A50; //E8 ? ? ? ? 5F 5E 32 C0 5B 83 C4 10


		static constexpr int WorldToScreen = 0xAA3FF0; //8B 11 8D 41 04 50 
		// E8 ? ? ? ? 83 C4 08 83 F8 01 75 16 
		// E8 ? ? ? ? 83 C4 08 84 C0 75 3D 

		static constexpr int CharacterDataStack__Update = 0x667E70; // 83 EC 1C 56 57 8D 44 24 ? 8B F1 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50 8D 44 24 ? 50
		static constexpr int CharacterDataStack__Push = 0x11ABA0; // 83 EC 74 53 55 56 57 8B F9 8B 47 04 39 07

		static constexpr int IsNotWall = 0xA21BF0; // E8 ? ? ? ? 33 C9 83 C4 0C 84
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
		static constexpr int Instance = -0x4C8;
	}

	namespace SpellData {
		static constexpr int SpellName = 0x6C; // 0x78;
		static constexpr int CooldownTime = 0x0288;

		static constexpr int MissileName = 0x6C;//0x78;
		static constexpr int SpellCost = 0x52C;
	}

	namespace SpellInfo {
		static constexpr int SpellData = 0x40;
		static constexpr int Slot = 0x4;
		static constexpr int StartTime = 0x8;
		static constexpr int SpellIndex = 0xC;
		static constexpr int Index = 0x70;
		static constexpr int Level = 0x5C;
		static constexpr int source_id = 0x64;
		static constexpr int SourceNetworkID = 0x70;
		static constexpr int StartPosition = 0x80;
		static constexpr int EndPosition = 0x8C;
		static constexpr int EndPosition2 = 0x98;

		static constexpr int HasTarget = 0xB4;
		static constexpr int TargetID = 0x74;
	}

	namespace MissileData {
		static constexpr int SpellInfo = 0x0260;
		static constexpr int SrcIdx = 0x2C4; //0x2DC;
		static constexpr int DestCheck = 0x31C;
		static constexpr int DestIdx = 0x318;// 0x330;
		static constexpr int StartPos = 0x2E0;
		static constexpr int CurPos = 0x1DC;
		static constexpr int EndPos = 0x2EC;
	}

	namespace Minimap {
		static constexpr int Object = 0x313D288; // 55 8B EC 83 E4 F0 81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 8B 0D ? ? ? ? 0F 57 C0 8B 45 08 89 44 24 08 53 56 57 0F 11 84 24 ? ? ? ? 0F 11 84 24 ? ? ? ? 85 C9 74 1D 8D 84 24 ? ? ? ? 50 E8 ? ? ? ? 0F 10 84 24 ? ? ? ?  mov     ecx, dword_329A27C

		static constexpr int ObjectHud = 0x150; // 0x128;// 0x120;
		static constexpr int HudPos = 0x3C;// 0x44;
		static constexpr int HudSize = 0x44;// 0x4C;
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
		static constexpr int NavArraySize = 0x1e8;
		static constexpr int NavArrayEnd = 0x1e8;
	};

	namespace GameObject {
		static constexpr int Index = 0x08; // 12.12
		static constexpr int NetworkID = 0xB4;
		static constexpr int TeamID = 0x34; // 12.12
		static constexpr int Position = 0x1DC; // 12.12
		static constexpr int Direction = 0x1BE0;
		static constexpr int SpellBook = 0x29C0; // 12.12 // 8B 84 83 ? ? ? ? EB 06 8B 83 ? ? ? ? 85 C0 0F 84 ? ? ? ? 53 8B CF E8 ? ? ? ? 8B C8 8B 10 FF 52 18 8B F0
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
		static constexpr int ChampionName = 0x2DAC; // 12.12
		static constexpr int CombatType = 0x20A8; // 12.12

		static constexpr int MagicPenFlat = 0x1270;
		static constexpr int ArmorPen = MagicPenFlat + 0x4;
		static constexpr int MagicPenMod = MagicPenFlat + 0x8;
		static constexpr int Lethality = MagicPenFlat + 0x1C;

		static constexpr int CharacterDataStack = 0x2DA0; // 8D 8E ? ? ? ? FF 74 24 4C

		static constexpr int RecallState = 0xD90;
		static constexpr int SkinPtr = 0x2D84; // 12.12


		//static constexpr int MinionPos = 0x34C; // 11.23

		static constexpr int MagicRes = 0x1384; // 11.23


		static constexpr int AbilityPower = 0x1750; // 11.23
		static constexpr int AbilityPowerMult = 0x1760; // 11.23

	}

	namespace BuffManager {
		//https://www.unknowncheats.me/forum/3473574-post6910.html
		static constexpr int Instance = 0x2330;

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
		static constexpr int Instance = 0x35E8;
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