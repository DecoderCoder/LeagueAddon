#pragma once
#include "../ObjectManager.h"
#include "../EventManager/EventManager.h"
#include "../Render.h"
//#include "../Console/Console.h"
#include "../Geometry.h"
#include "../Hooks/DirectX.h"
#include <unordered_map>

#define MathHuge 999999.f
#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

namespace JustEvade {
	enum class SpellType {
		linear,
		threeway,
		rectangular,
		circular,
		conic,
		polygon
	};

	enum class CastType : int
	{
		Position,
		Target,
		Self
	};

	enum class EvadeType {
		Blink,
		Dash,
		Invulnerability,
		MovementSpeedBuff,
		SpellShield,
		SafeTarget,
		Windwall
	};

	enum class SpellSlot : int
	{
		Unknown = -1,
		Q = 0,
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

	class Spell {
	public:
		string name;
		string missileName;
		string icon;
		string displayName;
		SpellSlot slot;
		SpellType type;

		float startY;
		float endY;

		Vector3 startPos;
		Vector3 endPos;
		Vector3 endPos2;
		bool useEndPos2;
		Vector3 currentPos;

		int spellIndex;
		SpellData* basicAttackSpellData;
		int sourceNetworkId;
		int source_id;


		Geometry::Polygon path;
		GameObject* obj;
		MissileSpellInfo* missileObject;
		float startTime;

		float speed;
		float range;
		float radius;
		float delay;
		float angle;
		int danger;
		bool cc;
		bool collisionWalls;
		bool collisionMinions;
		bool collisionChamps;
		bool windwall;
		bool hitbox;
		bool fow;
		bool exception;
		bool extend;
		

		// Custom settings
		bool Dodge;
		bool Evade;
		bool Draw = true;
		bool Force;
		bool FOW = true;
		int HP = 5;
		int ER;

		void SetMissileObject(MissileSpellInfo* missile) {
			this->missileObject = missile;
		}
	};

	class Vertex {
	public:
		float x;
		float y;
		float alpha = 0;
		Vertex* next = NULL;
		Vertex* prev = NULL;
		Vertex* neighbor = NULL;
		bool visited = false;
		bool intersection = false;

		Vertex(float x, float y, float alpha, bool intersection) {
			this->x = x;
			this->y = y;
			this->alpha = alpha;
			this->intersection = intersection;
		}

		Vertex* New(float x, float y, float alpha, bool intersection) {
			this->x = x;
			this->y = y;
			this->alpha = alpha;
			this->intersection = intersection;
			return this;
		}

		void InitLoop() {
			auto last = this->GetLast();
			last->prev->next = this;
			this->prev = last->prev;
		}

		void Insert(Vertex* first, Vertex* last) {
			auto res = first;
			while (res != last && res->alpha < this->alpha) {
				res = res->next;
			}
			this->next = res;
			this->prev = res->prev;
			if (this->prev)
				this->prev->next = this;
			this->next->prev = this;
		}

		Vertex* GetLast() {
			auto res = this;
			while (res->next && res->next != this) {
				res = res->next;
			}
			return res;
		}

		Vertex* GetNextNonIntersaction() {
			auto res = this;
			while (res && res->intersection) {
				res = res->next;
			}
			return res;
		}

		Vertex* GetFirstVertexOfIntersection() {
			auto res = this;
			while (true) {
				res = res->next;
				if (!res)
					break;
				if (res == this)
					break;
				if (res->intersection && !res->visited)
					break;
			}
			return res;
		};
	};

	class Champ {
	public:
		string Name;
		GameObject* obj = NULL;
		std::list<Spell> Spells;
	};

	namespace Core {
		inline float GameTimer;

		inline Vector3 SafePos;

		inline std::list<Spell> DetectedSpells;
		inline std::list<Spell> DodgeableSpells;

		inline std::list<Champ> SpellDB;

		inline std::list<Champ> ChampsInGame;

		inline bool SmoothEvade;
		inline bool LimitRange;
		inline int CQ = 16; // Circle Segments Quality
		inline int DS = 20; // Diagonal Search Step
		inline int DC = 4; // Diagonal Points Count
		inline int LR = 5250; // Limited Detection Range



		inline bool EvadeSpells;
		inline bool DodgeSpells;
		inline bool DrawSpells;
		inline bool DrawStatus;

		inline bool MissileDetection;

		inline bool Debug;
		inline bool DrawSafePosition;



		inline bool DoD = false; // Dodge only dangerous
		inline bool Evading = false;
		inline bool InsidePath = false;
		inline bool Loaded = false;

		inline Vector3 ExtendedPos;
		inline SpellSlot Flash;
		inline SpellSlot Flash2;
		inline float FlashRange;
		inline Vector2 MousePos;
		inline Vector3 MyHeroPos;

		void Initialize();
		void OnDraw();
		void OnMenu();
		void OnTick();

		bool IsValid(Spell& s);

		void DetectMissiles();

		void SpellManager();
		std::list<Spell> GetDodgeableSpells();
		void SpellExistsThenRemove(string name);
		Vector3 CalculateEndPos(Spell& spellInfo);
		Geometry::Polygon GetPath(Spell& spell, bool boundingRadius = true);
		void OnProcessSpell(void* spellBook, SpellInfo* castInfo);
		void AddSpell(Spell& spell);
	};

	void InitSpellsDB();
}