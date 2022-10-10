#pragma once
#include "../../ObjectManager.h"
#include "../../EventManager/EventManager.h"
#include "../../Render.h"
//#include "../Console/Console.h"
#include "../../Geometry.h"
#include "../../Hooks/DirectX.h"
#include <unordered_map>
#define MathHuge 999999.f

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

namespace Evade
{

	enum class CollisionObjectType : int
	{
		Champion,
		Minion,
		YasuoWall
	};

	enum class EvadeType : int
	{
		Blink,
		Dash,
		Invulnerability,
		MovementSpeedBuff,
		SpellShield,
		SafeTarget,
		Windwall
	};

	enum class CastType : int
	{
		Position,
		Target,
		Self
	};

	enum class SpellSlot : int
	{
		Unknown,
		Q,
		W,
		E,
		R
	};

	class EvadeSpellData
	{
	public:
		CastType CastType;
		std::string ChampionName;
		std::string CheckSpellName = "";
		int DangerLevel;
		int Delay;
		EvadeType EvadeType;
		bool FixedRange;
		bool IsBehindTarget;
		bool IsInfrontTarget;
		bool IsItem;
		bool IsReversed;
		bool IsSpecial;
		bool IsSummonerSpell;
		int ItemId;
		int MaxRange;
		int MinRange;
		float MovementSpeed;
		std::string Name;
		SpellSlot Slot = SpellSlot::Unknown;
		int Speed;

	};

	class PositionInfo
	{
	public:

		PositionInfo(Vector3 position, int posDangerLevel, int posDangerCount, bool isDangerousPos, float distanceToMouse, std::list<int> dodgeableSpells, std::list<int> undodgeableSpells)
		{
			this->position = position;
			this->posDangerLevel = posDangerLevel;
			this->posDangerCount = posDangerCount;
			this->isDangerousPos = isDangerousPos;
			this->distanceToMouse = distanceToMouse;
			this->dodgeableSpells = dodgeableSpells;
			this->undodgeableSpells = undodgeableSpells;
			this->timestamp = Function::GameTimeTick();
		}

		PositionInfo(Vector3 position, bool isDangerousPos, float distanceToMouse)
		{
			this->position = position;
			this->isDangerousPos = isDangerousPos;
			this->distanceToMouse = distanceToMouse;
			this->timestamp = Function::GameTimeTick();
		}


		Vector3 position;
		int posDangerLevel = 0;
		int posDangerCount = 0;
		bool isDangerousPos = false;
		float distanceToMouse = 0.f;
		std::list<int> dodgeableSpells;
		std::list<int> undodgeableSpells;
		std::list<int> spellList;
		float endTime = 0.f;
		bool hasExtraDistance = false;
		float closestDistance = FLT_MAX;
		float intersectionTime = FLT_MAX;
		bool rejectPosition = false;
		float posDistToChamps = FLT_MAX;
		bool hasComfortZone = true;
		GameObject* target = NULL;
		bool recalculatedPath = false;
		float speed = 0.f;
		float timestamp;
	};

	class Track
	{
	public:
		Track(Vector3 P0, Vector3 v)
		{
			this->P0 = P0;
			this->v = v;
		}
		Vector3 P0;
		Vector3 v;
	};

	/*class MathUtils
	{
	public:
		 bool CheckLineIntersection(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
		 std::tuple<float, float> LineToLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
		 float GetCollisionDistanceEx(Vector3 Pa, Vector3 Va, float Ra, Vector3 Pb, Vector3 Vb, float Rb, Vector3& PA, Vector3& PB);
		 float GetCollisionTime(Vector3 Pa, Vector3 Pb, Vector3 Va, Vector3 Vb, float Ra, float Rb, bool& collision);
	};
	class MathUtilsCPA
	{
	public:
		 float cpa_time(Track Tr1, Track Tr2);
		 float CPAPoints(Vector3 p1, Vector3 v1, Vector3 p2, Vector3 v2, Vector3 ret1, Vector3 ret2);
		 float CPAPointsEx(Vector3 p1, Vector3 v1, Vector3 p2, Vector3 v2, Vector3 p1end, Vector3 p2end);
		 float CPAPointsEx(Vector3 p1, Vector3 v1, Vector3 p2, Vector3 v2, Vector3 p1end, Vector3 p2end, Vector3 p1out, Vector3 p2out);
	};*/

	namespace Core {
		enum SpellType
		{
			linear,
			polygon,
			circular,
			conic,
			rectangular,
			threeway,
			arc,
			ring,
			zone
		};

		enum SlotID :int
		{
			_Q = 0,
			_W = 1,
			_E = 2,
			_R = 3,
			_D = 4,
			_F = 5
		};

		struct Spell
		{
			std::string name;
			std::string icon;
			std::string displayName;
			std::string missileName;
			bool nameStrictly = false;
			std::list<std::string> extraMissileNames;
			int slot = 0;
			int type = 0;
			float speed = 0.0f;
			float range = 0.0f;
			float delay = 0.0f;
			float delayBR = 0.0f; // Delay before return
			bool directionChanged = false; // Delay before return
			float time = 0.0f;
			float radius = 0.0f;
			float radiusRes = 0.0f;
			float radiusEx = 0.0f;
			float angle = 0.0f;
			int danger = 0;
			bool cc = false; // Stun, Root e.t.c
			bool collision = false;
			bool collisionWC = false; // Collision with champs
			bool windwall = false;
			bool hitbox = false;
			bool fow = false;
			bool exception = false;
			bool extend = false;
			bool enabled = true;
			bool useMissile = false;
			bool followEnemy = false; // Follow Enemy like DariusQ
			bool followEnemy2 = false; // Follow enemy like TaliyahQ
			bool followEnemy3 = false; // Follow enemy like PykeE

			// After detection
			float startTime = 0.0f;
			float endTime = 0.0f;
			Vector3 startPos;
			Vector3 endPos;
			Vector3 currPos;
			Geometry::Polygon path;
			SpellInfo* spell = nullptr;
			/*SpellInfo spellInstance;
			SpellData spellDataInstance;
			SpellDataResource SpellDataResourceInstance;*/
			GameObject* obj = nullptr;
		};

		struct Champ
		{
			std::string hero;

			GameObject* obj = NULL;

			std::vector<Spell>spells;
		};

		struct EvadeSpell
		{
			std::string name;
			std::string icon;
			std::string displayName;
			EvadeType type = EvadeType::Blink;
			int danger = 1;
			float range = 0;
			int slot = 0;
			bool use = true;
		};

		struct EvadeChamp
		{
			std::string hero;
			std::list<EvadeSpell>spells;
		};
		inline std::list< EvadeChamp>EvadeSpellDB;
		inline std::list<EvadeSpell>MyEvadeSpells;

		inline std::list<Champ>SpellDB;
		inline std::vector<Champ>ChampsInGame;

		inline std::vector<Spell>DetectedSkillshots;
		inline std::list<Spell>EnabledSkillshots;
		inline std::list<Spell>DangerSkillshots;

		inline float BoundingRadius;
		inline float GameTimer = 0;

		inline Vector3 MyHeroPos;
		inline Vector3 PreviousPos;
		inline Vector3 EvadePoint;
		inline Vector3 EvadeToPoint;

		inline bool NoSolutionFound = false;
		inline float LastClick = 0.0f;

		void InitSpells();

		void OnDraw();
		void OnMenu();
		void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
		void Initalize();


		bool CheckPosCollision();

		void StartEvading();

		void MoveToPos(Vector3 pos);
		void SetEvading(bool b);
		bool IsDangerous(Vector3 pos);

		void OnSpellCast(Spell& spell);
		Vector3 CalculateEndPos(Spell& spell);
		Geometry::Polygon GetPath(Spell& spell);

		inline bool DrawSpells = false;
		inline bool EvadeSpells = false;
		inline bool UseMissiles = false;
		inline float NewTimer = 0;
		inline float OldTimer = 0;
		inline bool Evading;
		inline Vector3 mWorldPos;
		inline Vector3 SafePos;
		inline Vector3 ExtendedPos;
		Vector3 GetMovePath();
		int CoreManager(Spell s);
		Vector3 PrependVector(Vector3 pos1, Vector3 pos2, float dist);
		void DodgeSpell();
		std::list<Vector3> FindIntersections(Geometry::Polygon poly, Vector3 p1, Vector3 p2);
		Vector3 GetExtendedSafePos(Vector3 pos);
		int Avoid(Spell spell, Vector3 dodgePos, EvadeSpell data);
		Vector3 GetBestEvadePos(std::list<Spell> spells, float radius, int mode);
		bool IsSafePos(Vector3 pos, int extra);
		float GetDistanceToChampions(Vector3 pos);
		bool isNearEnemy(Vector3 pos, float distance);
		float GetDistanceToMinions(Vector3 pos);
		bool isNearMinion(Vector3 pos, float distance);
		float GetDistanceToTurrets(Vector3 pos);
		float GetPositionValue(Vector3 pos);
		bool sortBest(Vector3 a, Vector3 b);
		// PositionInfo InitPositionInfo(Vector3 pos, float extraDelayBuffer, float extraEvadeDistance, Vector3 lastMovePos, Spell lowestEvadeTimeSpell);
		// float GetClosestDistanceApproac(Spell spell, Vector3 pos, float speed, float delay, Vector3 heroPos, float extraDist);
		bool IsAboutToHit(Spell& spell, Vector3 pos1, int extra);
		float GetMovementSpeed(bool extra, EvadeSpell evadeSpell);

		bool IsValid(Spell& spell);

		inline std::string SlotToName(int slot)
		{
			switch (slot)
			{
			case 0:
				return "[Q]";
			case 1:
				return "[W]";
			case 2:
				return "[E]";
			case 3:
				return "[R]";
			case 4:
				return "[D]";
			case 5:
				return "[F]";
			}
			return "";
		}

	};
};