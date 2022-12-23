#include "JustEvade.h"

#define _DEBUGEVADE true

namespace JustEvade {

	float BoundingRadius;
	std::list<int> addedSpells;
	Geometry::Polygon mapPoly;


	void Core::Initialize()
	{
		InitSpellsDB();

		for (GameObject* hero : ObjectManager::HeroList())
		{
#if !_DEBUGEVADE
			if (Local->IsAllyTo(hero) || hero->NetworkID == Local->NetworkID)
				continue;
#endif
			/*ChampsInGame.push_back((Champ)SpellDB.back());*/
			for (Champ champ : SpellDB)
			{
				if (hero->GetChampionName() == champ.Name)
				{
					champ.obj = hero;
					ChampsInGame.emplace_back(champ);
				}
			}
		}

		BoundingRadius = Function::GetBoundingRadius(Local);
		mapPoly.Add(Vector3(0, 0, 0));
		mapPoly.Add(Vector3(15000, 0, 0));
		mapPoly.Add(Vector3(15000, 0, 15000));
		mapPoly.Add(Vector3(0, 0, 15000));
		EventManager::AddEventHandler(EventManager::EventType::OnMenu, Core::OnMenu);
		EventManager::AddEventHandler(EventManager::EventType::OnDraw, Core::OnDraw);
		EventManager::AddEventHandler(EventManager::EventType::OnProcessSpell, Core::OnProcessSpell);
	}

	void Core::OnMenu() {
		if (ImGui::CollapsingHeader("JustEvade")) {
			if (ImGui::TreeNode("Main Settings")) {
				ImGui::Checkbox("Enable Evade", &Core::EvadeSpells);
				ImGui::Checkbox("Dodge Spells", &Core::DodgeSpells);
				ImGui::Checkbox("Dodge only Dangerous", &Core::DoD);
				ImGui::Checkbox("Draw Spells", &Core::DrawSpells);
				ImGui::Checkbox("Draw Status", &Core::DrawStatus);

				ImGui::Checkbox("Enable Missile Detection", &Core::MissileDetection);

				ImGui::Checkbox("Draw Safe Position", &DrawSafePosition);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Spell Settings")) {
				for (auto& champ : ChampsInGame) {
					if (ImGui::TreeNode(champ.Name.c_str())) {
						for (auto& spell : champ.Spells) {
							ImGui::SetNextItemWidth(50.0f);
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 11));

							auto image = Render::Images_Manager.GetImageByName(spell.icon);
							if (!nullptr) {
								image = Render::Images_Manager.GetImageByName(spell.name);
							}

							ImGui::Image(image, ImVec2(22, 22));
							ImGui::PopStyleVar();
							ImGui::SameLine();
							if (ImGui::TreeNode(("[" + QWERDF[(int)spell.slot] + "] " + spell.name).c_str())) {
								if (ImGui::TreeNode("Info")) {
									ImGui::PushStyleColor(ImGuiCol_Text, spell.cc ? IM_COL32(231, 76, 60, 255) : IM_COL32(46, 204, 113, 255));
									ImGui::Text(spell.cc ? "Has CC: Yes" : "Has CC: No");
									ImGui::PopStyleColor();
									ImGui::TreePop();
								}
								ImGui::Checkbox("Evade Spell", &spell.Evade);
								ImGui::Checkbox("Dodge Spell", &spell.Dodge);
								ImGui::Checkbox("Draw Spell", &spell.Draw);
								ImGui::Checkbox("Force To Dodge", &spell.Force);

								if (spell.fow) {
									ImGui::Checkbox("FOW Detection", &spell.FOW);
								}

								ImGui::SliderInt("HP", &spell.HP, 0, 100);
								ImGui::SliderInt("Danger", &spell.danger, 1, 5);

								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Evading Spells")) {
				ImGui::Checkbox("Use Flash", &Core::EvadeSpells);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Core Settings")) {
				ImGui::Checkbox("Enable Smooth Evading", &Core::SmoothEvade);

				ImGui::Checkbox("Limit Detection Range", &Core::LimitRange);
				ImGui::SliderInt("Limited Detection Range", &Core::LR, 500, 15000);
				ImGui::SliderInt("Circle Segments QUality", &Core::CQ, 10, 25);
				ImGui::SliderInt("Diagonal Search Step", &Core::DS, 5, 100);
				ImGui::SliderInt("Diagonal Points Count", &Core::DC, 1, 8);
				ImGui::TreePop();
			}
		}
	}

	void DrawArrow(Vector3 startPos, Vector3 endPos, ImColor color) {
		auto p1 = endPos - ((startPos - endPos).Normalized() * 30).Perpendicular() + (startPos - endPos) * 30;
		auto p2 = endPos - ((startPos - endPos).Normalized() * 30).Perpendicular2() + (startPos - endPos) * 30;

		Vector2 sP = Function::WorldToScreen(&startPos), eP = Function::WorldToScreen(&endPos), sP1 = Function::WorldToScreen(&p1), sP2 = Function::WorldToScreen(&p2);
		Render::Draw_Line(sP, eP, color, 1);
		Render::Draw_Line(p1, eP, color, 1);
		Render::Draw_Line(p2, eP, color, 1);
	}

	bool IsMoving() {
		AIManager* ai = Local->GetAIManager();
		return ai->Moving;
	}

	Vector3 GetMovePath() {
		AIManager* ai = Local->GetAIManager();
		return ai->NavEndPos;
	}

	void Core::OnTick() {
		if (!Core::EvadeSpells)
			return;
		//self.DoD = self.JEMenu.Main.DD:Value() == true

		if (!Function::IsAlive(Local))
			return;


		auto movePath = GetMovePath();
		if (IsMoving && !Core::Evading) {

		}
	}

	bool alreadyMissileAssigned(MissileSpellInfo* missile) {
		auto it = Core::DetectedSpells.begin();
		while (it != Core::DetectedSpells.end()) {
			if (it->missileObject == missile) {
				return true;
			}
			++it;
		}
		return false;
	}

	void Core::DetectMissiles() {
		if (Core::MissileDetection)
			for (MissileSpellInfo* missile : ObjectManager::MissileList()) {
				//MessageBoxA(0, to_hex(missile).c_str(), "", 0);
				auto source = ObjectManager::FindObjectByIndex(ObjectManager::MinionList(), missile->source_id);
				if (source)
					continue;
				source = ObjectManager::FindObjectByIndex(ObjectManager::HeroList(), missile->source_id);
#if !_DEBUGEVADE

				if (source && source->IsEnemyTo(Local)) // for sylas
#endif
					for (Champ& champ : Core::ChampsInGame)
					{
						for (Spell& s : champ.Spells)
						{

							if (Compare(missile->BasicAttackSpellData->Name, s.missileName, true)) // && missile->source_id
							{

								auto it = addedSpells.begin();
								bool found = false;
								while (it != addedSpells.end()) {
									if ((*it) == missile->Index) {
										found = true;

										auto ds = Core::DetectedSpells.begin();
										while (ds != Core::DetectedSpells.end()) {
											if (!ds->missileObject && missile->source_id == ds->source_id && Compare(ds->name, missile->BasicAttackSpellData->Name, true) && !alreadyMissileAssigned(missile)) {
												//MessageBoxA(0, ("Found missile\nmissile->source_id: " + to_string(missile->source_id) + "\nsp.source_id: " + to_string(sp.source_id)).c_str(), "", 0);
												ds->missileObject = missile;
												break;
											}
											else {
												++ds;
											}
										}
										break;
									}
									else {
										++it;  // go to next
									}
								}
								if (!found) {
									Spell newSpell = s;

									newSpell.startTime = GameTimer;
									newSpell.obj = champ.obj;

									newSpell.spellIndex = missile->Index;
									newSpell.basicAttackSpellData = missile->BasicAttackSpellData;

									newSpell.startY = missile->StartPosition.y;
									newSpell.endY = missile->EndPosition.y;
									newSpell.startPos = missile->StartPosition;
									newSpell.endPos = missile->EndPosition;
									newSpell.endPos2 = missile->EndPosition2;
									newSpell.sourceNetworkId = missile->SourceNetworkID;
									newSpell.source_id = missile->source_id;
									newSpell.missileObject = missile;

									//s.speed = missile->BasicAttackSpellData->Resource->MissileSpeed == 0 ? MathHuge : missile->BasicAttackSpellData->Resource->MissileSpeed;

									Core::AddSpell(s);
									addedSpells.push_back(missile->Index);
								}
							}
						}
					}
			}
	}

	inline bool InMap(Vector3 pos) {
		return mapPoly.IsInside(pos);
	}

	bool Core::IsValid(Spell& s)
	{
		if (s.path.Points.size() > 0)
		{
			//MessageBoxA(0, (
			//	"StartTime: " + to_string(s.startTime) + "\n" +
			//	"range: " + to_string(s.range) + "\n" +
			//	"speed: " + to_string(s.speed) + "\n" +
			//	"delay: " + to_string(s.delay) + "\n" +
			//	"(s.startTime + s.range / s.speed + s.delay): " + to_string((s.startTime + s.range / s.speed + s.delay)) + "\n" +
			//	"GameTime: " + to_string(GameTimer) + "\n"
			//	).c_str(), "IsValid", 0);

			if ((s.startTime + s.range / s.speed + s.delay) > GameTimer)
			{
				//if (s.speed != MathHuge && s.startTime + s.delay < GameTimer)
				{
					if (s.type == SpellType::linear || s.type == SpellType::threeway)
					{
						s.endPos = CalculateEndPos(s);

						float rng = s.speed * (GameTimer - s.startTime - s.delay);
						Vector3 sP = s.startPos.Extend(s.endPos, rng);
						if (s.type == SpellType::linear && s.missileObject) {
							sP = Vector3(s.missileObject->CurrentPosition.x, 0, s.missileObject->CurrentPosition.z);
							if (!InMap(s.currentPos)) {
								return false;
							}
							else {
								auto found = ObjectManager::FindObjectByIndex(ObjectManager::MissileList(), s.missileObject->Index);
								if (!found)
									return false;
							}
						}
						else
							s.currentPos = sP;
						s.path = Geometry::Rectangle(sP, s.endPos, s.radius).ToPolygon().SetY(s.startY);
					}
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		return false;
	}

	int GetHealthPercent() {
		return Local->Health / Local->MaxHealth * 100;
	}

	std::list<Spell> Core::GetDodgeableSpells() {
		std::list<Spell> result;

		if (!Core::DodgeSpells)
			return result;
		auto it = Core::DetectedSpells.begin();
		while (it != Core::DetectedSpells.end()) {
			Spell s = *it;
			if (s.Dodge && GetHealthPercent() < s.HP) {
				if ((Core::DoD && s.danger >= 4) || !Core::DoD) {
					result.push_back(s);
				}
			}
			++it;
		}

		return result;
	}

	void Core::SpellManager() {
		auto it = Core::DetectedSpells.begin();
		while (it != Core::DetectedSpells.end()) {
			if (!Core::IsValid(*it) && !Core::Debug) {
				it = Core::DetectedSpells.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void Core::OnDraw() {
		GameTimer = Function::GameTime();

		DetectMissiles();
		SpellManager();

		DodgeableSpells = GetDodgeableSpells();

		OnTick();

		Render::BeginOverlay();

		if (DrawStatus) {
			Vector3 lpPos = Function::WorldToScreen(&Local->Position);
			Render::Draw_Text_Centered(lpPos.x - 15, lpPos.y - 7, 30, 15, (EvadeSpells ? (DoD ? "E: DoD" : "E: ON") : "E: OFF"), (EvadeSpells ? ImColor(0, 255, 0) : ImColor(255, 0, 0)));
		}

		
		if (Core::DrawSpells) {

			for (Spell spell : DetectedSpells)
			{
				if (spell.Draw)
					Render::Polygon(spell.path, ImColor(255, 255, 255), 1);
			}

		}

		if (/*DetectedSpells.size() > 0 && Evading &&*/ DrawSafePosition) {
			DrawArrow(Local->Position, SafePos, ImColor(41, 128, 185));
		}
		Render::EndOverlay();
	}

	void Core::SpellExistsThenRemove(string name) {
		auto it = DetectedSpells.begin();
		while (it != DetectedSpells.end()) {
			if ((*it).name == name && !Core::Debug) {
				it = DetectedSpells.erase(it);
			}
			else {
				++it;
			}
		}
	}

	Geometry::Polygon Core::GetPath(Spell& spell, bool boundingRadius)
	{
		float yCoord = spell.startY;

		// SpecialSpells
		if (spell.name == "PantheonR") {
			auto sP2 = spell.endPos.Extend(spell.startPos, 1150);
			auto eP2 = spell.startPos.Append(spell.startPos, spell.endPos, 200);
			return Geometry::Rectangle(sP2, eP2, spell.radius).ToPolygon().SetY(yCoord);
		}
		else if (spell.name == "ZoeE") {

		}
		else if (spell.name == "AatroxQ2") { // Tested: OK
			auto dir = (spell.startPos - spell.endPos).Perpendicular().Normalized() * spell.radius;
			auto s1 = spell.startPos - dir;
			auto s2 = spell.startPos + dir;

			auto e1 = spell.endPos.Rotate(s1, DEG2RAD(40));
			auto e2 = spell.endPos.Rotate(s2, -DEG2RAD(40));

			Geometry::Polygon path;
			path.Add(s1);
			path.Add(e1);
			path.Add(e2);
			path.Add(s2);

			path.SetY(yCoord);

			return path.OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.name == "GravesQLineSpell") { // Tested: OK
			auto s1 = spell.endPos - (spell.endPos - spell.startPos).Perpendicular().Normalized() * 240;
			auto e1 = spell.endPos + (spell.endPos - spell.startPos).Perpendicular().Normalized() * 240;

			auto p1 = Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon();
			auto p2 = Geometry::Rectangle(s1, e1, 150).ToPolygon();

			auto path = Geometry::Geometry::ClipPolygons(p1 + p2);
			return Geometry::Geometry::ClipToPolygons(path, yCoord).SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.name == "GravesChargeShot") { // Tested: OK
			auto p1 = Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon();
			auto e1 = spell.startPos.Append(spell.startPos, spell.endPos, 700);

			auto dir = (spell.endPos - e1).Perpendicular().Normalized() * 350;
			Geometry::Polygon path;

			path.Add(p1.Points[1]);
			path.Add(p1.Points[2]);
			path.Add(e1 + dir);
			path.Add(e1 - dir);
			path.Add(p1.Points[3]);
			path.Add(p1.Points[0]);

			path.SetY(yCoord);

			return path.OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.name == "JinxE") {

		}
		else if (spell.name == "MordekaiserQ") { // Tested: OK // a bit incorrect sides
			auto dir = (spell.endPos - spell.startPos).Perpendicular().Normalized() * 75;
			auto s1 = spell.startPos - dir;
			auto s2 = spell.startPos + dir;

			auto e1 = s1.Extend(spell.endPos, 675).Rotate(s1, -DEG2RAD(18));
			auto e2 = s2.Extend(spell.endPos, 675).Rotate(s2, DEG2RAD(18));

			Geometry::Polygon path;

			path.Add(s1);
			path.Add(e1);
			path.Add(e2);
			path.Add(s2);

			path.SetY(yCoord);

			return boundingRadius ? path.OffsetPolygon(BoundingRadius) : path;
		}
		else if (spell.name == "MordekaiserE") { // Tested: OK
			Vector3 endPos;
			Vector3 sP = spell.startPos;
			if (spell.startPos.distanceTo(spell.endPos) > spell.range - 200)
				endPos = spell.startPos.Extend(spell.endPos, spell.range);
			else
			{
				sP = spell.endPos.Extend(spell.startPos, spell.range);
				sP = sP.Prepend(sP, spell.endPos, 200);
				endPos = sP.Append(sP, spell.endPos, 200);
			}

			auto path = Geometry::Rectangle(sP, endPos, spell.radius).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
			return path;
		}
		else if (spell.name == "OrianaIzuna") {
			auto p1 = Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon(boundingRadius ? BoundingRadius : 0);
			auto p2 = Geometry::Circle(spell.endPos, 135).ToPolygon(boundingRadius ? BoundingRadius : 0, -1.0F, Core::CQ);
			auto path = Geometry::Geometry::ClipPolygons(p1 + p2);
			return Geometry::Geometry::ClipToPolygons(path, spell.endPos.y).SetY(yCoord);
		}
		else if (spell.name == "RellW") {
			auto sP2 = spell.endPos.Extend(spell.startPos, 500);
			auto eP2 = spell.startPos.Append(spell.startPos, spell.endPos, 200);
			return Geometry::Rectangle(sP2, eP2, spell.radius).ToPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.name == "SettW") { // Tested: OK
			auto sPos = spell.startPos.Append(spell.endPos, spell.startPos, -40);
			auto ePos = sPos.Extend(spell.endPos, spell.range);
			auto dir = (ePos - sPos).Perpendicular().Normalized() * spell.radius;
			auto s1 = sPos - dir;
			auto s2 = sPos + dir;
			auto e1 = s1.Extend(ePos, spell.range).Rotate(s1, -DEG2RAD(30));   // s1.Rotate(s1.Extend(ePos, spell.range), );
			auto e2 = s2.Extend(ePos, spell.range).Rotate(s2, DEG2RAD(30));

			Geometry::Polygon path;
			path.Add(s1);
			path.Add(e1);
			path.Add(e2);
			path.Add(s2);

			path.SetY(yCoord);

			return boundingRadius ? path.OffsetPolygon(BoundingRadius) : path;
		}
		else if (spell.name == "SettE") { // Tested: OK
			auto sPos = spell.startPos.Extend(spell.endPos, -spell.range);
			return Geometry::Rectangle(sPos, spell.endPos, spell.radius).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.name == "SylasQ") { // Tested: OK
			if (spell.startPos.distanceTo(spell.endPos) < 160) {
				spell.endPos = spell.startPos.Extend(spell.endPos, 160);
			}
			auto dir = (spell.endPos - spell.startPos).Perpendicular().Normalized() * 100;
			auto s1 = spell.startPos - dir;
			auto s2 = spell.startPos + dir;


			auto e1 = s1.Rotate(s1.Extend(spell.endPos, spell.range), DEG2RAD(3));
			auto e2 = s2.Rotate(s2.Extend(spell.endPos, spell.range), -DEG2RAD(3));

			auto p1 = Geometry::Rectangle(s1, e1, spell.range).ToPolygon(0, -1, 1);
			auto p2 = Geometry::Rectangle(s2, e2, spell.range).ToPolygon(0, -1, 0);


			auto p3 = Geometry::Circle(spell.endPos, 180 / 2).ToPolygon(0, -1.0F, Core::CQ);
			auto path = Geometry::Geometry::ClipToPolygons(Geometry::Geometry::ClipPolygons(p3 + (p1 + p2)), yCoord);

			return path.SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);

		}
		else if (spell.name == "ThreshEFlay") {

		}
		else if (spell.name == "ZiggsQ") {

		}

		if (spell.type == SpellType::linear || spell.type == SpellType::polygon)
		{
			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		/*else if (spell.type == SpellType::zone) {
			return Geometry::Rectangle(spell.endPos.Extend(spell.startPos, -spell.radius), spell.endPos, spell.radius).ToPolygon();
		}*/
		else if (spell.type == SpellType::rectangular)
		{
			Vector3 dir = (spell.endPos - spell.startPos).Perpendicular().Normalized() * (400);
			spell.startPos = spell.endPos - dir;
			spell.endPos = spell.endPos + dir;

			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius / 2).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.type == SpellType::circular)
		{
			return Geometry::Circle(spell.endPos, spell.radius).ToPolygon(0, -1, CQ).SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		/*else if (spell.type == SpellType::arc)
		{
			return Geometry::Arc(spell.startPos, spell.endPos, spell.radius).ToPolygon();
		}
		else if (spell.type == SpellType::ring)
		{
			return Geometry::Ring(spell.endPos, spell.radius, spell.radius).ToPolygon();
		}*/
		else if (spell.type == SpellType::conic)
		{
			return Geometry::Sector(spell.startPos, spell.endPos - spell.startPos, spell.angle * M_PI / 180.0f, spell.range).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		else if (spell.type == SpellType::threeway)
		{
			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
		}
		return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon().SetY(yCoord).OffsetPolygon(boundingRadius ? BoundingRadius : 0);
	}

	Vector3 Core::CalculateEndPos(Spell& spellInfo)
	{
		Vector3 startPos1 = spellInfo.startPos;
		Vector3 placementPos = spellInfo.endPos;
		Vector3 unitPos = spellInfo.obj->Position;
		float range = spellInfo.range;
		Vector3 endPos = startPos1.Extend(placementPos, range);

		if (!spellInfo.extend)
		{
			if (range > 0)
			{
				if (unitPos.distanceTo(placementPos) < range)
				{
					endPos = placementPos;
				}
			}
			else
			{
				endPos = unitPos;
			}
		}
		else if (spellInfo.type == SpellType::linear)
		{
			if (spellInfo.speed != MathHuge)
			{
				endPos = endPos.Append(startPos1, endPos, spellInfo.radius);
			}
			if (spellInfo.collisionMinions || spellInfo.collisionChamps)
			{
				Vector3 startPos = startPos1.Extend(placementPos, 45);
				std::list<Vector3>minions;
				if (spellInfo.collisionMinions) {
					for (GameObject* minion : ObjectManager::MinionList())
					{
						if (minion->NetworkID - (unsigned int)0x40000000 > 0x100000)
							continue;
						if (minion && minion->IsTargetable && minion->IsEnemyTo(spellInfo.obj))
						{
							Vector3 minionPos = minion->Position;
							if (minionPos.distanceTo(startPos) <= range && minion->MaxHealth > 295 && minion->Health > 5)
							{
								Vector3 col = minionPos.ProjectOn(startPos, placementPos).SegmentPoint;
								if (!col.IsZero() && col.distanceTo(minionPos) < ((minion->GetBoundingRadius()) /* / 2 */ + spellInfo.radius))
								{
									minions.emplace_back(minionPos);
								}
							}
						}
					}
				}
				if (spellInfo.collisionMinions || spellInfo.collisionChamps) {
					for (GameObject* minion : ObjectManager::HeroList()) // TODO remove code copying
					{
						if (minion->NetworkID - (unsigned int)0x40000000 > 0x100000)
							continue;
						if (minion && minion->IsTargetable && minion->NetworkID != Local->NetworkID && minion->IsEnemyTo(spellInfo.obj))
						{
							Vector3 minionPos = minion->Position;
							if (minionPos.distanceTo(startPos) <= range && minion->MaxHealth > 295 && minion->Health > 5)
							{
								Vector3 col = minionPos.ProjectOn(startPos, placementPos).SegmentPoint;
								if (!col.IsZero() && col.distanceTo(minionPos) < ((minion->GetBoundingRadius()) /* / 2 */ + spellInfo.radius))
								{
									minions.emplace_back(minionPos);
								}
							}
						}
					}
				}
				if (minions.size() > 0)
				{
					minions.sort([&](Vector3 const& a, Vector3 const& b)
						{
							return a.DistanceSquared(startPos) < b.DistanceSquared(startPos);
						});
					float range2 = startPos.distanceTo(minions.front());
					endPos = (startPos).Extend(placementPos, range2);
					spellInfo.range = range2;
				}
			}
		}
		if (!spellInfo.extend)
			spellInfo.range = startPos1.distanceTo(endPos);
		spellInfo.range = range;

		return endPos;
	}

	void Core::AddSpell(Spell& spell) {
		Core::DetectedSpells.push_back(spell);
	}

	void Core::OnProcessSpell(void* spellBook, SpellInfo* castInfo) {

		auto Caster = ObjectManager::FindObjectByIndex(ObjectManager::MinionList(), castInfo->source_id);
		if (Caster)
			return;
		auto caster2 = ObjectManager::FindObjectByIndex(ObjectManager::HeroList(), castInfo->source_id);

#if !_DEBUGEVADE

		if (caster2 && Local->IsAllyTo(caster2))
			return;
#endif


		if (Core::LimitRange && caster2 && caster2->Position.distanceTo(Local->Position) > Core::LR)
			return;


		for (Champ& champ : ChampsInGame)
		{
			for (Spell& s : champ.Spells)
			{
				if (Compare(castInfo->BasicAttackSpellData->Name, s.name, true))
				{
					Spell newSpell = s;

					newSpell.startTime = GameTimer;
					newSpell.source_id = castInfo->source_id;
					newSpell.sourceNetworkId = castInfo->SourceNetworkID;

					if (newSpell.useEndPos2) {
						newSpell.startY = castInfo->EndPosition.y;
						newSpell.endY = castInfo->EndPosition2.y;
						newSpell.startPos = Vector3(castInfo->EndPosition.x, 0, castInfo->EndPosition.z);
						newSpell.endPos = Vector3(castInfo->EndPosition2.x, 0, castInfo->EndPosition2.z);
						//newSpell.endPos2 = Vector3(castInfo->EndPosition2.x, 0, castInfo->EndPosition2.z);
					}
					else {
						newSpell.startY = castInfo->StartPosition.y;
						newSpell.endY = castInfo->EndPosition.y;
						newSpell.startPos = Vector3(castInfo->StartPosition.x, 0, castInfo->StartPosition.z);
						newSpell.endPos = Vector3(castInfo->EndPosition.x, 0, castInfo->EndPosition.z);
						//newSpell.endPos2 = Vector3(castInfo->EndPosition2.x, 0, castInfo->EndPosition2.z);
					}


					newSpell.obj = Caster ? Caster : caster2;

					Vector3 endPos = CalculateEndPos(newSpell);
					newSpell.endPos = endPos;
					/*if (champ.Name == "Yasuo" || champ.Name == "Yone")
						endPos = newSpell.startPos + champ.obj->Direction * s.range;*/

					auto Path = GetPath(newSpell, false);
					if (s.name == "VelkozQ") {
						SpellExistsThenRemove("VelkozQ");
					}
					newSpell.path = Path;

					Core::AddSpell(newSpell);
					addedSpells.push_back(castInfo->Index);
				}
			}
		}
	}

	void InitSpellsDB() {
		{
			Champ Aatrox;
			Aatrox.Name = "Aatrox";
			Spell AatroxQ;
			AatroxQ.name = "AatroxQ";
			AatroxQ.icon = "AatroxQ1";
			AatroxQ.displayName = "The Darkin Blade [First]";
			AatroxQ.missileName = "";
			AatroxQ.slot = SpellSlot::Q;
			AatroxQ.type = SpellType::linear;
			AatroxQ.range = 650;
			AatroxQ.radius = 130;
			AatroxQ.delay = 0.6;
			AatroxQ.danger = 3;
			AatroxQ.speed = MathHuge;
			AatroxQ.cc = true;
			AatroxQ.collisionMinions = false;
			AatroxQ.windwall = false;
			AatroxQ.hitbox = false;
			AatroxQ.fow = false;
			AatroxQ.exception = false;
			AatroxQ.extend = true;
			Aatrox.Spells.push_back(AatroxQ);
			Spell AatroxQ2;
			AatroxQ2.name = "AatroxQ2";
			AatroxQ2.icon = "AatroxQ2";
			AatroxQ2.displayName = "The Darkin Blade [Second]";
			AatroxQ2.missileName = "";
			AatroxQ2.slot = SpellSlot::Q;
			AatroxQ2.type = SpellType::polygon;
			AatroxQ2.range = 500;
			AatroxQ2.radius = 200;
			AatroxQ2.delay = 0.6;
			AatroxQ2.danger = 3;
			AatroxQ2.speed = MathHuge;
			AatroxQ2.cc = true;
			AatroxQ2.collisionMinions = false;
			AatroxQ2.windwall = false;
			AatroxQ2.hitbox = false;
			AatroxQ2.fow = false;
			AatroxQ2.exception = false;
			AatroxQ2.extend = true;
			Aatrox.Spells.push_back(AatroxQ2);
			Spell AatroxQ3;
			AatroxQ3.name = "AatroxQ3";
			AatroxQ3.icon = "AatroxQ3";
			AatroxQ3.displayName = "The Darkin Blade [Third]";
			AatroxQ3.missileName = "";
			AatroxQ3.slot = SpellSlot::Q;
			AatroxQ3.type = SpellType::circular;
			AatroxQ3.range = 200;
			AatroxQ3.radius = 300;
			AatroxQ3.delay = 0.6;
			AatroxQ3.danger = 4;
			AatroxQ3.speed = MathHuge;
			AatroxQ3.cc = true;
			AatroxQ3.collisionMinions = false;
			AatroxQ3.windwall = false;
			AatroxQ3.hitbox = false;
			AatroxQ3.fow = false;
			AatroxQ3.exception = false;
			AatroxQ3.extend = false;
			Aatrox.Spells.push_back(AatroxQ3);
			Spell AatroxW;
			AatroxW.name = "AatroxW";
			AatroxW.icon = "AatroxW";
			AatroxW.displayName = "Infernal Chains";
			AatroxW.missileName = "AatroxW";
			AatroxW.slot = SpellSlot::W;
			AatroxW.type = SpellType::linear;
			AatroxW.range = 825;
			AatroxW.radius = 80;
			AatroxW.delay = 0.25;
			AatroxW.danger = 2;
			AatroxW.speed = 1800;
			AatroxW.cc = true;
			AatroxW.collisionMinions = true;
			AatroxW.windwall = true;
			AatroxW.hitbox = true;
			AatroxW.fow = true;
			AatroxW.exception = false;
			AatroxW.extend = true;
			Aatrox.Spells.push_back(AatroxW);
			Core::SpellDB.emplace_back(Aatrox);
		}
		{
			Champ Ahri;
			Ahri.Name = "Ahri";
			Spell AhriOrbofDeception;
			AhriOrbofDeception.name = "AhriOrbofDeception";
			AhriOrbofDeception.icon = "AhriQ";
			AhriOrbofDeception.displayName = "Orb of Deception";
			AhriOrbofDeception.missileName = "AhriOrbMissile";
			AhriOrbofDeception.slot = SpellSlot::Q;
			AhriOrbofDeception.type = SpellType::linear;
			AhriOrbofDeception.range = 880;
			AhriOrbofDeception.radius = 100;
			AhriOrbofDeception.delay = 0.25;
			AhriOrbofDeception.danger = 2;
			AhriOrbofDeception.speed = 2500;
			AhriOrbofDeception.cc = false;
			AhriOrbofDeception.collisionMinions = false;
			AhriOrbofDeception.windwall = true;
			AhriOrbofDeception.hitbox = true;
			AhriOrbofDeception.fow = true;
			AhriOrbofDeception.exception = false;
			AhriOrbofDeception.extend = true;
			Ahri.Spells.push_back(AhriOrbofDeception);
			Spell AhriSeduce;
			AhriSeduce.name = "AhriSeduce";
			AhriSeduce.icon = "AhriE";
			AhriSeduce.displayName = "Seduce";
			AhriSeduce.missileName = "AhriSeduceMissile";
			AhriSeduce.slot = SpellSlot::E;
			AhriSeduce.type = SpellType::linear;
			AhriSeduce.range = 975;
			AhriSeduce.radius = 60;
			AhriSeduce.delay = 0.25;
			AhriSeduce.danger = 1;
			AhriSeduce.speed = 1500;
			AhriSeduce.cc = true;
			AhriSeduce.collisionMinions = true;
			AhriSeduce.windwall = true;
			AhriSeduce.hitbox = true;
			AhriSeduce.fow = true;
			AhriSeduce.exception = false;
			AhriSeduce.extend = true;
			Ahri.Spells.push_back(AhriSeduce);
			Core::SpellDB.emplace_back(Ahri);
		}
		{
			Champ Akali;
			Akali.Name = "Akali";
			Spell AkaliQ;
			AkaliQ.name = "AkaliQ";
			AkaliQ.icon = "AkaliQ";
			AkaliQ.displayName = "Five Point Strike";
			AkaliQ.missileName = "";
			AkaliQ.slot = SpellSlot::Q;
			AkaliQ.type = SpellType::conic;
			AkaliQ.range = 550;
			AkaliQ.radius = 60;
			AkaliQ.delay = 0.25;
			AkaliQ.danger = 2;
			AkaliQ.speed = 3200;
			AkaliQ.cc = false;
			AkaliQ.collisionMinions = false;
			AkaliQ.windwall = true;
			AkaliQ.hitbox = false;
			AkaliQ.fow = false;
			AkaliQ.exception = false;
			AkaliQ.extend = true;
			Akali.Spells.push_back(AkaliQ);
			Spell AkaliE;
			AkaliE.name = "AkaliE";
			AkaliE.icon = "AkaliE";
			AkaliE.displayName = "Shuriken Flip";
			AkaliE.missileName = "AkaliEMis";
			AkaliE.slot = SpellSlot::E;
			AkaliE.type = SpellType::linear;
			AkaliE.range = 825;
			AkaliE.radius = 70;
			AkaliE.delay = 0.25;
			AkaliE.danger = 2;
			AkaliE.speed = 1800;
			AkaliE.cc = false;
			AkaliE.collisionMinions = true;
			AkaliE.windwall = true;
			AkaliE.hitbox = true;
			AkaliE.fow = true;
			AkaliE.exception = false;
			AkaliE.extend = true;
			Akali.Spells.push_back(AkaliE);
			Spell AkaliR;
			AkaliR.name = "AkaliR";
			AkaliR.icon = "AkaliR1";
			AkaliR.displayName = "Perfect Execution [First]";
			AkaliR.missileName = "";
			AkaliR.slot = SpellSlot::R;
			AkaliR.type = SpellType::linear;
			AkaliR.range = 675;
			AkaliR.radius = 65;
			AkaliR.delay = 0;
			AkaliR.danger = 4;
			AkaliR.speed = 1800;
			AkaliR.cc = true;
			AkaliR.collisionMinions = false;
			AkaliR.windwall = false;
			AkaliR.hitbox = false;
			AkaliR.fow = false;
			AkaliR.exception = false;
			AkaliR.extend = true;
			Akali.Spells.push_back(AkaliR);
			Spell AkaliRb;
			AkaliRb.name = "AkaliRb";
			AkaliRb.icon = "AkaliR2";
			AkaliRb.displayName = "Perfect Execution [Second]";
			AkaliRb.missileName = "";
			AkaliRb.slot = SpellSlot::R;
			AkaliRb.type = SpellType::linear;
			AkaliRb.range = 525;
			AkaliRb.radius = 65;
			AkaliRb.delay = 0;
			AkaliRb.danger = 4;
			AkaliRb.speed = 3600;
			AkaliRb.cc = false;
			AkaliRb.collisionMinions = false;
			AkaliRb.windwall = false;
			AkaliRb.hitbox = false;
			AkaliRb.fow = false;
			AkaliRb.exception = false;
			AkaliRb.extend = true;
			Akali.Spells.push_back(AkaliRb);
			Core::SpellDB.emplace_back(Akali);
		}
		{
			Champ Alistar;
			Alistar.Name = "Alistar";
			Spell Pulverize;
			Pulverize.name = "Pulverize";
			Pulverize.icon = "AlistarQ";
			Pulverize.displayName = "Pulverize";
			Pulverize.missileName = "";
			Pulverize.slot = SpellSlot::Q;
			Pulverize.type = SpellType::circular;
			Pulverize.range = 0;
			Pulverize.radius = 365;
			Pulverize.delay = 0.25;
			Pulverize.danger = 3;
			Pulverize.speed = MathHuge;
			Pulverize.cc = true;
			Pulverize.collisionMinions = false;
			Pulverize.windwall = false;
			Pulverize.hitbox = false;
			Pulverize.fow = false;
			Pulverize.exception = false;
			Pulverize.extend = false;
			Alistar.Spells.push_back(Pulverize);
			Core::SpellDB.emplace_back(Alistar);
		}
		{
			Champ Amumu;
			Amumu.Name = "Amumu";
			Spell BandageToss;
			BandageToss.name = "BandageToss";
			BandageToss.icon = "AmumuQ";
			BandageToss.displayName = "Bandage Toss";
			BandageToss.missileName = "SadMummyBandageToss";
			BandageToss.slot = SpellSlot::Q;
			BandageToss.type = SpellType::linear;
			BandageToss.range = 1100;
			BandageToss.radius = 80;
			BandageToss.delay = 0.25;
			BandageToss.danger = 3;
			BandageToss.speed = 2000;
			BandageToss.cc = true;
			BandageToss.collisionMinions = true;
			BandageToss.windwall = true;
			BandageToss.hitbox = false;
			BandageToss.fow = true;
			BandageToss.exception = false;
			BandageToss.extend = true;
			Amumu.Spells.push_back(BandageToss);
			Spell CurseoftheSadMummy;
			CurseoftheSadMummy.name = "CurseoftheSadMummy";
			CurseoftheSadMummy.icon = "AmumuR";
			CurseoftheSadMummy.displayName = "Curse of the Sad Mummy";
			CurseoftheSadMummy.missileName = "";
			CurseoftheSadMummy.slot = SpellSlot::R;
			CurseoftheSadMummy.type = SpellType::circular;
			CurseoftheSadMummy.range = 0;
			CurseoftheSadMummy.radius = 550;
			CurseoftheSadMummy.delay = 0.25;
			CurseoftheSadMummy.danger = 5;
			CurseoftheSadMummy.speed = MathHuge;
			CurseoftheSadMummy.cc = true;
			CurseoftheSadMummy.collisionMinions = false;
			CurseoftheSadMummy.windwall = false;
			CurseoftheSadMummy.hitbox = false;
			CurseoftheSadMummy.fow = false;
			CurseoftheSadMummy.exception = false;
			CurseoftheSadMummy.extend = false;
			Amumu.Spells.push_back(CurseoftheSadMummy);
			Core::SpellDB.emplace_back(Amumu);
		}
		{
			Champ Anivia;
			Anivia.Name = "Anivia";
			Spell FlashFrostSpell;
			FlashFrostSpell.name = "FlashFrostSpell";
			FlashFrostSpell.icon = "AniviaQ";
			FlashFrostSpell.displayName = "Flash Frost";
			FlashFrostSpell.missileName = "FlashFrostSpell";
			FlashFrostSpell.slot = SpellSlot::Q;
			FlashFrostSpell.type = SpellType::linear;
			FlashFrostSpell.range = 1100;
			FlashFrostSpell.radius = 110;
			FlashFrostSpell.delay = 0.25;
			FlashFrostSpell.danger = 2;
			FlashFrostSpell.speed = 950;
			FlashFrostSpell.cc = true;
			FlashFrostSpell.collisionMinions = false;
			FlashFrostSpell.windwall = true;
			FlashFrostSpell.hitbox = true;
			FlashFrostSpell.fow = true;
			FlashFrostSpell.exception = false;
			FlashFrostSpell.extend = true;
			Anivia.Spells.push_back(FlashFrostSpell);
			Core::SpellDB.emplace_back(Anivia);
		}
		{
			Champ Annie;
			Annie.Name = "Annie";
			Spell AnnieW;
			AnnieW.name = "AnnieW";
			AnnieW.icon = "AnnieW";
			AnnieW.displayName = "Incinerate";
			AnnieW.missileName = "";
			AnnieW.slot = SpellSlot::W;
			AnnieW.type = SpellType::conic;
			AnnieW.range = 600;
			AnnieW.radius = 0;
			AnnieW.delay = 0.25;
			AnnieW.danger = 2;
			AnnieW.speed = MathHuge;
			AnnieW.cc = false;
			AnnieW.collisionMinions = false;
			AnnieW.windwall = false;
			AnnieW.hitbox = false;
			AnnieW.fow = false;
			AnnieW.exception = false;
			AnnieW.extend = true;
			Annie.Spells.push_back(AnnieW);
			Spell AnnieR;
			AnnieR.name = "AnnieR";
			AnnieR.icon = "AnnieR";
			AnnieR.displayName = "Summon: Tibbers";
			AnnieR.missileName = "";
			AnnieR.slot = SpellSlot::R;
			AnnieR.type = SpellType::circular;
			AnnieR.range = 600;
			AnnieR.radius = 290;
			AnnieR.delay = 0.25;
			AnnieR.danger = 5;
			AnnieR.speed = MathHuge;
			AnnieR.cc = false;
			AnnieR.collisionMinions = false;
			AnnieR.windwall = false;
			AnnieR.hitbox = false;
			AnnieR.fow = false;
			AnnieR.exception = false;
			AnnieR.extend = false;
			Annie.Spells.push_back(AnnieR);
			Core::SpellDB.emplace_back(Annie);
		}
		{
			Champ Aphelios;
			Aphelios.Name = "Aphelios";
			Spell ApheliosCalibrumQ;
			ApheliosCalibrumQ.name = "ApheliosCalibrumQ";
			ApheliosCalibrumQ.icon = "ApheliosQ1";
			ApheliosCalibrumQ.displayName = "Moonshot";
			ApheliosCalibrumQ.missileName = "ApheliosCalibrumQ";
			ApheliosCalibrumQ.slot = SpellSlot::Q;
			ApheliosCalibrumQ.type = SpellType::linear;
			ApheliosCalibrumQ.range = 1450;
			ApheliosCalibrumQ.radius = 60;
			ApheliosCalibrumQ.delay = 0.35;
			ApheliosCalibrumQ.danger = 1;
			ApheliosCalibrumQ.speed = 1850;
			ApheliosCalibrumQ.cc = false;
			ApheliosCalibrumQ.collisionMinions = true;
			ApheliosCalibrumQ.windwall = true;
			ApheliosCalibrumQ.hitbox = false;
			ApheliosCalibrumQ.fow = true;
			ApheliosCalibrumQ.exception = false;
			ApheliosCalibrumQ.extend = true;
			Aphelios.Spells.push_back(ApheliosCalibrumQ);
			Spell ApheliosInfernumQ;
			ApheliosInfernumQ.name = "ApheliosInfernumQ";
			ApheliosInfernumQ.icon = "ApheliosQ2";
			ApheliosInfernumQ.displayName = "Duskwave";
			ApheliosInfernumQ.missileName = "";
			ApheliosInfernumQ.slot = SpellSlot::Q;
			ApheliosInfernumQ.type = SpellType::conic;
			ApheliosInfernumQ.range = 850;
			ApheliosInfernumQ.radius = 65;
			ApheliosInfernumQ.delay = 0.25;
			ApheliosInfernumQ.danger = 2;
			ApheliosInfernumQ.speed = 1500;
			ApheliosInfernumQ.cc = false;
			ApheliosInfernumQ.collisionMinions = false;
			ApheliosInfernumQ.windwall = true;
			ApheliosInfernumQ.hitbox = false;
			ApheliosInfernumQ.fow = false;
			ApheliosInfernumQ.exception = false;
			ApheliosInfernumQ.extend = true;
			Aphelios.Spells.push_back(ApheliosInfernumQ);
			Spell ApheliosR;
			ApheliosR.name = "ApheliosR";
			ApheliosR.icon = "ApheliosR";
			ApheliosR.displayName = "Moonlight Vigil";
			ApheliosR.missileName = "ApheliosRMis";
			ApheliosR.slot = SpellSlot::R;
			ApheliosR.type = SpellType::linear;
			ApheliosR.range = 1600;
			ApheliosR.radius = 125;
			ApheliosR.delay = 0.5;
			ApheliosR.danger = 3;
			ApheliosR.speed = 2050;
			ApheliosR.cc = false;
			ApheliosR.collisionMinions = false;
			ApheliosR.windwall = true;
			ApheliosR.hitbox = false;
			ApheliosR.fow = true;
			ApheliosR.exception = false;
			ApheliosR.extend = true;
			Aphelios.Spells.push_back(ApheliosR);
			Core::SpellDB.emplace_back(Aphelios);
		}
		{
			Champ Ashe;
			Ashe.Name = "Ashe";
			Spell Volley;
			Volley.name = "Volley";
			Volley.icon = "AsheW";
			Volley.displayName = "Volley";
			Volley.missileName = "VolleyRightAttack";
			Volley.slot = SpellSlot::W;
			Volley.type = SpellType::conic;
			Volley.range = 1200;
			Volley.radius = 20;
			Volley.delay = 0.25;
			Volley.danger = 2;
			Volley.speed = 2000;
			Volley.cc = true;
			Volley.collisionMinions = true;
			Volley.windwall = true;
			Volley.hitbox = false;
			Volley.fow = true;
			Volley.exception = false;
			Volley.extend = true;
			Ashe.Spells.push_back(Volley);
			Spell EnchantedCrystalArrow;
			EnchantedCrystalArrow.name = "EnchantedCrystalArrow";
			EnchantedCrystalArrow.icon = "AsheR";
			EnchantedCrystalArrow.displayName = "Enchanted Crystal Arrow";
			EnchantedCrystalArrow.missileName = "EnchantedCrystalArrow";
			EnchantedCrystalArrow.slot = SpellSlot::R;
			EnchantedCrystalArrow.type = SpellType::linear;
			EnchantedCrystalArrow.range = 12500;
			EnchantedCrystalArrow.radius = 130;
			EnchantedCrystalArrow.delay = 0.25;
			EnchantedCrystalArrow.danger = 4;
			EnchantedCrystalArrow.speed = 1600;
			EnchantedCrystalArrow.cc = true;
			EnchantedCrystalArrow.collisionMinions = false;
			EnchantedCrystalArrow.windwall = true;
			EnchantedCrystalArrow.hitbox = true;
			EnchantedCrystalArrow.fow = true;
			EnchantedCrystalArrow.exception = false;
			EnchantedCrystalArrow.extend = true;
			Ashe.Spells.push_back(EnchantedCrystalArrow);
			Core::SpellDB.emplace_back(Ashe);
		}
		{
			Champ AurelionSol;
			AurelionSol.Name = "AurelionSol";
			Spell AurelionSolQ;
			AurelionSolQ.name = "AurelionSolQ";
			AurelionSolQ.icon = "AurelionSolQ";
			AurelionSolQ.displayName = "Starsurge";
			AurelionSolQ.missileName = "AurelionSolQMissile";
			AurelionSolQ.slot = SpellSlot::Q;
			AurelionSolQ.type = SpellType::linear;
			AurelionSolQ.range = 1075;
			AurelionSolQ.radius = 110;
			AurelionSolQ.delay = 0;
			AurelionSolQ.danger = 2;
			AurelionSolQ.speed = 850;
			AurelionSolQ.cc = true;
			AurelionSolQ.collisionMinions = false;
			AurelionSolQ.windwall = true;
			AurelionSolQ.hitbox = true;
			AurelionSolQ.fow = true;
			AurelionSolQ.exception = false;
			AurelionSolQ.extend = true;
			AurelionSol.Spells.push_back(AurelionSolQ);
			Spell AurelionSolR;
			AurelionSolR.name = "AurelionSolR";
			AurelionSolR.icon = "AurelionSolR";
			AurelionSolR.displayName = "Voice of Light";
			AurelionSolR.missileName = "";
			AurelionSolR.slot = SpellSlot::R;
			AurelionSolR.type = SpellType::linear;
			AurelionSolR.range = 1500;
			AurelionSolR.radius = 120;
			AurelionSolR.delay = 0.35;
			AurelionSolR.danger = 5;
			AurelionSolR.speed = 4500;
			AurelionSolR.cc = true;
			AurelionSolR.collisionMinions = false;
			AurelionSolR.windwall = false;
			AurelionSolR.hitbox = false;
			AurelionSolR.fow = false;
			AurelionSolR.exception = false;
			AurelionSolR.extend = true;
			AurelionSol.Spells.push_back(AurelionSolR);
			Core::SpellDB.emplace_back(AurelionSol);
		}
		{
			Champ Azir;
			Azir.Name = "Azir";
			Spell AzirR;
			AzirR.name = "AzirR";
			AzirR.icon = "AzirR";
			AzirR.displayName = "Emperor's Divide";
			AzirR.missileName = "";
			AzirR.slot = SpellSlot::R;
			AzirR.type = SpellType::linear;
			AzirR.range = 500;
			AzirR.radius = 250;
			AzirR.delay = 0.3;
			AzirR.danger = 5;
			AzirR.speed = 1400;
			AzirR.cc = true;
			AzirR.collisionMinions = false;
			AzirR.windwall = false;
			AzirR.hitbox = false;
			AzirR.fow = false;
			AzirR.exception = false;
			AzirR.extend = true;
			Azir.Spells.push_back(AzirR);
			Core::SpellDB.emplace_back(Azir);
		}
		{
			Champ BelVeth;
			BelVeth.Name = "BelVeth";
			Spell BelvethQ;
			BelvethQ.name = "BelvethQ";
			BelvethQ.icon = "BelVethQ";
			BelvethQ.displayName = "Void Surge";
			BelvethQ.missileName = "";
			BelvethQ.slot = SpellSlot::Q;
			BelvethQ.type = SpellType::linear;
			BelvethQ.range = 450;
			BelvethQ.radius = 100;
			BelvethQ.delay = 0.0;
			BelvethQ.danger = 1;
			BelvethQ.speed = 1200;
			BelvethQ.cc = false;
			BelvethQ.collisionMinions = false;
			BelvethQ.windwall = false;
			BelvethQ.hitbox = false;
			BelvethQ.fow = false;
			BelvethQ.exception = false;
			BelvethQ.extend = false;
			BelVeth.Spells.push_back(BelvethQ);
			Spell BelvethW;
			BelvethW.name = "BelvethW";
			BelvethW.icon = "BelVethW";
			BelvethW.displayName = "Above and Below";
			BelvethW.missileName = "";
			BelvethW.slot = SpellSlot::W;
			BelvethW.type = SpellType::linear;
			BelvethW.range = 715;
			BelvethW.radius = 200;
			BelvethW.delay = 0.5;
			BelvethW.danger = 3;
			BelvethW.speed = 500;
			BelvethW.cc = true;
			BelvethW.collisionMinions = false;
			BelvethW.windwall = false;
			BelvethW.hitbox = false;
			BelvethW.fow = false;
			BelvethW.exception = false;
			BelvethW.extend = true;
			BelVeth.Spells.push_back(BelvethW);
			Spell BelvethE;
			BelvethE.name = "BelvethE";
			BelvethE.icon = "BelVethE";
			BelvethE.displayName = "Royal Maelstrom";
			BelvethE.missileName = "";
			BelvethE.slot = SpellSlot::E;
			BelvethE.type = SpellType::circular;
			BelvethE.range = 0.0;
			BelvethE.radius = 500;
			BelvethE.delay = 1.5;
			BelvethE.danger = 2;
			BelvethE.speed = MathHuge;
			BelvethE.cc = false;
			BelvethE.collisionMinions = false;
			BelvethE.windwall = false;
			BelvethE.hitbox = false;
			BelvethE.fow = false;
			BelvethE.exception = false;
			BelvethE.extend = false;
			BelVeth.Spells.push_back(BelvethE);
			Spell BelvethR;
			BelvethR.name = "BelvethR";
			BelvethR.icon = "BelVethR";
			BelvethR.displayName = "Endless Banquet";
			BelvethR.missileName = "";
			BelvethR.slot = SpellSlot::R;
			BelvethR.type = SpellType::circular;
			BelvethR.range = 275;
			BelvethR.radius = 500;
			BelvethR.delay = 1.0;
			BelvethR.danger = 4;
			BelvethR.speed = MathHuge;
			BelvethR.cc = true;
			BelvethR.collisionMinions = false;
			BelvethR.windwall = false;
			BelvethR.hitbox = false;
			BelvethR.fow = false;
			BelvethR.exception = false;
			BelvethR.extend = false;
			BelVeth.Spells.push_back(BelvethR);
			Core::SpellDB.emplace_back(BelVeth);
		}
		{
			Champ Bard;
			Bard.Name = "Bard";
			Spell BardQ;
			BardQ.name = "BardQ";
			BardQ.icon = "BardQ";
			BardQ.displayName = "Cosmic Binding";
			BardQ.missileName = "BardQMissile";
			BardQ.slot = SpellSlot::Q;
			BardQ.type = SpellType::linear;
			BardQ.range = 950;
			BardQ.radius = 60;
			BardQ.delay = 0.25;
			BardQ.danger = 2;
			BardQ.speed = 1500;
			BardQ.cc = true;
			BardQ.collisionMinions = true;
			BardQ.windwall = true;
			BardQ.hitbox = true;
			BardQ.fow = true;
			BardQ.exception = false;
			BardQ.extend = true;
			Bard.Spells.push_back(BardQ);
			Spell BardR;
			BardR.name = "BardR";
			BardR.icon = "BardR";
			BardR.displayName = "Tempered Fate";
			BardR.missileName = "BardRMissile";
			BardR.slot = SpellSlot::R;
			BardR.type = SpellType::circular;
			BardR.range = 3400;
			BardR.radius = 350;
			BardR.delay = 0.5;
			BardR.danger = 2;
			BardR.speed = 2100;
			BardR.cc = true;
			BardR.collisionMinions = false;
			BardR.windwall = false;
			BardR.hitbox = false;
			BardR.fow = true;
			BardR.exception = false;
			BardR.extend = false;
			Bard.Spells.push_back(BardR);
			Core::SpellDB.emplace_back(Bard);
		}
		{
			Champ Blitzcrank;
			Blitzcrank.Name = "Blitzcrank";
			Spell RocketGrab;
			RocketGrab.name = "RocketGrab";
			RocketGrab.icon = "BlitzcrankQ";
			RocketGrab.displayName = "Rocket Grab";
			RocketGrab.missileName = "RocketGrabMissile";
			RocketGrab.slot = SpellSlot::Q;
			RocketGrab.type = SpellType::linear;
			RocketGrab.range = 1150;
			RocketGrab.radius = 70;
			RocketGrab.delay = 0.25;
			RocketGrab.danger = 3;
			RocketGrab.speed = 1800;
			RocketGrab.cc = true;
			RocketGrab.collisionMinions = true;
			RocketGrab.windwall = true;
			RocketGrab.hitbox = true;
			RocketGrab.fow = true;
			RocketGrab.exception = false;
			RocketGrab.extend = true;
			Blitzcrank.Spells.push_back(RocketGrab);
			Spell StaticField;
			StaticField.name = "StaticField";
			StaticField.icon = "BlitzcrankR";
			StaticField.displayName = "Static Field";
			StaticField.missileName = "";
			StaticField.slot = SpellSlot::R;
			StaticField.type = SpellType::circular;
			StaticField.range = 0;
			StaticField.radius = 600;
			StaticField.delay = 0.25;
			StaticField.danger = 4;
			StaticField.speed = MathHuge;
			StaticField.cc = true;
			StaticField.collisionMinions = false;
			StaticField.windwall = false;
			StaticField.hitbox = false;
			StaticField.fow = false;
			StaticField.exception = false;
			StaticField.extend = false;
			Blitzcrank.Spells.push_back(StaticField);
			Core::SpellDB.emplace_back(Blitzcrank);
		}
		{
			Champ Brand;
			Brand.Name = "Brand";
			Spell BrandQ;
			BrandQ.name = "BrandQ";
			BrandQ.icon = "BrandQ";
			BrandQ.displayName = "Sear";
			BrandQ.missileName = "BrandQMissile";
			BrandQ.slot = SpellSlot::Q;
			BrandQ.type = SpellType::linear;
			BrandQ.range = 1050;
			BrandQ.radius = 60;
			BrandQ.delay = 0.25;
			BrandQ.danger = 1;
			BrandQ.speed = 1600;
			BrandQ.cc = false;
			BrandQ.collisionMinions = true;
			BrandQ.windwall = true;
			BrandQ.hitbox = true;
			BrandQ.fow = true;
			BrandQ.exception = false;
			BrandQ.extend = true;
			Brand.Spells.push_back(BrandQ);
			Spell BrandW;
			BrandW.name = "BrandW";
			BrandW.icon = "BrandW";
			BrandW.displayName = "Pillar of Flame";
			BrandW.missileName = "";
			BrandW.slot = SpellSlot::W;
			BrandW.type = SpellType::circular;
			BrandW.range = 900;
			BrandW.radius = 250;
			BrandW.delay = 0.85;
			BrandW.danger = 2;
			BrandW.speed = MathHuge;
			BrandW.cc = false;
			BrandW.collisionMinions = false;
			BrandW.windwall = false;
			BrandW.hitbox = false;
			BrandW.fow = false;
			BrandW.exception = false;
			BrandW.extend = false;
			Brand.Spells.push_back(BrandW);
			Core::SpellDB.emplace_back(Brand);
		}
		{
			Champ Braum;
			Braum.Name = "Braum";
			Spell BraumQ;
			BraumQ.name = "BraumQ";
			BraumQ.icon = "BraumQ";
			BraumQ.displayName = "Winter's Bite";
			BraumQ.missileName = "BraumQMissile";
			BraumQ.slot = SpellSlot::Q;
			BraumQ.type = SpellType::linear;
			BraumQ.range = 1000;
			BraumQ.radius = 70;
			BraumQ.delay = 0.25;
			BraumQ.danger = 3;
			BraumQ.speed = 1700;
			BraumQ.cc = true;
			BraumQ.collisionMinions = true;
			BraumQ.windwall = true;
			BraumQ.hitbox = true;
			BraumQ.fow = true;
			BraumQ.exception = false;
			BraumQ.extend = true;
			Braum.Spells.push_back(BraumQ);
			Spell BraumR;
			BraumR.name = "BraumR";
			BraumR.icon = "BraumR";
			BraumR.displayName = "Glacial Fissure";
			BraumR.missileName = "BraumRMissile";
			BraumR.slot = SpellSlot::R;
			BraumR.type = SpellType::linear;
			BraumR.range = 1250;
			BraumR.radius = 115;
			BraumR.delay = 0.5;
			BraumR.danger = 4;
			BraumR.speed = 1400;
			BraumR.cc = true;
			BraumR.collisionMinions = false;
			BraumR.windwall = true;
			BraumR.hitbox = true;
			BraumR.fow = true;
			BraumR.exception = false;
			BraumR.extend = true;
			Braum.Spells.push_back(BraumR);
			Core::SpellDB.emplace_back(Braum);
		}
		{
			Champ Caitlyn;
			Caitlyn.Name = "Caitlyn";
			Spell CaitlynPiltoverPeacemaker;
			CaitlynPiltoverPeacemaker.name = "CaitlynPiltoverPeacemaker";
			CaitlynPiltoverPeacemaker.icon = "CaitlynQ";
			CaitlynPiltoverPeacemaker.displayName = "Piltover Peacemaker";
			CaitlynPiltoverPeacemaker.missileName = "CaitlynPiltoverPeacemaker";
			CaitlynPiltoverPeacemaker.slot = SpellSlot::Q;
			CaitlynPiltoverPeacemaker.type = SpellType::linear;
			CaitlynPiltoverPeacemaker.range = 1250;
			CaitlynPiltoverPeacemaker.radius = 90;
			CaitlynPiltoverPeacemaker.delay = 0.625;
			CaitlynPiltoverPeacemaker.danger = 1;
			CaitlynPiltoverPeacemaker.speed = 2200;
			CaitlynPiltoverPeacemaker.cc = false;
			CaitlynPiltoverPeacemaker.collisionMinions = false;
			CaitlynPiltoverPeacemaker.windwall = true;
			CaitlynPiltoverPeacemaker.hitbox = true;
			CaitlynPiltoverPeacemaker.fow = true;
			CaitlynPiltoverPeacemaker.exception = false;
			CaitlynPiltoverPeacemaker.extend = true;
			Caitlyn.Spells.push_back(CaitlynPiltoverPeacemaker);
			Spell CaitlynYordleTrap;
			CaitlynYordleTrap.name = "CaitlynYordleTrap";
			CaitlynYordleTrap.icon = "CaitlynW";
			CaitlynYordleTrap.displayName = "Yordle Trap";
			CaitlynYordleTrap.missileName = "";
			CaitlynYordleTrap.slot = SpellSlot::W;
			CaitlynYordleTrap.type = SpellType::circular;
			CaitlynYordleTrap.range = 800;
			CaitlynYordleTrap.radius = 75;
			CaitlynYordleTrap.delay = 0.35;
			CaitlynYordleTrap.danger = 1;
			CaitlynYordleTrap.speed = MathHuge;
			CaitlynYordleTrap.cc = true;
			CaitlynYordleTrap.collisionMinions = false;
			CaitlynYordleTrap.windwall = false;
			CaitlynYordleTrap.hitbox = false;
			CaitlynYordleTrap.fow = false;
			CaitlynYordleTrap.exception = false;
			CaitlynYordleTrap.extend = false;
			Caitlyn.Spells.push_back(CaitlynYordleTrap);
			Spell CaitlynEntrapment;
			CaitlynEntrapment.name = "CaitlynEntrapment";
			CaitlynEntrapment.icon = "CaitlynE";
			CaitlynEntrapment.displayName = "Entrapment";
			CaitlynEntrapment.missileName = "CaitlynEntrapment";
			CaitlynEntrapment.slot = SpellSlot::E;
			CaitlynEntrapment.type = SpellType::linear;
			CaitlynEntrapment.range = 750;
			CaitlynEntrapment.radius = 70;
			CaitlynEntrapment.delay = 0.15;
			CaitlynEntrapment.danger = 2;
			CaitlynEntrapment.speed = 1600;
			CaitlynEntrapment.cc = true;
			CaitlynEntrapment.collisionMinions = true;
			CaitlynEntrapment.windwall = true;
			CaitlynEntrapment.hitbox = true;
			CaitlynEntrapment.fow = true;
			CaitlynEntrapment.exception = false;
			CaitlynEntrapment.extend = true;
			Caitlyn.Spells.push_back(CaitlynEntrapment);
			Core::SpellDB.emplace_back(Caitlyn);
		}
		{
			Champ Camille;
			Camille.Name = "Camille";
			Spell CamilleE;
			CamilleE.name = "CamilleE";
			CamilleE.icon = "CamilleE1";
			CamilleE.displayName = "Hookshot [First]";
			CamilleE.missileName = "CamilleEMissile";
			CamilleE.slot = SpellSlot::E;
			CamilleE.type = SpellType::linear;
			CamilleE.range = 800;
			CamilleE.radius = 60;
			CamilleE.delay = 0;
			CamilleE.danger = 1;
			CamilleE.speed = 1900;
			CamilleE.cc = false;
			CamilleE.collisionMinions = false;
			CamilleE.windwall = true;
			CamilleE.hitbox = true;
			CamilleE.fow = true;
			CamilleE.exception = false;
			CamilleE.extend = true;
			Camille.Spells.push_back(CamilleE);
			Spell CamilleEDash2;
			CamilleEDash2.name = "CamilleEDash2";
			CamilleEDash2.icon = "CamilleE2";
			CamilleEDash2.displayName = "Hookshot [Second]";
			CamilleEDash2.missileName = "";
			CamilleEDash2.slot = SpellSlot::E;
			CamilleEDash2.type = SpellType::linear;
			CamilleEDash2.range = 400;
			CamilleEDash2.radius = 60;
			CamilleEDash2.delay = 0;
			CamilleEDash2.danger = 2;
			CamilleEDash2.speed = 1900;
			CamilleEDash2.cc = true;
			CamilleEDash2.collisionMinions = false;
			CamilleEDash2.windwall = true;
			CamilleEDash2.hitbox = false;
			CamilleEDash2.fow = false;
			CamilleEDash2.exception = false;
			CamilleEDash2.extend = true;
			Camille.Spells.push_back(CamilleEDash2);
			Core::SpellDB.emplace_back(Camille);
		}
		{
			Champ Cassiopeia;
			Cassiopeia.Name = "Cassiopeia";
			Spell CassiopeiaQ;
			CassiopeiaQ.name = "CassiopeiaQ";
			CassiopeiaQ.icon = "CassiopeiaQ";
			CassiopeiaQ.displayName = "Noxious Blast";
			CassiopeiaQ.missileName = "";
			CassiopeiaQ.slot = SpellSlot::Q;
			CassiopeiaQ.type = SpellType::circular;
			CassiopeiaQ.range = 850;
			CassiopeiaQ.radius = 150;
			CassiopeiaQ.delay = 0.75;
			CassiopeiaQ.danger = 2;
			CassiopeiaQ.speed = MathHuge;
			CassiopeiaQ.cc = false;
			CassiopeiaQ.collisionMinions = false;
			CassiopeiaQ.windwall = false;
			CassiopeiaQ.hitbox = false;
			CassiopeiaQ.fow = false;
			CassiopeiaQ.exception = false;
			CassiopeiaQ.extend = false;
			Cassiopeia.Spells.push_back(CassiopeiaQ);
			Spell CassiopeiaW;
			CassiopeiaW.name = "CassiopeiaW";
			CassiopeiaW.icon = "CassiopeiaW";
			CassiopeiaW.displayName = "Miasma";
			CassiopeiaW.missileName = "";
			CassiopeiaW.slot = SpellSlot::W;
			CassiopeiaW.type = SpellType::circular;
			CassiopeiaW.range = 800;
			CassiopeiaW.radius = 160;
			CassiopeiaW.delay = 0.75;
			CassiopeiaW.danger = 2;
			CassiopeiaW.speed = 2500;
			CassiopeiaW.cc = true;
			CassiopeiaW.collisionMinions = false;
			CassiopeiaW.windwall = true;
			CassiopeiaW.hitbox = false;
			CassiopeiaW.fow = false;
			CassiopeiaW.exception = false;
			CassiopeiaW.extend = false;
			Cassiopeia.Spells.push_back(CassiopeiaW);
			Spell CassiopeiaR;
			CassiopeiaR.name = "CassiopeiaR";
			CassiopeiaR.icon = "CassiopeiaR";
			CassiopeiaR.displayName = "Petrifying Gaze";
			CassiopeiaR.missileName = "";
			CassiopeiaR.slot = SpellSlot::R;
			CassiopeiaR.type = SpellType::conic;
			CassiopeiaR.range = 825;
			CassiopeiaR.radius = 0;
			CassiopeiaR.delay = 0.5;
			CassiopeiaR.danger = 5;
			CassiopeiaR.speed = MathHuge;
			CassiopeiaR.cc = true;
			CassiopeiaR.collisionMinions = false;
			CassiopeiaR.windwall = false;
			CassiopeiaR.hitbox = false;
			CassiopeiaR.fow = false;
			CassiopeiaR.exception = false;
			CassiopeiaR.extend = true;
			Cassiopeia.Spells.push_back(CassiopeiaR);
			Core::SpellDB.emplace_back(Cassiopeia);
		}
		{
			Champ Chogath;
			Chogath.Name = "Chogath";
			Spell Rupture;
			Rupture.name = "Rupture";
			Rupture.icon = "ChogathQ";
			Rupture.displayName = "Rupture";
			Rupture.missileName = "";
			Rupture.slot = SpellSlot::Q;
			Rupture.type = SpellType::circular;
			Rupture.range = 950;
			Rupture.radius = 250;
			Rupture.delay = 1.2;
			Rupture.danger = 2;
			Rupture.speed = MathHuge;
			Rupture.cc = true;
			Rupture.collisionMinions = false;
			Rupture.windwall = false;
			Rupture.hitbox = false;
			Rupture.fow = false;
			Rupture.exception = false;
			Rupture.extend = false;
			Chogath.Spells.push_back(Rupture);
			Spell FeralScream;
			FeralScream.name = "FeralScream";
			FeralScream.icon = "ChogathW";
			FeralScream.displayName = "Feral Scream";
			FeralScream.missileName = "";
			FeralScream.slot = SpellSlot::W;
			FeralScream.type = SpellType::conic;
			FeralScream.range = 650;
			FeralScream.radius = 0;
			FeralScream.delay = 0.5;
			FeralScream.danger = 2;
			FeralScream.speed = MathHuge;
			FeralScream.cc = true;
			FeralScream.collisionMinions = false;
			FeralScream.windwall = false;
			FeralScream.hitbox = false;
			FeralScream.fow = false;
			FeralScream.exception = false;
			FeralScream.extend = true;
			Chogath.Spells.push_back(FeralScream);
			Core::SpellDB.emplace_back(Chogath);
		}
		{
			Champ Corki;
			Corki.Name = "Corki";
			Spell PhosphorusBomb;
			PhosphorusBomb.name = "PhosphorusBomb";
			PhosphorusBomb.icon = "CorkiQ";
			PhosphorusBomb.displayName = "Phosphorus Bomb";
			PhosphorusBomb.missileName = "PhosphorusBombMissile";
			PhosphorusBomb.slot = SpellSlot::Q;
			PhosphorusBomb.type = SpellType::circular;
			PhosphorusBomb.range = 825;
			PhosphorusBomb.radius = 250;
			PhosphorusBomb.delay = 0.25;
			PhosphorusBomb.danger = 2;
			PhosphorusBomb.speed = 1000;
			PhosphorusBomb.cc = false;
			PhosphorusBomb.collisionMinions = false;
			PhosphorusBomb.windwall = true;
			PhosphorusBomb.hitbox = false;
			PhosphorusBomb.fow = true;
			PhosphorusBomb.exception = false;
			PhosphorusBomb.extend = false;
			Corki.Spells.push_back(PhosphorusBomb);
			Spell MissileBarrageMissile;
			MissileBarrageMissile.name = "MissileBarrageMissile";
			MissileBarrageMissile.icon = "CorkiR1";
			MissileBarrageMissile.displayName = "Missile Barrage [Standard]";
			MissileBarrageMissile.missileName = "MissileBarrageMissile";
			MissileBarrageMissile.slot = SpellSlot::R;
			MissileBarrageMissile.type = SpellType::linear;
			MissileBarrageMissile.range = 1300;
			MissileBarrageMissile.radius = 40;
			MissileBarrageMissile.delay = 0.175;
			MissileBarrageMissile.danger = 1;
			MissileBarrageMissile.speed = 2000;
			MissileBarrageMissile.cc = false;
			MissileBarrageMissile.collisionMinions = true;
			MissileBarrageMissile.windwall = true;
			MissileBarrageMissile.hitbox = true;
			MissileBarrageMissile.fow = true;
			MissileBarrageMissile.exception = false;
			MissileBarrageMissile.extend = true;
			Corki.Spells.push_back(MissileBarrageMissile);
			Spell MissileBarrageMissile2;
			MissileBarrageMissile2.name = "MissileBarrageMissile2";
			MissileBarrageMissile2.icon = "CorkiR2";
			MissileBarrageMissile2.displayName = "Missile Barrage [Big]";
			MissileBarrageMissile2.missileName = "MissileBarrageMissile2";
			MissileBarrageMissile2.slot = SpellSlot::R;
			MissileBarrageMissile2.type = SpellType::linear;
			MissileBarrageMissile2.range = 1500;
			MissileBarrageMissile2.radius = 40;
			MissileBarrageMissile2.delay = 0.175;
			MissileBarrageMissile2.danger = 1;
			MissileBarrageMissile2.speed = 2000;
			MissileBarrageMissile2.cc = false;
			MissileBarrageMissile2.collisionMinions = true;
			MissileBarrageMissile2.windwall = true;
			MissileBarrageMissile2.hitbox = true;
			MissileBarrageMissile2.fow = true;
			MissileBarrageMissile2.exception = false;
			MissileBarrageMissile2.extend = true;
			Corki.Spells.push_back(MissileBarrageMissile2);
			Core::SpellDB.emplace_back(Corki);
		}
		{
			Champ Diana;
			Diana.Name = "Diana";
			Spell DianaQ;
			DianaQ.name = "DianaQ";
			DianaQ.icon = "DianaQ";
			DianaQ.displayName = "Crescent Strike";
			DianaQ.missileName = "";
			DianaQ.slot = SpellSlot::Q;
			DianaQ.type = SpellType::circular;
			DianaQ.range = 900;
			DianaQ.radius = 185;
			DianaQ.delay = 0.25;
			DianaQ.danger = 2;
			DianaQ.speed = 1900;
			DianaQ.cc = false;
			DianaQ.collisionMinions = true;
			DianaQ.windwall = true;
			DianaQ.hitbox = false;
			DianaQ.fow = false;
			DianaQ.exception = false;
			DianaQ.extend = false;
			Diana.Spells.push_back(DianaQ);
			Core::SpellDB.emplace_back(Diana);
		}
		{
			Champ Draven;
			Draven.Name = "Draven";
			Spell DravenDoubleShot;
			DravenDoubleShot.name = "DravenDoubleShot";
			DravenDoubleShot.icon = "DravenE";
			DravenDoubleShot.displayName = "Double Shot";
			DravenDoubleShot.missileName = "DravenDoubleShotMissile";
			DravenDoubleShot.slot = SpellSlot::E;
			DravenDoubleShot.type = SpellType::linear;
			DravenDoubleShot.range = 1050;
			DravenDoubleShot.radius = 130;
			DravenDoubleShot.delay = 0.25;
			DravenDoubleShot.danger = 3;
			DravenDoubleShot.speed = 1600;
			DravenDoubleShot.cc = true;
			DravenDoubleShot.collisionMinions = false;
			DravenDoubleShot.windwall = true;
			DravenDoubleShot.hitbox = true;
			DravenDoubleShot.fow = true;
			DravenDoubleShot.exception = false;
			DravenDoubleShot.extend = true;
			Draven.Spells.push_back(DravenDoubleShot);
			Spell DravenRCast;
			DravenRCast.name = "DravenRCast";
			DravenRCast.icon = "DravenR";
			DravenRCast.displayName = "Whirling Death";
			DravenRCast.missileName = "";
			DravenRCast.slot = SpellSlot::R;
			DravenRCast.type = SpellType::linear;
			DravenRCast.range = 12500;
			DravenRCast.radius = 160;
			DravenRCast.delay = 0.25;
			DravenRCast.danger = 4;
			DravenRCast.speed = 2000;
			DravenRCast.cc = false;
			DravenRCast.collisionMinions = false;
			DravenRCast.windwall = true;
			DravenRCast.hitbox = true;
			DravenRCast.fow = false;
			DravenRCast.exception = false;
			DravenRCast.extend = true;
			Draven.Spells.push_back(DravenRCast);
			Core::SpellDB.emplace_back(Draven);
		}
		{
			Champ DrMundo;
			DrMundo.Name = "DrMundo";
			Spell DrMundoQ;
			DrMundoQ.name = "DrMundoQ";
			DrMundoQ.icon = "DrMundoQ";
			DrMundoQ.displayName = "Infected Bonesaw";
			DrMundoQ.missileName = "DrMundoQ";
			DrMundoQ.slot = SpellSlot::Q;
			DrMundoQ.type = SpellType::linear;
			DrMundoQ.range = 990;
			DrMundoQ.radius = 120;
			DrMundoQ.delay = 0.25;
			DrMundoQ.danger = 2;
			DrMundoQ.speed = 2000;
			DrMundoQ.cc = true;
			DrMundoQ.collisionMinions = true;
			DrMundoQ.windwall = true;
			DrMundoQ.hitbox = true;
			DrMundoQ.fow = true;
			DrMundoQ.exception = false;
			DrMundoQ.extend = true;
			DrMundo.Spells.push_back(DrMundoQ);
			Core::SpellDB.emplace_back(DrMundo);
		}
		{
			Champ Ekko;
			Ekko.Name = "Ekko";
			Spell EkkoQ;
			EkkoQ.name = "EkkoQ";
			EkkoQ.icon = "EkkoQ";
			EkkoQ.displayName = "Timewinder";
			EkkoQ.missileName = "EkkoQMis";
			EkkoQ.slot = SpellSlot::Q;
			EkkoQ.type = SpellType::linear;
			EkkoQ.range = 1175;
			EkkoQ.radius = 60;
			EkkoQ.delay = 0.25;
			EkkoQ.danger = 1;
			EkkoQ.speed = 1650;
			EkkoQ.cc = true;
			EkkoQ.collisionMinions = false;
			EkkoQ.windwall = true;
			EkkoQ.hitbox = true;
			EkkoQ.fow = true;
			EkkoQ.exception = false;
			EkkoQ.extend = true;
			Ekko.Spells.push_back(EkkoQ);
			Spell EkkoW;
			EkkoW.name = "EkkoW";
			EkkoW.icon = "EkkoW";
			EkkoW.displayName = "Parallel Convergence";
			EkkoW.missileName = "";
			EkkoW.slot = SpellSlot::W;
			EkkoW.type = SpellType::circular;
			EkkoW.range = 1600;
			EkkoW.radius = 400;
			EkkoW.delay = 3.35;
			EkkoW.danger = 1;
			EkkoW.speed = MathHuge;
			EkkoW.cc = true;
			EkkoW.collisionMinions = false;
			EkkoW.windwall = false;
			EkkoW.hitbox = false;
			EkkoW.fow = false;
			EkkoW.exception = false;
			EkkoW.extend = false;
			Ekko.Spells.push_back(EkkoW);
			Core::SpellDB.emplace_back(Ekko);
		}
		{
			Champ Elise;
			Elise.Name = "Elise";
			Spell EliseHumanE;
			EliseHumanE.name = "EliseHumanE";
			EliseHumanE.icon = "EliseE";
			EliseHumanE.displayName = "Cocoon";
			EliseHumanE.missileName = "EliseHumanE";
			EliseHumanE.slot = SpellSlot::E;
			EliseHumanE.type = SpellType::linear;
			EliseHumanE.range = 1075;
			EliseHumanE.radius = 55;
			EliseHumanE.delay = 0.25;
			EliseHumanE.danger = 1;
			EliseHumanE.speed = 1600;
			EliseHumanE.cc = true;
			EliseHumanE.collisionMinions = true;
			EliseHumanE.windwall = true;
			EliseHumanE.hitbox = true;
			EliseHumanE.fow = true;
			EliseHumanE.exception = false;
			EliseHumanE.extend = true;
			Elise.Spells.push_back(EliseHumanE);
			Core::SpellDB.emplace_back(Elise);
		}
		{
			Champ Evelynn;
			Evelynn.Name = "Evelynn";
			Spell EvelynnQ;
			EvelynnQ.name = "EvelynnQ";
			EvelynnQ.icon = "EvelynnQ";
			EvelynnQ.displayName = "Hate Spike";
			EvelynnQ.missileName = "EvelynnQ";
			EvelynnQ.slot = SpellSlot::Q;
			EvelynnQ.type = SpellType::linear;
			EvelynnQ.range = 800;
			EvelynnQ.radius = 60;
			EvelynnQ.delay = 0.25;
			EvelynnQ.danger = 2;
			EvelynnQ.speed = 2400;
			EvelynnQ.cc = false;
			EvelynnQ.collisionMinions = true;
			EvelynnQ.windwall = true;
			EvelynnQ.hitbox = true;
			EvelynnQ.fow = true;
			EvelynnQ.exception = false;
			EvelynnQ.extend = true;
			Evelynn.Spells.push_back(EvelynnQ);
			Spell EvelynnR;
			EvelynnR.name = "EvelynnR";
			EvelynnR.icon = "EvelynnR";
			EvelynnR.displayName = "Last Caress";
			EvelynnR.missileName = "";
			EvelynnR.slot = SpellSlot::R;
			EvelynnR.type = SpellType::conic;
			EvelynnR.range = 450;
			EvelynnR.radius = 180;
			EvelynnR.delay = 0.35;
			EvelynnR.danger = 5;
			EvelynnR.speed = MathHuge;
			EvelynnR.cc = false;
			EvelynnR.collisionMinions = false;
			EvelynnR.windwall = false;
			EvelynnR.hitbox = false;
			EvelynnR.fow = false;
			EvelynnR.exception = false;
			EvelynnR.extend = true;
			Evelynn.Spells.push_back(EvelynnR);
			Core::SpellDB.emplace_back(Evelynn);
		}
		{
			Champ Ezreal;
			Ezreal.Name = "Ezreal";
			Spell EzrealQ;
			EzrealQ.name = "EzrealQ";
			EzrealQ.icon = "EzrealQ";
			EzrealQ.displayName = "Mystic Shot";
			EzrealQ.missileName = "EzrealQ";
			EzrealQ.slot = SpellSlot::Q;
			EzrealQ.type = SpellType::linear;
			EzrealQ.range = 1150;
			EzrealQ.radius = 60;
			EzrealQ.delay = 0.25;
			EzrealQ.danger = 1;
			EzrealQ.speed = 2000;
			EzrealQ.cc = false;
			EzrealQ.collisionMinions = true;
			EzrealQ.windwall = true;
			EzrealQ.hitbox = true;
			EzrealQ.fow = true;
			EzrealQ.exception = false;
			EzrealQ.extend = true;
			Ezreal.Spells.push_back(EzrealQ);
			Spell EzrealW;
			EzrealW.name = "EzrealW";
			EzrealW.icon = "EzrealW";
			EzrealW.displayName = "Essence Flux";
			EzrealW.missileName = "EzrealW";
			EzrealW.slot = SpellSlot::W;
			EzrealW.type = SpellType::linear;
			EzrealW.range = 1150;
			EzrealW.radius = 60;
			EzrealW.delay = 0.25;
			EzrealW.danger = 1;
			EzrealW.speed = 2000;
			EzrealW.cc = false;
			EzrealW.collisionMinions = false;
			EzrealW.windwall = true;
			EzrealW.hitbox = true;
			EzrealW.fow = true;
			EzrealW.exception = false;
			EzrealW.extend = true;
			Ezreal.Spells.push_back(EzrealW);
			Spell EzrealR;
			EzrealR.name = "EzrealR";
			EzrealR.icon = "EzrealR";
			EzrealR.displayName = "Trueshot Barrage";
			EzrealR.missileName = "EzrealR";
			EzrealR.slot = SpellSlot::R;
			EzrealR.type = SpellType::linear;
			EzrealR.range = 12500;
			EzrealR.radius = 160;
			EzrealR.delay = 1;
			EzrealR.danger = 4;
			EzrealR.speed = 2000;
			EzrealR.cc = false;
			EzrealR.collisionMinions = false;
			EzrealR.windwall = true;
			EzrealR.hitbox = true;
			EzrealR.fow = true;
			EzrealR.exception = false;
			EzrealR.extend = true;
			Ezreal.Spells.push_back(EzrealR);
			Core::SpellDB.emplace_back(Ezreal);
		}
		{
			Champ Fiora;
			Fiora.Name = "Fiora";
			Spell FioraW;
			FioraW.name = "FioraW";
			FioraW.icon = "FioraW";
			FioraW.displayName = "Riposte";
			FioraW.missileName = "";
			FioraW.slot = SpellSlot::W;
			FioraW.type = SpellType::linear;
			FioraW.range = 750;
			FioraW.radius = 70;
			FioraW.delay = 0.75;
			FioraW.danger = 2;
			FioraW.speed = 3200;
			FioraW.cc = true;
			FioraW.collisionMinions = false;
			FioraW.windwall = true;
			FioraW.hitbox = false;
			FioraW.fow = false;
			FioraW.exception = false;
			FioraW.extend = true;
			Fiora.Spells.push_back(FioraW);
			Core::SpellDB.emplace_back(Fiora);
		}
		{
			Champ Fizz;
			Fizz.Name = "Fizz";
			Spell FizzR;
			FizzR.name = "FizzR";
			FizzR.icon = "FizzR";
			FizzR.displayName = "Chum the Waters";
			FizzR.missileName = "FizzRMissile";
			FizzR.slot = SpellSlot::R;
			FizzR.type = SpellType::linear;
			FizzR.range = 1300;
			FizzR.radius = 150;
			FizzR.delay = 0.25;
			FizzR.danger = 5;
			FizzR.speed = 1300;
			FizzR.cc = true;
			FizzR.collisionMinions = false;
			FizzR.windwall = true;
			FizzR.hitbox = true;
			FizzR.fow = true;
			FizzR.exception = false;
			FizzR.extend = true;
			Fizz.Spells.push_back(FizzR);
			Core::SpellDB.emplace_back(Fizz);
		}
		{
			Champ Galio;
			Galio.Name = "Galio";
			Spell GalioQ;
			GalioQ.name = "GalioQ";
			GalioQ.icon = "GalioQ";
			GalioQ.displayName = "Winds of War";
			GalioQ.missileName = "GalioQMissile";
			GalioQ.slot = SpellSlot::Q;
			GalioQ.type = SpellType::circular;
			GalioQ.range = 825;
			GalioQ.radius = 235;
			GalioQ.delay = 0.25;
			GalioQ.danger = 2;
			GalioQ.speed = 1150;
			GalioQ.cc = false;
			GalioQ.collisionMinions = false;
			GalioQ.windwall = true;
			GalioQ.hitbox = false;
			GalioQ.fow = true;
			GalioQ.exception = false;
			GalioQ.extend = false;
			Galio.Spells.push_back(GalioQ);
			Spell GalioE;
			GalioE.name = "GalioE";
			GalioE.icon = "GalioE";
			GalioE.displayName = "Justice Punch";
			GalioE.missileName = "";
			GalioE.slot = SpellSlot::E;
			GalioE.type = SpellType::linear;
			GalioE.range = 650;
			GalioE.radius = 160;
			GalioE.delay = 0.4;
			GalioE.danger = 3;
			GalioE.speed = 2300;
			GalioE.cc = true;
			GalioE.collisionMinions = false;
			GalioE.windwall = false;
			GalioE.hitbox = false;
			GalioE.fow = false;
			GalioE.exception = false;
			GalioE.extend = true;
			Galio.Spells.push_back(GalioE);
			Core::SpellDB.emplace_back(Galio);
		}
		{
			Champ Gnar;
			Gnar.Name = "Gnar";
			Spell GnarQMissile;
			GnarQMissile.name = "GnarQMissile";
			GnarQMissile.icon = "GnarQMini";
			GnarQMissile.displayName = "Boomerang Throw";
			GnarQMissile.missileName = "GnarQMissile";
			GnarQMissile.slot = SpellSlot::Q;
			GnarQMissile.type = SpellType::linear;
			GnarQMissile.range = 1125;
			GnarQMissile.radius = 55;
			GnarQMissile.delay = 0.25;
			GnarQMissile.danger = 2;
			GnarQMissile.speed = 2500;
			GnarQMissile.cc = true;
			GnarQMissile.collisionMinions = false;
			GnarQMissile.windwall = true;
			GnarQMissile.hitbox = true;
			GnarQMissile.fow = true;
			GnarQMissile.exception = false;
			GnarQMissile.extend = true;
			Gnar.Spells.push_back(GnarQMissile);
			Spell GnarBigQMissile;
			GnarBigQMissile.name = "GnarBigQMissile";
			GnarBigQMissile.icon = "GnarQMega";
			GnarBigQMissile.displayName = "Boulder Toss";
			GnarBigQMissile.missileName = "GnarBigQMissile";
			GnarBigQMissile.slot = SpellSlot::Q;
			GnarBigQMissile.type = SpellType::linear;
			GnarBigQMissile.range = 1125;
			GnarBigQMissile.radius = 90;
			GnarBigQMissile.delay = 0.5;
			GnarBigQMissile.danger = 2;
			GnarBigQMissile.speed = 2100;
			GnarBigQMissile.cc = true;
			GnarBigQMissile.collisionMinions = true;
			GnarBigQMissile.windwall = true;
			GnarBigQMissile.hitbox = true;
			GnarBigQMissile.fow = true;
			GnarBigQMissile.exception = false;
			GnarBigQMissile.extend = true;
			Gnar.Spells.push_back(GnarBigQMissile);
			Spell GnarBigW;
			GnarBigW.name = "GnarBigW";
			GnarBigW.icon = "GnarWMega";
			GnarBigW.displayName = "Wallop";
			GnarBigW.missileName = "";
			GnarBigW.slot = SpellSlot::W;
			GnarBigW.type = SpellType::linear;
			GnarBigW.range = 575;
			GnarBigW.radius = 100;
			GnarBigW.delay = 0.6;
			GnarBigW.danger = 3;
			GnarBigW.speed = MathHuge;
			GnarBigW.cc = true;
			GnarBigW.collisionMinions = false;
			GnarBigW.windwall = false;
			GnarBigW.hitbox = false;
			GnarBigW.fow = false;
			GnarBigW.exception = false;
			GnarBigW.extend = true;
			Gnar.Spells.push_back(GnarBigW);
			Spell GnarR;
			GnarR.name = "GnarR";
			GnarR.icon = "GnarR";
			GnarR.displayName = "GNAR!";
			GnarR.missileName = "";
			GnarR.slot = SpellSlot::R;
			GnarR.type = SpellType::circular;
			GnarR.range = 0;
			GnarR.radius = 475;
			GnarR.delay = 0.25;
			GnarR.danger = 5;
			GnarR.speed = MathHuge;
			GnarR.cc = true;
			GnarR.collisionMinions = false;
			GnarR.windwall = false;
			GnarR.hitbox = false;
			GnarR.fow = false;
			GnarR.exception = false;
			GnarR.extend = false;
			Gnar.Spells.push_back(GnarR);
			Core::SpellDB.emplace_back(Gnar);
		}
		{
			Champ Gragas;
			Gragas.Name = "Gragas";
			Spell GragasQ;
			GragasQ.name = "GragasQ";
			GragasQ.icon = "GragasQ";
			GragasQ.displayName = "Barrel Roll";
			GragasQ.missileName = "GragasQMissile";
			GragasQ.slot = SpellSlot::Q;
			GragasQ.type = SpellType::circular;
			GragasQ.range = 850;
			GragasQ.radius = 275;
			GragasQ.delay = 0.25;
			GragasQ.danger = 2;
			GragasQ.speed = 1000;
			GragasQ.cc = true;
			GragasQ.collisionMinions = false;
			GragasQ.windwall = true;
			GragasQ.hitbox = false;
			GragasQ.fow = true;
			GragasQ.exception = false;
			GragasQ.extend = false;
			Gragas.Spells.push_back(GragasQ);
			Spell GragasR;
			GragasR.name = "GragasR";
			GragasR.icon = "GragasR";
			GragasR.displayName = "Explosive Cask";
			GragasR.missileName = "GragasRBoom";
			GragasR.slot = SpellSlot::R;
			GragasR.type = SpellType::circular;
			GragasR.range = 1000;
			GragasR.radius = 400;
			GragasR.delay = 0.25;
			GragasR.danger = 5;
			GragasR.speed = 1800;
			GragasR.cc = true;
			GragasR.collisionMinions = false;
			GragasR.windwall = true;
			GragasR.hitbox = false;
			GragasR.fow = true;
			GragasR.exception = false;
			GragasR.extend = false;
			Gragas.Spells.push_back(GragasR);
			Core::SpellDB.emplace_back(Gragas);
		}
		{
			Champ Graves;
			Graves.Name = "Graves";
			Spell GravesQLineSpell;
			GravesQLineSpell.name = "GravesQLineSpell";
			GravesQLineSpell.icon = "GravesQ";
			GravesQLineSpell.displayName = "End of the Line";
			GravesQLineSpell.missileName = "";
			GravesQLineSpell.slot = SpellSlot::Q;
			GravesQLineSpell.type = SpellType::polygon;
			GravesQLineSpell.range = 800;
			GravesQLineSpell.radius = 20;
			GravesQLineSpell.delay = 1.4;
			GravesQLineSpell.danger = 1;
			GravesQLineSpell.speed = MathHuge;
			GravesQLineSpell.cc = false;
			GravesQLineSpell.collisionMinions = false;
			GravesQLineSpell.windwall = true;
			GravesQLineSpell.hitbox = true;
			GravesQLineSpell.fow = false;
			GravesQLineSpell.exception = false;
			GravesQLineSpell.extend = true;
			Graves.Spells.push_back(GravesQLineSpell);
			Spell GravesSmokeGrenade;
			GravesSmokeGrenade.name = "GravesSmokeGrenade";
			GravesSmokeGrenade.icon = "GravesW";
			GravesSmokeGrenade.displayName = "Smoke Grenade";
			GravesSmokeGrenade.missileName = "GravesSmokeGrenadeBoom";
			GravesSmokeGrenade.slot = SpellSlot::W;
			GravesSmokeGrenade.type = SpellType::circular;
			GravesSmokeGrenade.range = 950;
			GravesSmokeGrenade.radius = 250;
			GravesSmokeGrenade.delay = 0.15;
			GravesSmokeGrenade.danger = 2;
			GravesSmokeGrenade.speed = 1500;
			GravesSmokeGrenade.cc = true;
			GravesSmokeGrenade.collisionMinions = false;
			GravesSmokeGrenade.windwall = true;
			GravesSmokeGrenade.hitbox = false;
			GravesSmokeGrenade.fow = true;
			GravesSmokeGrenade.exception = false;
			GravesSmokeGrenade.extend = false;
			Graves.Spells.push_back(GravesSmokeGrenade);
			Spell GravesChargeShot;
			GravesChargeShot.name = "GravesChargeShot";
			GravesChargeShot.icon = "GravesR";
			GravesChargeShot.displayName = "Charge Shot";
			GravesChargeShot.missileName = "GravesChargeShotShot";
			GravesChargeShot.slot = SpellSlot::R;
			GravesChargeShot.type = SpellType::polygon;
			GravesChargeShot.range = 1000;
			GravesChargeShot.radius = 100;
			GravesChargeShot.delay = 0.25;
			GravesChargeShot.danger = 5;
			GravesChargeShot.speed = 2100;
			GravesChargeShot.cc = false;
			GravesChargeShot.collisionMinions = false;
			GravesChargeShot.windwall = true;
			GravesChargeShot.hitbox = true;
			GravesChargeShot.fow = true;
			GravesChargeShot.exception = false;
			GravesChargeShot.extend = true;
			Graves.Spells.push_back(GravesChargeShot);
			Core::SpellDB.emplace_back(Graves);
		}
		{
			Champ Gwen;
			Gwen.Name = "Gwen";
			Spell GwenQ;
			GwenQ.name = "GwenQ";
			GwenQ.icon = "GwenQ";
			GwenQ.displayName = "Snip Snip!";
			GwenQ.missileName = "";
			GwenQ.slot = SpellSlot::Q;
			GwenQ.type = SpellType::circular;
			GwenQ.range = 450;
			GwenQ.radius = 275;
			GwenQ.delay = 0;
			GwenQ.danger = 2;
			GwenQ.speed = 1500;
			GwenQ.cc = false;
			GwenQ.collisionMinions = false;
			GwenQ.windwall = true;
			GwenQ.hitbox = false;
			GwenQ.fow = true;
			GwenQ.exception = false;
			GwenQ.extend = true;
			Gwen.Spells.push_back(GwenQ);
			Spell GwenR;
			GwenR.name = "GwenR";
			GwenR.icon = "GwenR";
			GwenR.displayName = "Needlework";
			GwenR.missileName = "GwenRMissile";
			GwenR.slot = SpellSlot::R;
			GwenR.type = SpellType::linear;
			GwenR.range = 1230;
			GwenR.radius = 250;
			GwenR.delay = 0.25;
			GwenR.danger = 3;
			GwenR.speed = 1800;
			GwenR.cc = true;
			GwenR.collisionMinions = false;
			GwenR.windwall = true;
			GwenR.hitbox = false;
			GwenR.fow = true;
			GwenR.exception = false;
			GwenR.extend = true;
			Gwen.Spells.push_back(GwenR);
			Core::SpellDB.emplace_back(Gwen);
		}
		{
			Champ Hecarim;
			Hecarim.Name = "Hecarim";
			Spell HecarimUlt;
			HecarimUlt.name = "HecarimUlt";
			HecarimUlt.icon = "HecarimR";
			HecarimUlt.displayName = "Onslaught of Shadows";
			HecarimUlt.missileName = "HecarimUltMissile";
			HecarimUlt.slot = SpellSlot::R;
			HecarimUlt.type = SpellType::linear;
			HecarimUlt.range = 1650;
			HecarimUlt.radius = 280;
			HecarimUlt.delay = 0.2;
			HecarimUlt.danger = 4;
			HecarimUlt.speed = 1100;
			HecarimUlt.cc = true;
			HecarimUlt.collisionMinions = false;
			HecarimUlt.windwall = false;
			HecarimUlt.hitbox = false;
			HecarimUlt.fow = true;
			HecarimUlt.exception = false;
			HecarimUlt.extend = true;
			Hecarim.Spells.push_back(HecarimUlt);
			Core::SpellDB.emplace_back(Hecarim);
		}
		{
			Champ Heimerdinger;
			Heimerdinger.Name = "Heimerdinger";
			Spell HeimerdingerW;
			HeimerdingerW.name = "HeimerdingerW";
			HeimerdingerW.icon = "HeimerdingerW";
			HeimerdingerW.displayName = "Hextech Micro-Rockets";
			HeimerdingerW.missileName = "";
			HeimerdingerW.slot = SpellSlot::W;
			HeimerdingerW.type = SpellType::linear;
			HeimerdingerW.range = 1325;
			HeimerdingerW.radius = 100;
			HeimerdingerW.delay = 0.25;
			HeimerdingerW.danger = 2;
			HeimerdingerW.speed = 2050;
			HeimerdingerW.cc = false;
			HeimerdingerW.collisionMinions = false;
			HeimerdingerW.windwall = true;
			HeimerdingerW.hitbox = true;
			HeimerdingerW.fow = false;
			HeimerdingerW.exception = false;
			HeimerdingerW.extend = true;
			Heimerdinger.Spells.push_back(HeimerdingerW);
			Spell HeimerdingerE;
			HeimerdingerE.name = "HeimerdingerE";
			HeimerdingerE.icon = "HeimerdingerE1";
			HeimerdingerE.displayName = "CH-2 Electron Storm Grenade";
			HeimerdingerE.missileName = "HeimerdingerESpell";
			HeimerdingerE.slot = SpellSlot::E;
			HeimerdingerE.type = SpellType::circular;
			HeimerdingerE.range = 970;
			HeimerdingerE.radius = 250;
			HeimerdingerE.delay = 0.25;
			HeimerdingerE.danger = 2;
			HeimerdingerE.speed = 1200;
			HeimerdingerE.cc = true;
			HeimerdingerE.collisionMinions = false;
			HeimerdingerE.windwall = true;
			HeimerdingerE.hitbox = false;
			HeimerdingerE.fow = true;
			HeimerdingerE.exception = false;
			HeimerdingerE.extend = false;
			Heimerdinger.Spells.push_back(HeimerdingerE);
			Spell HeimerdingerEUlt;
			HeimerdingerEUlt.name = "HeimerdingerEUlt";
			HeimerdingerEUlt.icon = "HeimerdingerE2";
			HeimerdingerEUlt.displayName = "CH-2 Electron Storm Grenade [Ult]";
			HeimerdingerEUlt.missileName = "HeimerdingerESpell_ult";
			HeimerdingerEUlt.slot = SpellSlot::E;
			HeimerdingerEUlt.type = SpellType::circular;
			HeimerdingerEUlt.range = 970;
			HeimerdingerEUlt.radius = 250;
			HeimerdingerEUlt.delay = 0.25;
			HeimerdingerEUlt.danger = 3;
			HeimerdingerEUlt.speed = 1200;
			HeimerdingerEUlt.cc = true;
			HeimerdingerEUlt.collisionMinions = false;
			HeimerdingerEUlt.windwall = true;
			HeimerdingerEUlt.hitbox = false;
			HeimerdingerEUlt.fow = true;
			HeimerdingerEUlt.exception = false;
			HeimerdingerEUlt.extend = false;
			Heimerdinger.Spells.push_back(HeimerdingerEUlt);
			Core::SpellDB.emplace_back(Heimerdinger);
		}
		{
			Champ Illaoi;
			Illaoi.Name = "Illaoi";
			Spell IllaoiQ;
			IllaoiQ.name = "IllaoiQ";
			IllaoiQ.icon = "IllaoiQ";
			IllaoiQ.displayName = "Tentacle Smash";
			IllaoiQ.missileName = "";
			IllaoiQ.slot = SpellSlot::Q;
			IllaoiQ.type = SpellType::linear;
			IllaoiQ.range = 850;
			IllaoiQ.radius = 100;
			IllaoiQ.delay = 0.75;
			IllaoiQ.danger = 2;
			IllaoiQ.speed = MathHuge;
			IllaoiQ.cc = false;
			IllaoiQ.collisionMinions = true;
			IllaoiQ.windwall = false;
			IllaoiQ.hitbox = false;
			IllaoiQ.fow = false;
			IllaoiQ.exception = false;
			IllaoiQ.extend = true;
			Illaoi.Spells.push_back(IllaoiQ);
			Spell IllaoiE;
			IllaoiE.name = "IllaoiE";
			IllaoiE.icon = "IllaoiE";
			IllaoiE.displayName = "Test of Spirit";
			IllaoiE.missileName = "IllaoiEMis";
			IllaoiE.slot = SpellSlot::E;
			IllaoiE.type = SpellType::linear;
			IllaoiE.range = 900;
			IllaoiE.radius = 50;
			IllaoiE.delay = 0.25;
			IllaoiE.danger = 1;
			IllaoiE.speed = 1900;
			IllaoiE.cc = false;
			IllaoiE.collisionMinions = true;
			IllaoiE.windwall = true;
			IllaoiE.hitbox = false;
			IllaoiE.fow = true;
			IllaoiE.exception = false;
			IllaoiE.extend = true;
			Illaoi.Spells.push_back(IllaoiE);
			Core::SpellDB.emplace_back(Illaoi);
		}
		{
			Champ Irelia;
			Irelia.Name = "Irelia";
			Spell IreliaW2;
			IreliaW2.name = "IreliaW2";
			IreliaW2.icon = "IreliaW";
			IreliaW2.displayName = "Defiant Dance";
			IreliaW2.missileName = "";
			IreliaW2.slot = SpellSlot::W;
			IreliaW2.type = SpellType::linear;
			IreliaW2.range = 825;
			IreliaW2.radius = 120;
			IreliaW2.delay = 0.25;
			IreliaW2.danger = 3;
			IreliaW2.speed = MathHuge;
			IreliaW2.cc = true;
			IreliaW2.collisionMinions = false;
			IreliaW2.windwall = false;
			IreliaW2.hitbox = false;
			IreliaW2.fow = false;
			IreliaW2.exception = false;
			IreliaW2.extend = true;
			Irelia.Spells.push_back(IreliaW2);
			Spell IreliaR;
			IreliaR.name = "IreliaR";
			IreliaR.icon = "IreliaR";
			IreliaR.displayName = "Vanguard's Edge";
			IreliaR.missileName = "IreliaR";
			IreliaR.slot = SpellSlot::R;
			IreliaR.type = SpellType::linear;
			IreliaR.range = 950;
			IreliaR.radius = 160;
			IreliaR.delay = 0.4;
			IreliaR.danger = 4;
			IreliaR.speed = 2000;
			IreliaR.cc = true;
			IreliaR.collisionMinions = false;
			IreliaR.windwall = true;
			IreliaR.hitbox = false;
			IreliaR.fow = true;
			IreliaR.exception = false;
			IreliaR.extend = true;
			Irelia.Spells.push_back(IreliaR);
			Core::SpellDB.emplace_back(Irelia);
		}
		{
			Champ Ivern;
			Ivern.Name = "Ivern";
			Spell IvernQ;
			IvernQ.name = "IvernQ";
			IvernQ.icon = "IvernQ";
			IvernQ.displayName = "Rootcaller";
			IvernQ.missileName = "IvernQ";
			IvernQ.slot = SpellSlot::Q;
			IvernQ.type = SpellType::linear;
			IvernQ.range = 1075;
			IvernQ.radius = 80;
			IvernQ.delay = 0.25;
			IvernQ.danger = 1;
			IvernQ.speed = 1300;
			IvernQ.cc = true;
			IvernQ.collisionMinions = true;
			IvernQ.windwall = true;
			IvernQ.hitbox = true;
			IvernQ.fow = true;
			IvernQ.exception = false;
			IvernQ.extend = true;
			Ivern.Spells.push_back(IvernQ);
			Core::SpellDB.emplace_back(Ivern);
		}
		{
			Champ Janna;
			Janna.Name = "Janna";
			Spell HowlingGaleSpell;
			HowlingGaleSpell.name = "HowlingGaleSpell";
			HowlingGaleSpell.icon = "JannaQ";
			HowlingGaleSpell.displayName = "Howling Gale";
			HowlingGaleSpell.missileName = "HowlingGaleSpell";
			HowlingGaleSpell.slot = SpellSlot::Q;
			HowlingGaleSpell.type = SpellType::linear;
			HowlingGaleSpell.range = 1750;
			HowlingGaleSpell.radius = 100;
			HowlingGaleSpell.delay = 0;
			HowlingGaleSpell.danger = 2;
			HowlingGaleSpell.speed = 667;
			HowlingGaleSpell.cc = true;
			HowlingGaleSpell.collisionMinions = false;
			HowlingGaleSpell.windwall = true;
			HowlingGaleSpell.hitbox = false;
			HowlingGaleSpell.fow = true;
			HowlingGaleSpell.exception = true;
			HowlingGaleSpell.extend = false;
			Janna.Spells.push_back(HowlingGaleSpell);
			Core::SpellDB.emplace_back(Janna);
		}
		{
			Champ JarvanIV;
			JarvanIV.Name = "JarvanIV";
			Spell JarvanIVDragonStrike;
			JarvanIVDragonStrike.name = "JarvanIVDragonStrike";
			JarvanIVDragonStrike.icon = "JarvanIVQ";
			JarvanIVDragonStrike.displayName = "Dragon Strike";
			JarvanIVDragonStrike.missileName = "";
			JarvanIVDragonStrike.slot = SpellSlot::Q;
			JarvanIVDragonStrike.type = SpellType::linear;
			JarvanIVDragonStrike.range = 770;
			JarvanIVDragonStrike.radius = 70;
			JarvanIVDragonStrike.delay = 0.4;
			JarvanIVDragonStrike.danger = 2;
			JarvanIVDragonStrike.speed = MathHuge;
			JarvanIVDragonStrike.cc = true;
			JarvanIVDragonStrike.collisionMinions = false;
			JarvanIVDragonStrike.windwall = false;
			JarvanIVDragonStrike.hitbox = false;
			JarvanIVDragonStrike.fow = false;
			JarvanIVDragonStrike.exception = false;
			JarvanIVDragonStrike.extend = true;
			JarvanIV.Spells.push_back(JarvanIVDragonStrike);
			Spell JarvanIVDemacianStandard;
			JarvanIVDemacianStandard.name = "JarvanIVDemacianStandard";
			JarvanIVDemacianStandard.icon = "JarvanIVE";
			JarvanIVDemacianStandard.displayName = "Demacian Standard";
			JarvanIVDemacianStandard.missileName = "";
			JarvanIVDemacianStandard.slot = SpellSlot::E;
			JarvanIVDemacianStandard.type = SpellType::circular;
			JarvanIVDemacianStandard.range = 860;
			JarvanIVDemacianStandard.radius = 175;
			JarvanIVDemacianStandard.delay = 0;
			JarvanIVDemacianStandard.danger = 2;
			JarvanIVDemacianStandard.speed = 3440;
			JarvanIVDemacianStandard.cc = false;
			JarvanIVDemacianStandard.collisionMinions = false;
			JarvanIVDemacianStandard.windwall = false;
			JarvanIVDemacianStandard.hitbox = false;
			JarvanIVDemacianStandard.fow = false;
			JarvanIVDemacianStandard.exception = false;
			JarvanIVDemacianStandard.extend = false;
			JarvanIV.Spells.push_back(JarvanIVDemacianStandard);
			Core::SpellDB.emplace_back(JarvanIV);
		}
		{
			Champ Jayce;
			Jayce.Name = "Jayce";
			Spell JayceShockBlast;
			JayceShockBlast.name = "JayceShockBlast";
			JayceShockBlast.icon = "JayceQ";
			JayceShockBlast.displayName = "Shock Blast [Standard]";
			JayceShockBlast.missileName = "JayceShockBlastMis";
			JayceShockBlast.slot = SpellSlot::Q;
			JayceShockBlast.type = SpellType::linear;
			JayceShockBlast.range = 1050;
			JayceShockBlast.radius = 70;
			JayceShockBlast.delay = 0.214;
			JayceShockBlast.danger = 1;
			JayceShockBlast.speed = 1450;
			JayceShockBlast.cc = false;
			JayceShockBlast.collisionMinions = true;
			JayceShockBlast.windwall = true;
			JayceShockBlast.hitbox = true;
			JayceShockBlast.fow = true;
			JayceShockBlast.exception = false;
			JayceShockBlast.extend = true;
			Jayce.Spells.push_back(JayceShockBlast);
			Spell JayceShockBlastWallMis;
			JayceShockBlastWallMis.name = "JayceShockBlastWallMis";
			JayceShockBlastWallMis.icon = "JayceQ";
			JayceShockBlastWallMis.displayName = "Shock Blast [Accelerated]";
			JayceShockBlastWallMis.missileName = "JayceShockBlastWallMis";
			JayceShockBlastWallMis.slot = SpellSlot::Q;
			JayceShockBlastWallMis.type = SpellType::linear;
			JayceShockBlastWallMis.range = 1600;
			JayceShockBlastWallMis.radius = 115;
			JayceShockBlastWallMis.delay = 0.152;
			JayceShockBlastWallMis.danger = 3;
			JayceShockBlastWallMis.speed = 2350;
			JayceShockBlastWallMis.cc = false;
			JayceShockBlastWallMis.collisionMinions = true;
			JayceShockBlastWallMis.windwall = true;
			JayceShockBlastWallMis.hitbox = true;
			JayceShockBlastWallMis.fow = true;
			JayceShockBlastWallMis.exception = true;
			JayceShockBlastWallMis.extend = false;
			Jayce.Spells.push_back(JayceShockBlastWallMis);
			Core::SpellDB.emplace_back(Jayce);
		}
		{
			Champ Jhin;
			Jhin.Name = "Jhin";
			Spell JhinW;
			JhinW.name = "JhinW";
			JhinW.icon = "JhinW";
			JhinW.displayName = "Deadly Flourish";
			JhinW.missileName = "";
			JhinW.slot = SpellSlot::W;
			JhinW.type = SpellType::linear;
			JhinW.range = 2550;
			JhinW.radius = 40;
			JhinW.delay = 0.75;
			JhinW.danger = 1;
			JhinW.speed = 5000;
			JhinW.cc = true;
			JhinW.collisionMinions = false;
			JhinW.windwall = true;
			JhinW.hitbox = false;
			JhinW.fow = false;
			JhinW.exception = false;
			JhinW.extend = true;
			Jhin.Spells.push_back(JhinW);
			Spell JhinE;
			JhinE.name = "JhinE";
			JhinE.icon = "JhinE";
			JhinE.displayName = "Captive Audience";
			JhinE.missileName = "JhinETrap";
			JhinE.slot = SpellSlot::E;
			JhinE.type = SpellType::circular;
			JhinE.range = 750;
			JhinE.radius = 130;
			JhinE.delay = 0.25;
			JhinE.danger = 1;
			JhinE.speed = 1600;
			JhinE.cc = true;
			JhinE.collisionMinions = false;
			JhinE.windwall = false;
			JhinE.hitbox = false;
			JhinE.fow = true;
			JhinE.exception = false;
			JhinE.extend = false;
			Jhin.Spells.push_back(JhinE);
			Spell JhinRShot;
			JhinRShot.name = "JhinRShot";
			JhinRShot.icon = "JhinR";
			JhinRShot.displayName = "Curtain Call";
			JhinRShot.missileName = "JhinRShotMis";
			JhinRShot.slot = SpellSlot::R;
			JhinRShot.type = SpellType::linear;
			JhinRShot.range = 3500;
			JhinRShot.radius = 80;
			JhinRShot.delay = 0.25;
			JhinRShot.danger = 2;
			JhinRShot.speed = 5000;
			JhinRShot.cc = true;
			JhinRShot.collisionMinions = false;
			JhinRShot.windwall = true;
			JhinRShot.hitbox = true;
			JhinRShot.fow = true;
			JhinRShot.exception = false;
			JhinRShot.extend = true;
			Jhin.Spells.push_back(JhinRShot);
			Core::SpellDB.emplace_back(Jhin);
		}
		{
			Champ Jinx;
			Jinx.Name = "Jinx";
			Spell JinxWMissile;
			JinxWMissile.name = "JinxWMissile";
			JinxWMissile.icon = "JinxW";
			JinxWMissile.displayName = "Zap!";
			JinxWMissile.missileName = "JinxWMissile";
			JinxWMissile.slot = SpellSlot::W;
			JinxWMissile.type = SpellType::linear;
			JinxWMissile.range = 1450;
			JinxWMissile.radius = 60;
			JinxWMissile.delay = 0.6;
			JinxWMissile.danger = 1;
			JinxWMissile.speed = 3300;
			JinxWMissile.cc = true;
			JinxWMissile.collisionMinions = true;
			JinxWMissile.windwall = true;
			JinxWMissile.hitbox = true;
			JinxWMissile.fow = true;
			JinxWMissile.exception = false;
			JinxWMissile.extend = true;
			Jinx.Spells.push_back(JinxWMissile);
			Spell JinxEHit;
			JinxEHit.name = "JinxEHit";
			JinxEHit.icon = "JinxE";
			JinxEHit.displayName = "Flame Chompers!";
			JinxEHit.missileName = "JinxEHit";
			JinxEHit.slot = SpellSlot::E;
			JinxEHit.type = SpellType::polygon;
			JinxEHit.range = 900;
			JinxEHit.radius = 120;
			JinxEHit.delay = 1.5;
			JinxEHit.danger = 1;
			JinxEHit.speed = 1100;
			JinxEHit.cc = true;
			JinxEHit.collisionMinions = false;
			JinxEHit.windwall = true;
			JinxEHit.hitbox = false;
			JinxEHit.fow = true;
			JinxEHit.exception = false;
			JinxEHit.extend = true;
			Jinx.Spells.push_back(JinxEHit);
			Spell JinxR;
			JinxR.name = "JinxR";
			JinxR.icon = "JinxR";
			JinxR.displayName = "Super Mega Death Rocket!";
			JinxR.missileName = "JinxR";
			JinxR.slot = SpellSlot::R;
			JinxR.type = SpellType::linear;
			JinxR.range = 12500;
			JinxR.radius = 140;
			JinxR.delay = 0.6;
			JinxR.danger = 4;
			JinxR.speed = 1700;
			JinxR.cc = false;
			JinxR.collisionMinions = false;
			JinxR.windwall = true;
			JinxR.hitbox = true;
			JinxR.fow = true;
			JinxR.exception = false;
			JinxR.extend = true;
			Jinx.Spells.push_back(JinxR);
			Core::SpellDB.emplace_back(Jinx);
		}
		{
			Champ Kaisa;
			Kaisa.Name = "Kaisa";
			Spell KaisaW;
			KaisaW.name = "KaisaW";
			KaisaW.icon = "KaisaW";
			KaisaW.displayName = "Void Seeker";
			KaisaW.missileName = "KaisaW";
			KaisaW.slot = SpellSlot::W;
			KaisaW.type = SpellType::linear;
			KaisaW.range = 3000;
			KaisaW.radius = 100;
			KaisaW.delay = 0.4;
			KaisaW.danger = 1;
			KaisaW.speed = 1750;
			KaisaW.cc = false;
			KaisaW.collisionMinions = true;
			KaisaW.windwall = true;
			KaisaW.hitbox = true;
			KaisaW.fow = true;
			KaisaW.exception = false;
			KaisaW.extend = true;
			Kaisa.Spells.push_back(KaisaW);
			Core::SpellDB.emplace_back(Kaisa);
		}
		{
			Champ Kalista;
			Kalista.Name = "Kalista";
			Spell KalistaMysticShot;
			KalistaMysticShot.name = "KalistaMysticShot";
			KalistaMysticShot.icon = "KalistaQ";
			KalistaMysticShot.displayName = "Pierce";
			KalistaMysticShot.missileName = "KalistaMysticShotMisTrue";
			KalistaMysticShot.slot = SpellSlot::Q;
			KalistaMysticShot.type = SpellType::linear;
			KalistaMysticShot.range = 1150;
			KalistaMysticShot.radius = 40;
			KalistaMysticShot.delay = 0.25;
			KalistaMysticShot.danger = 1;
			KalistaMysticShot.speed = 2400;
			KalistaMysticShot.cc = false;
			KalistaMysticShot.collisionMinions = true;
			KalistaMysticShot.windwall = true;
			KalistaMysticShot.hitbox = true;
			KalistaMysticShot.fow = true;
			KalistaMysticShot.exception = false;
			KalistaMysticShot.extend = true;
			Kalista.Spells.push_back(KalistaMysticShot);
			Core::SpellDB.emplace_back(Kalista);
		}
		{
			Champ Karma;
			Karma.Name = "Karma";
			Spell KarmaQ;
			KarmaQ.name = "KarmaQ";
			KarmaQ.icon = "KarmaQ1";
			KarmaQ.displayName = "Inner Flame";
			KarmaQ.missileName = "KarmaQMissile";
			KarmaQ.slot = SpellSlot::Q;
			KarmaQ.type = SpellType::linear;
			KarmaQ.range = 950;
			KarmaQ.radius = 60;
			KarmaQ.delay = 0.25;
			KarmaQ.danger = 2;
			KarmaQ.speed = 1700;
			KarmaQ.cc = true;
			KarmaQ.collisionMinions = true;
			KarmaQ.windwall = true;
			KarmaQ.hitbox = true;
			KarmaQ.fow = true;
			KarmaQ.exception = false;
			KarmaQ.extend = true;
			Karma.Spells.push_back(KarmaQ);
			Spell KarmaQMantra;
			KarmaQMantra.name = "KarmaQMantra";
			KarmaQMantra.icon = "KarmaQ2";
			KarmaQMantra.displayName = "Inner Flame [Mantra]";
			KarmaQMantra.missileName = "KarmaQMissileMantra";
			KarmaQMantra.slot = SpellSlot::Q;
			KarmaQMantra.type = SpellType::linear;
			KarmaQMantra.range = 950;
			KarmaQMantra.radius = 80;
			KarmaQMantra.delay = 0.25;
			KarmaQMantra.danger = 2;
			KarmaQMantra.speed = 1700;
			KarmaQMantra.cc = true;
			KarmaQMantra.collisionMinions = true;
			KarmaQMantra.windwall = true;
			KarmaQMantra.hitbox = true;
			KarmaQMantra.fow = true;
			KarmaQMantra.exception = false;
			KarmaQMantra.extend = true;
			Karma.Spells.push_back(KarmaQMantra);
			Core::SpellDB.emplace_back(Karma);
		}
		{
			Champ Karthus;
			Karthus.Name = "Karthus";
			Spell KarthusLayWasteA1;
			KarthusLayWasteA1.name = "KarthusLayWasteA1";
			KarthusLayWasteA1.icon = "KarthusQ";
			KarthusLayWasteA1.displayName = "Lay Waste [1]";
			KarthusLayWasteA1.missileName = "";
			KarthusLayWasteA1.slot = SpellSlot::Q;
			KarthusLayWasteA1.type = SpellType::circular;
			KarthusLayWasteA1.range = 875;
			KarthusLayWasteA1.radius = 175;
			KarthusLayWasteA1.delay = 0.9;
			KarthusLayWasteA1.danger = 1;
			KarthusLayWasteA1.speed = MathHuge;
			KarthusLayWasteA1.cc = false;
			KarthusLayWasteA1.collisionMinions = false;
			KarthusLayWasteA1.windwall = false;
			KarthusLayWasteA1.hitbox = false;
			KarthusLayWasteA1.fow = false;
			KarthusLayWasteA1.exception = false;
			KarthusLayWasteA1.extend = false;
			Karthus.Spells.push_back(KarthusLayWasteA1);
			Spell KarthusLayWasteA2;
			KarthusLayWasteA2.name = "KarthusLayWasteA2";
			KarthusLayWasteA2.icon = "KarthusQ";
			KarthusLayWasteA2.displayName = "Lay Waste [2]";
			KarthusLayWasteA2.missileName = "";
			KarthusLayWasteA2.slot = SpellSlot::Q;
			KarthusLayWasteA2.type = SpellType::circular;
			KarthusLayWasteA2.range = 875;
			KarthusLayWasteA2.radius = 175;
			KarthusLayWasteA2.delay = 0.9;
			KarthusLayWasteA2.danger = 1;
			KarthusLayWasteA2.speed = MathHuge;
			KarthusLayWasteA2.cc = false;
			KarthusLayWasteA2.collisionMinions = false;
			KarthusLayWasteA2.windwall = false;
			KarthusLayWasteA2.hitbox = false;
			KarthusLayWasteA2.fow = false;
			KarthusLayWasteA2.exception = false;
			KarthusLayWasteA2.extend = false;
			Karthus.Spells.push_back(KarthusLayWasteA2);
			Spell KarthusLayWasteA3;
			KarthusLayWasteA3.name = "KarthusLayWasteA3";
			KarthusLayWasteA3.icon = "KarthusQ";
			KarthusLayWasteA3.displayName = "Lay Waste [3]";
			KarthusLayWasteA3.missileName = "";
			KarthusLayWasteA3.slot = SpellSlot::Q;
			KarthusLayWasteA3.type = SpellType::circular;
			KarthusLayWasteA3.range = 875;
			KarthusLayWasteA3.radius = 175;
			KarthusLayWasteA3.delay = 0.9;
			KarthusLayWasteA3.danger = 1;
			KarthusLayWasteA3.speed = MathHuge;
			KarthusLayWasteA3.cc = false;
			KarthusLayWasteA3.collisionMinions = false;
			KarthusLayWasteA3.windwall = false;
			KarthusLayWasteA3.hitbox = false;
			KarthusLayWasteA3.fow = false;
			KarthusLayWasteA3.exception = false;
			KarthusLayWasteA3.extend = false;
			Karthus.Spells.push_back(KarthusLayWasteA3);
			Core::SpellDB.emplace_back(Karthus);
		}
		{
			Champ Kassadin;
			Kassadin.Name = "Kassadin";
			Spell ForcePulse;
			ForcePulse.name = "ForcePulse";
			ForcePulse.icon = "KassadinE";
			ForcePulse.displayName = "Force Pulse";
			ForcePulse.missileName = "";
			ForcePulse.slot = SpellSlot::E;
			ForcePulse.type = SpellType::conic;
			ForcePulse.range = 600;
			ForcePulse.radius = 0;
			ForcePulse.delay = 0.3;
			ForcePulse.danger = 3;
			ForcePulse.speed = MathHuge;
			ForcePulse.cc = true;
			ForcePulse.collisionMinions = false;
			ForcePulse.windwall = false;
			ForcePulse.hitbox = false;
			ForcePulse.fow = false;
			ForcePulse.exception = false;
			ForcePulse.extend = true;
			Kassadin.Spells.push_back(ForcePulse);
			Spell RiftWalk;
			RiftWalk.name = "RiftWalk";
			RiftWalk.icon = "KassadinR";
			RiftWalk.displayName = "Rift Walk";
			RiftWalk.missileName = "";
			RiftWalk.slot = SpellSlot::R;
			RiftWalk.type = SpellType::circular;
			RiftWalk.range = 500;
			RiftWalk.radius = 250;
			RiftWalk.delay = 0.25;
			RiftWalk.danger = 3;
			RiftWalk.speed = MathHuge;
			RiftWalk.cc = false;
			RiftWalk.collisionMinions = false;
			RiftWalk.windwall = false;
			RiftWalk.hitbox = false;
			RiftWalk.fow = false;
			RiftWalk.exception = false;
			RiftWalk.extend = false;
			Kassadin.Spells.push_back(RiftWalk);
			Core::SpellDB.emplace_back(Kassadin);
		}
		{
			Champ Kayle;
			Kayle.Name = "Kayle";
			Spell KayleQ;
			KayleQ.name = "KayleQ";
			KayleQ.icon = "KayleQ";
			KayleQ.displayName = "Radiant Blast";
			KayleQ.missileName = "KayleQMis";
			KayleQ.slot = SpellSlot::Q;
			KayleQ.type = SpellType::linear;
			KayleQ.range = 900;
			KayleQ.radius = 60;
			KayleQ.delay = 0.25;
			KayleQ.danger = 1;
			KayleQ.speed = 1600;
			KayleQ.cc = true;
			KayleQ.collisionMinions = false;
			KayleQ.windwall = true;
			KayleQ.hitbox = true;
			KayleQ.fow = true;
			KayleQ.exception = false;
			KayleQ.extend = true;
			Kayle.Spells.push_back(KayleQ);
			Core::SpellDB.emplace_back(Kayle);
		}
		{
			Champ Kayn;
			Kayn.Name = "Kayn";
			Spell KaynW;
			KaynW.name = "KaynW";
			KaynW.icon = "KaynW";
			KaynW.displayName = "Blade's Reach";
			KaynW.missileName = "";
			KaynW.slot = SpellSlot::W;
			KaynW.type = SpellType::linear;
			KaynW.range = 700;
			KaynW.radius = 90;
			KaynW.delay = 0.55;
			KaynW.danger = 2;
			KaynW.speed = MathHuge;
			KaynW.cc = true;
			KaynW.collisionMinions = false;
			KaynW.windwall = false;
			KaynW.hitbox = false;
			KaynW.fow = false;
			KaynW.exception = false;
			KaynW.extend = true;
			Kayn.Spells.push_back(KaynW);

			Spell W2;
			W2.name = "KaynAssW";
			W2.icon = "KaynAssW";
			W2.displayName = "Blade's Reach (Blue)";
			W2.slot = SpellSlot::W;
			W2.type = SpellType::linear;
			W2.speed = MathHuge;
			W2.range = 900;
			W2.delay = 0.60f;
			W2.radius = 100;
			W2.danger = 3;
			W2.cc = false;
			W2.collisionMinions = false;
			W2.windwall = false;
			W2.hitbox = false;
			W2.fow = false;
			W2.exception = false;
			W2.extend = true;
			Kayn.Spells.emplace_back(W2);

			Core::SpellDB.emplace_back(Kayn);
		}
		{
			Champ Kennen;
			Kennen.Name = "Kennen";
			Spell KennenShurikenHurlMissile1;
			KennenShurikenHurlMissile1.name = "KennenShurikenHurlMissile1";
			KennenShurikenHurlMissile1.icon = "KennenQ";
			KennenShurikenHurlMissile1.displayName = "Shuriken Hurl";
			KennenShurikenHurlMissile1.missileName = "KennenShurikenHurlMissile1";
			KennenShurikenHurlMissile1.slot = SpellSlot::Q;
			KennenShurikenHurlMissile1.type = SpellType::linear;
			KennenShurikenHurlMissile1.range = 1050;
			KennenShurikenHurlMissile1.radius = 50;
			KennenShurikenHurlMissile1.delay = 0.175;
			KennenShurikenHurlMissile1.danger = 2;
			KennenShurikenHurlMissile1.speed = 1700;
			KennenShurikenHurlMissile1.cc = false;
			KennenShurikenHurlMissile1.collisionMinions = true;
			KennenShurikenHurlMissile1.windwall = true;
			KennenShurikenHurlMissile1.hitbox = true;
			KennenShurikenHurlMissile1.fow = true;
			KennenShurikenHurlMissile1.exception = false;
			KennenShurikenHurlMissile1.extend = true;
			Kennen.Spells.push_back(KennenShurikenHurlMissile1);
			Core::SpellDB.emplace_back(Kennen);
		}
		{
			Champ Khazix;
			Khazix.Name = "Khazix";
			Spell KhazixW;
			KhazixW.name = "KhazixW";
			KhazixW.icon = "KhazixW1";
			KhazixW.displayName = "Void Spike [Standard]";
			KhazixW.missileName = "KhazixWMissile";
			KhazixW.slot = SpellSlot::W;
			KhazixW.type = SpellType::linear;
			KhazixW.range = 1000;
			KhazixW.radius = 70;
			KhazixW.delay = 0.25;
			KhazixW.danger = 1;
			KhazixW.speed = 1700;
			KhazixW.cc = false;
			KhazixW.collisionMinions = true;
			KhazixW.windwall = true;
			KhazixW.hitbox = true;
			KhazixW.fow = true;
			KhazixW.exception = false;
			KhazixW.extend = true;
			Khazix.Spells.push_back(KhazixW);
			Spell KhazixWLong;
			KhazixWLong.name = "KhazixWLong";
			KhazixWLong.icon = "KhazixW2";
			KhazixWLong.displayName = "Void Spike [Threeway]";
			KhazixWLong.missileName = "";
			KhazixWLong.slot = SpellSlot::W;
			KhazixWLong.type = SpellType::threeway;
			KhazixWLong.range = 1000;
			KhazixWLong.radius = 70;
			KhazixWLong.delay = 0.25;
			KhazixWLong.danger = 2;
			KhazixWLong.speed = 1700;
			KhazixWLong.cc = true;
			KhazixWLong.collisionMinions = true;
			KhazixWLong.windwall = true;
			KhazixWLong.hitbox = true;
			KhazixWLong.fow = false;
			KhazixWLong.exception = false;
			KhazixWLong.extend = true;
			Khazix.Spells.push_back(KhazixWLong);
			Core::SpellDB.emplace_back(Khazix);
		}
		{
			Champ Kled;
			Kled.Name = "Kled";
			Spell KledQ;
			KledQ.name = "KledQ";
			KledQ.icon = "KledQMount";
			KledQ.displayName = "Beartrap on a Rope";
			KledQ.missileName = "KledQMissile";
			KledQ.slot = SpellSlot::Q;
			KledQ.type = SpellType::linear;
			KledQ.range = 800;
			KledQ.radius = 45;
			KledQ.delay = 0.25;
			KledQ.danger = 1;
			KledQ.speed = 1600;
			KledQ.cc = true;
			KledQ.collisionMinions = false;
			KledQ.windwall = true;
			KledQ.hitbox = false;
			KledQ.fow = true;
			KledQ.exception = false;
			KledQ.extend = true;
			Kled.Spells.push_back(KledQ);
			Spell KledRiderQ;
			KledRiderQ.name = "KledRiderQ";
			KledRiderQ.icon = "KledQDismount";
			KledRiderQ.displayName = "Pocket Pistol";
			KledRiderQ.missileName = "KledRiderQMissile";
			KledRiderQ.slot = SpellSlot::Q;
			KledRiderQ.type = SpellType::conic;
			KledRiderQ.range = 700;
			KledRiderQ.radius = 0;
			KledRiderQ.delay = 0.25;
			KledRiderQ.danger = 3;
			KledRiderQ.speed = 3000;
			KledRiderQ.cc = false;
			KledRiderQ.collisionMinions = false;
			KledRiderQ.windwall = true;
			KledRiderQ.hitbox = false;
			KledRiderQ.fow = true;
			KledRiderQ.exception = false;
			KledRiderQ.extend = true;
			Kled.Spells.push_back(KledRiderQ);
			Core::SpellDB.emplace_back(Kled);
		}
		{
			Champ KogMaw;
			KogMaw.Name = "KogMaw";
			Spell KogMawQ;
			KogMawQ.name = "KogMawQ";
			KogMawQ.icon = "KogMawQ";
			KogMawQ.displayName = "Caustic Spittle";
			KogMawQ.missileName = "KogMawQ";
			KogMawQ.slot = SpellSlot::Q;
			KogMawQ.type = SpellType::linear;
			KogMawQ.range = 1175;
			KogMawQ.radius = 70;
			KogMawQ.delay = 0.25;
			KogMawQ.danger = 1;
			KogMawQ.speed = 1650;
			KogMawQ.cc = false;
			KogMawQ.collisionMinions = true;
			KogMawQ.windwall = true;
			KogMawQ.hitbox = true;
			KogMawQ.fow = true;
			KogMawQ.exception = false;
			KogMawQ.extend = true;
			KogMaw.Spells.push_back(KogMawQ);
			Spell KogMawVoidOozeMissile;
			KogMawVoidOozeMissile.name = "KogMawVoidOozeMissile";
			KogMawVoidOozeMissile.icon = "KogMawE";
			KogMawVoidOozeMissile.displayName = "Void Ooze";
			KogMawVoidOozeMissile.missileName = "KogMawVoidOozeMissile";
			KogMawVoidOozeMissile.slot = SpellSlot::E;
			KogMawVoidOozeMissile.type = SpellType::linear;
			KogMawVoidOozeMissile.range = 1360;
			KogMawVoidOozeMissile.radius = 120;
			KogMawVoidOozeMissile.delay = 0.25;
			KogMawVoidOozeMissile.danger = 2;
			KogMawVoidOozeMissile.speed = 1400;
			KogMawVoidOozeMissile.cc = true;
			KogMawVoidOozeMissile.collisionMinions = false;
			KogMawVoidOozeMissile.windwall = true;
			KogMawVoidOozeMissile.hitbox = true;
			KogMawVoidOozeMissile.fow = true;
			KogMawVoidOozeMissile.exception = false;
			KogMawVoidOozeMissile.extend = true;
			KogMaw.Spells.push_back(KogMawVoidOozeMissile);
			Spell KogMawLivingArtillery;
			KogMawLivingArtillery.name = "KogMawLivingArtillery";
			KogMawLivingArtillery.icon = "KogMawR";
			KogMawLivingArtillery.displayName = "Living Artillery";
			KogMawLivingArtillery.missileName = "";
			KogMawLivingArtillery.slot = SpellSlot::R;
			KogMawLivingArtillery.type = SpellType::circular;
			KogMawLivingArtillery.range = 1300;
			KogMawLivingArtillery.radius = 200;
			KogMawLivingArtillery.delay = 1.1;
			KogMawLivingArtillery.danger = 1;
			KogMawLivingArtillery.speed = MathHuge;
			KogMawLivingArtillery.cc = false;
			KogMawLivingArtillery.collisionMinions = false;
			KogMawLivingArtillery.windwall = false;
			KogMawLivingArtillery.hitbox = false;
			KogMawLivingArtillery.fow = false;
			KogMawLivingArtillery.exception = false;
			KogMawLivingArtillery.extend = false;
			KogMaw.Spells.push_back(KogMawLivingArtillery);
			Core::SpellDB.emplace_back(KogMaw);
		}
		{
			Champ Leblanc;
			Leblanc.Name = "Leblanc";
			Spell LeblancE;
			LeblancE.name = "LeblancE";
			LeblancE.icon = "LeblancE";
			LeblancE.displayName = "Ethereal Chains [Standard]";
			LeblancE.missileName = "LeblancEMissile";
			LeblancE.slot = SpellSlot::E;
			LeblancE.type = SpellType::linear;
			LeblancE.range = 925;
			LeblancE.radius = 55;
			LeblancE.delay = 0.25;
			LeblancE.danger = 1;
			LeblancE.speed = 1750;
			LeblancE.cc = true;
			LeblancE.collisionMinions = true;
			LeblancE.windwall = true;
			LeblancE.hitbox = true;
			LeblancE.fow = true;
			LeblancE.exception = false;
			LeblancE.extend = true;
			Leblanc.Spells.push_back(LeblancE);
			Spell LeblancRE;
			LeblancRE.name = "LeblancRE";
			LeblancRE.icon = "LeblancRE";
			LeblancRE.displayName = "Ethereal Chains [Ultimate]";
			LeblancRE.missileName = "LeblancREMissile";
			LeblancRE.slot = SpellSlot::E;
			LeblancRE.type = SpellType::linear;
			LeblancRE.range = 925;
			LeblancRE.radius = 55;
			LeblancRE.delay = 0.25;
			LeblancRE.danger = 1;
			LeblancRE.speed = 1750;
			LeblancRE.cc = true;
			LeblancRE.collisionMinions = true;
			LeblancRE.windwall = true;
			LeblancRE.hitbox = true;
			LeblancRE.fow = true;
			LeblancRE.exception = false;
			LeblancRE.extend = true;
			Leblanc.Spells.push_back(LeblancRE);
			Core::SpellDB.emplace_back(Leblanc);
		}
		{
			Champ LeeSin;
			LeeSin.Name = "LeeSin";
			Spell BlindMonkQOne;
			BlindMonkQOne.name = "BlindMonkQOne";
			BlindMonkQOne.icon = "LeeSinQ";
			BlindMonkQOne.displayName = "Sonic Wave";
			BlindMonkQOne.missileName = "BlindMonkQOne";
			BlindMonkQOne.slot = SpellSlot::Q;
			BlindMonkQOne.type = SpellType::linear;
			BlindMonkQOne.range = 1100;
			BlindMonkQOne.radius = 60;
			BlindMonkQOne.delay = 0.25;
			BlindMonkQOne.danger = 1;
			BlindMonkQOne.speed = 1800;
			BlindMonkQOne.cc = false;
			BlindMonkQOne.collisionMinions = true;
			BlindMonkQOne.windwall = true;
			BlindMonkQOne.hitbox = true;
			BlindMonkQOne.fow = true;
			BlindMonkQOne.exception = false;
			BlindMonkQOne.extend = true;
			LeeSin.Spells.push_back(BlindMonkQOne);
			Core::SpellDB.emplace_back(LeeSin);
		}
		{
			Champ Leona;
			Leona.Name = "Leona";
			Spell LeonaZenithBlade;
			LeonaZenithBlade.name = "LeonaZenithBlade";
			LeonaZenithBlade.icon = "LeonaE";
			LeonaZenithBlade.displayName = "Zenith Blade";
			LeonaZenithBlade.missileName = "LeonaZenithBladeMissile";
			LeonaZenithBlade.slot = SpellSlot::E;
			LeonaZenithBlade.type = SpellType::linear;
			LeonaZenithBlade.range = 875;
			LeonaZenithBlade.radius = 70;
			LeonaZenithBlade.delay = 0.25;
			LeonaZenithBlade.danger = 2;
			LeonaZenithBlade.speed = 2000;
			LeonaZenithBlade.cc = true;
			LeonaZenithBlade.collisionMinions = false;
			LeonaZenithBlade.windwall = true;
			LeonaZenithBlade.hitbox = true;
			LeonaZenithBlade.fow = true;
			LeonaZenithBlade.exception = false;
			LeonaZenithBlade.extend = true;
			Leona.Spells.push_back(LeonaZenithBlade);
			Spell LeonaSolarFlare;
			LeonaSolarFlare.name = "LeonaSolarFlare";
			LeonaSolarFlare.icon = "LeonaR";
			LeonaSolarFlare.displayName = "Solar Flare";
			LeonaSolarFlare.missileName = "";
			LeonaSolarFlare.slot = SpellSlot::R;
			LeonaSolarFlare.type = SpellType::circular;
			LeonaSolarFlare.range = 1200;
			LeonaSolarFlare.radius = 300;
			LeonaSolarFlare.delay = 0.85;
			LeonaSolarFlare.danger = 5;
			LeonaSolarFlare.speed = MathHuge;
			LeonaSolarFlare.cc = true;
			LeonaSolarFlare.collisionMinions = false;
			LeonaSolarFlare.windwall = false;
			LeonaSolarFlare.hitbox = false;
			LeonaSolarFlare.fow = false;
			LeonaSolarFlare.exception = false;
			LeonaSolarFlare.extend = false;
			Leona.Spells.push_back(LeonaSolarFlare);
			Core::SpellDB.emplace_back(Leona);
		}
		{
			Champ Lissandra;
			Lissandra.Name = "Lissandra";
			Spell LissandraQMissile;
			LissandraQMissile.name = "LissandraQMissile";
			LissandraQMissile.icon = "LissandraQ";
			LissandraQMissile.displayName = "Ice Shard";
			LissandraQMissile.missileName = "LissandraQMissile";
			LissandraQMissile.slot = SpellSlot::Q;
			LissandraQMissile.type = SpellType::linear;
			LissandraQMissile.range = 750;
			LissandraQMissile.radius = 75;
			LissandraQMissile.delay = 0.25;
			LissandraQMissile.danger = 2;
			LissandraQMissile.speed = 2200;
			LissandraQMissile.cc = true;
			LissandraQMissile.collisionMinions = false;
			LissandraQMissile.windwall = true;
			LissandraQMissile.hitbox = true;
			LissandraQMissile.fow = true;
			LissandraQMissile.exception = false;
			LissandraQMissile.extend = true;
			Lissandra.Spells.push_back(LissandraQMissile);
			Spell LissandraEMissile;
			LissandraEMissile.name = "LissandraEMissile";
			LissandraEMissile.icon = "LissandraE";
			LissandraEMissile.displayName = "Glacial Path";
			LissandraEMissile.missileName = "LissandraEMissile";
			LissandraEMissile.slot = SpellSlot::E;
			LissandraEMissile.type = SpellType::linear;
			LissandraEMissile.range = 1025;
			LissandraEMissile.radius = 125;
			LissandraEMissile.delay = 0.25;
			LissandraEMissile.danger = 2;
			LissandraEMissile.speed = 850;
			LissandraEMissile.cc = false;
			LissandraEMissile.collisionMinions = false;
			LissandraEMissile.windwall = true;
			LissandraEMissile.hitbox = true;
			LissandraEMissile.fow = true;
			LissandraEMissile.exception = false;
			LissandraEMissile.extend = true;
			Lissandra.Spells.push_back(LissandraEMissile);
			Core::SpellDB.emplace_back(Lissandra);
		}
		{
			Champ Lucian;
			Lucian.Name = "Lucian";
			Spell LucianQ;
			LucianQ.name = "LucianQ";
			LucianQ.icon = "LucianQ";
			LucianQ.displayName = "Piercing Light";
			LucianQ.missileName = "";
			LucianQ.slot = SpellSlot::Q;
			LucianQ.type = SpellType::linear;
			LucianQ.range = 900;
			LucianQ.radius = 65;
			LucianQ.delay = 0.35;
			LucianQ.danger = 2;
			LucianQ.speed = MathHuge;
			LucianQ.cc = false;
			LucianQ.collisionMinions = false;
			LucianQ.windwall = false;
			LucianQ.hitbox = false;
			LucianQ.fow = false;
			LucianQ.exception = false;
			LucianQ.extend = true;
			Lucian.Spells.push_back(LucianQ);
			Spell LucianW;
			LucianW.name = "LucianW";
			LucianW.icon = "LucianW";
			LucianW.displayName = "Ardent Blaze";
			LucianW.missileName = "LucianW";
			LucianW.slot = SpellSlot::W;
			LucianW.type = SpellType::linear;
			LucianW.range = 900;
			LucianW.radius = 80;
			LucianW.delay = 0.25;
			LucianW.danger = 2;
			LucianW.speed = 1600;
			LucianW.cc = false;
			LucianW.collisionMinions = true;
			LucianW.windwall = true;
			LucianW.hitbox = true;
			LucianW.fow = true;
			LucianW.exception = false;
			LucianW.extend = true;
			Lucian.Spells.push_back(LucianW);
			Core::SpellDB.emplace_back(Lucian);
		}
		{
			Champ Lulu;
			Lulu.Name = "Lulu";
			Spell LuluQ;
			LuluQ.name = "LuluQ";
			LuluQ.icon = "LuluQ";
			LuluQ.displayName = "Glitterlance";
			LuluQ.missileName = "LuluQMissile";
			LuluQ.slot = SpellSlot::Q;
			LuluQ.type = SpellType::linear;
			LuluQ.range = 925;
			LuluQ.radius = 60;
			LuluQ.delay = 0.25;
			LuluQ.danger = 1;
			LuluQ.speed = 1450;
			LuluQ.cc = true;
			LuluQ.collisionMinions = false;
			LuluQ.windwall = true;
			LuluQ.hitbox = true;
			LuluQ.fow = true;
			LuluQ.exception = false;
			LuluQ.extend = true;
			Lulu.Spells.push_back(LuluQ);
			Core::SpellDB.emplace_back(Lulu);
		}
		{
			Champ Lux;
			Lux.Name = "Lux";
			Spell LuxLightBinding;
			LuxLightBinding.name = "LuxLightBinding";
			LuxLightBinding.icon = "LuxQ";
			LuxLightBinding.displayName = "Light Binding";
			LuxLightBinding.missileName = "LuxLightBindingDummy";
			LuxLightBinding.slot = SpellSlot::Q;
			LuxLightBinding.type = SpellType::linear;
			LuxLightBinding.range = 1175;
			LuxLightBinding.radius = 70;
			LuxLightBinding.delay = 0.25;
			LuxLightBinding.danger = 1;
			LuxLightBinding.speed = 1200;
			LuxLightBinding.cc = true;
			LuxLightBinding.collisionMinions = false;
			LuxLightBinding.windwall = true;
			LuxLightBinding.hitbox = true;
			LuxLightBinding.fow = true;
			LuxLightBinding.exception = false;
			LuxLightBinding.extend = true;
			Lux.Spells.push_back(LuxLightBinding);
			Spell LuxLightStrikeKugel;
			LuxLightStrikeKugel.name = "LuxLightStrikeKugel";
			LuxLightStrikeKugel.icon = "LuxE";
			LuxLightStrikeKugel.displayName = "Light Strike Kugel";
			LuxLightStrikeKugel.missileName = "LuxLightStrikeKugel";
			LuxLightStrikeKugel.slot = SpellSlot::E;
			LuxLightStrikeKugel.type = SpellType::circular;
			LuxLightStrikeKugel.range = 1100;
			LuxLightStrikeKugel.radius = 300;
			LuxLightStrikeKugel.delay = 0.25;
			LuxLightStrikeKugel.danger = 3;
			LuxLightStrikeKugel.speed = 1200;
			LuxLightStrikeKugel.cc = true;
			LuxLightStrikeKugel.collisionMinions = true;
			LuxLightStrikeKugel.windwall = true;
			LuxLightStrikeKugel.hitbox = false;
			LuxLightStrikeKugel.fow = true;
			LuxLightStrikeKugel.exception = false;
			LuxLightStrikeKugel.extend = false;
			Lux.Spells.push_back(LuxLightStrikeKugel);
			Spell LuxMaliceCannon;
			LuxMaliceCannon.name = "LuxMaliceCannon";
			LuxMaliceCannon.icon = "LuxR";
			LuxMaliceCannon.displayName = "Malice Cannon";
			LuxMaliceCannon.missileName = "LuxRVfxMis";
			LuxMaliceCannon.slot = SpellSlot::R;
			LuxMaliceCannon.type = SpellType::linear;
			LuxMaliceCannon.range = 3340;
			LuxMaliceCannon.radius = 120;
			LuxMaliceCannon.delay = 1;
			LuxMaliceCannon.danger = 4;
			LuxMaliceCannon.speed = MathHuge;
			LuxMaliceCannon.cc = false;
			LuxMaliceCannon.collisionMinions = false;
			LuxMaliceCannon.windwall = false;
			LuxMaliceCannon.hitbox = false;
			LuxMaliceCannon.fow = true;
			LuxMaliceCannon.exception = false;
			LuxMaliceCannon.extend = true;
			Lux.Spells.push_back(LuxMaliceCannon);
			Core::SpellDB.emplace_back(Lux);
		}
		{
			Champ Malphite;
			Malphite.Name = "Malphite";
			Spell Landslide;
			Landslide.name = "Landslide";
			Landslide.icon = "MalphiteE";
			Landslide.displayName = "Ground Slam";
			Landslide.missileName = "";
			Landslide.slot = SpellSlot::E;
			Landslide.type = SpellType::circular;
			Landslide.range = 0;
			Landslide.radius = 400;
			Landslide.delay = 0.242;
			Landslide.danger = 2;
			Landslide.speed = MathHuge;
			Landslide.cc = true;
			Landslide.collisionMinions = false;
			Landslide.windwall = false;
			Landslide.hitbox = false;
			Landslide.fow = false;
			Landslide.exception = false;
			Landslide.extend = false;
			Malphite.Spells.push_back(Landslide);
			Core::SpellDB.emplace_back(Malphite);
		}
		{
			Champ Malzahar;
			Malzahar.Name = "Malzahar";
			Spell MalzaharQ;
			MalzaharQ.name = "MalzaharQ";
			MalzaharQ.icon = "MalzaharQ";
			MalzaharQ.displayName = "Call of the Void";
			MalzaharQ.missileName = "";
			MalzaharQ.slot = SpellSlot::Q;
			MalzaharQ.type = SpellType::rectangular;
			MalzaharQ.range = 900;
			MalzaharQ.radius = 100;
			MalzaharQ.delay = 0.5;
			MalzaharQ.danger = 1;
			MalzaharQ.speed = 1600;
			MalzaharQ.cc = true;
			MalzaharQ.collisionMinions = false;
			MalzaharQ.windwall = false;
			MalzaharQ.hitbox = false;
			MalzaharQ.fow = false;
			MalzaharQ.exception = false;
			MalzaharQ.extend = false;
			Malzahar.Spells.push_back(MalzaharQ);
			Core::SpellDB.emplace_back(Malzahar);
		}
		{
			Champ Maokai;
			Maokai.Name = "Maokai";
			Spell MaokaiQ;
			MaokaiQ.name = "MaokaiQ";
			MaokaiQ.icon = "MaokaiQ";
			MaokaiQ.displayName = "Bramble Smash";
			MaokaiQ.missileName = "MaokaiQMissile";
			MaokaiQ.slot = SpellSlot::Q;
			MaokaiQ.type = SpellType::linear;
			MaokaiQ.range = 600;
			MaokaiQ.radius = 110;
			MaokaiQ.delay = 0.375;
			MaokaiQ.danger = 2;
			MaokaiQ.speed = 1600;
			MaokaiQ.cc = true;
			MaokaiQ.collisionMinions = false;
			MaokaiQ.windwall = true;
			MaokaiQ.hitbox = true;
			MaokaiQ.fow = true;
			MaokaiQ.exception = false;
			MaokaiQ.extend = true;
			Maokai.Spells.push_back(MaokaiQ);
			Core::SpellDB.emplace_back(Maokai);
		}
		{
			Champ MissFortune;
			MissFortune.Name = "MissFortune";
			Spell MissFortuneBulletTime;
			MissFortuneBulletTime.name = "MissFortuneBulletTime";
			MissFortuneBulletTime.icon = "MissFortuneR";
			MissFortuneBulletTime.displayName = "Bullet Time";
			MissFortuneBulletTime.missileName = "";
			MissFortuneBulletTime.slot = SpellSlot::R;
			MissFortuneBulletTime.type = SpellType::conic;
			MissFortuneBulletTime.range = 1400;
			MissFortuneBulletTime.radius = 100;
			MissFortuneBulletTime.delay = 0.25;
			MissFortuneBulletTime.danger = 4;
			MissFortuneBulletTime.speed = 2000;
			MissFortuneBulletTime.cc = false;
			MissFortuneBulletTime.collisionMinions = false;
			MissFortuneBulletTime.windwall = true;
			MissFortuneBulletTime.hitbox = false;
			MissFortuneBulletTime.fow = false;
			MissFortuneBulletTime.exception = false;
			MissFortuneBulletTime.extend = true;
			MissFortune.Spells.push_back(MissFortuneBulletTime);
			Core::SpellDB.emplace_back(MissFortune);
		}
		{
			Champ Mordekaiser;
			Mordekaiser.Name = "Mordekaiser";
			Spell MordekaiserQ;
			MordekaiserQ.name = "MordekaiserQ";
			MordekaiserQ.icon = "MordekaiserQ";
			MordekaiserQ.displayName = "Obliterate";
			MordekaiserQ.missileName = "";
			MordekaiserQ.slot = SpellSlot::Q;
			MordekaiserQ.type = SpellType::polygon;
			MordekaiserQ.range = 675;
			MordekaiserQ.radius = 200;
			MordekaiserQ.delay = 0.4;
			MordekaiserQ.danger = 2;
			MordekaiserQ.speed = MathHuge;
			MordekaiserQ.cc = false;
			MordekaiserQ.collisionMinions = false;
			MordekaiserQ.windwall = false;
			MordekaiserQ.hitbox = false;
			MordekaiserQ.fow = false;
			MordekaiserQ.exception = false;
			MordekaiserQ.extend = true;
			Mordekaiser.Spells.push_back(MordekaiserQ);
			Spell MordekaiserE;
			MordekaiserE.name = "MordekaiserE";
			MordekaiserE.icon = "MordekaiserE";
			MordekaiserE.displayName = "Death's Grasp";
			MordekaiserE.missileName = "";
			MordekaiserE.slot = SpellSlot::E;
			MordekaiserE.type = SpellType::polygon;
			MordekaiserE.range = 900;
			MordekaiserE.radius = 140;
			MordekaiserE.delay = 0.9;
			MordekaiserE.danger = 3;
			MordekaiserE.speed = MathHuge;
			MordekaiserE.cc = true;
			MordekaiserE.collisionMinions = false;
			MordekaiserE.windwall = true;
			MordekaiserE.hitbox = true;
			MordekaiserE.fow = false;
			MordekaiserE.exception = false;
			MordekaiserE.extend = false;
			Mordekaiser.Spells.push_back(MordekaiserE);
			Core::SpellDB.emplace_back(Mordekaiser);
		}
		{
			Champ Morgana;
			Morgana.Name = "Morgana";
			Spell MorganaQ;
			MorganaQ.name = "MorganaQ";
			MorganaQ.icon = "MorganaQ";
			MorganaQ.displayName = "Dark Binding";
			MorganaQ.missileName = "MorganaQ";
			MorganaQ.slot = SpellSlot::Q;
			MorganaQ.type = SpellType::linear;
			MorganaQ.range = 1250;
			MorganaQ.radius = 70;
			MorganaQ.delay = 0.25;
			MorganaQ.danger = 1;
			MorganaQ.speed = 1200;
			MorganaQ.cc = true;
			MorganaQ.collisionMinions = true;
			MorganaQ.windwall = true;
			MorganaQ.hitbox = true;
			MorganaQ.fow = true;
			MorganaQ.exception = false;
			MorganaQ.extend = true;
			Morgana.Spells.push_back(MorganaQ);
			Core::SpellDB.emplace_back(Morgana);
		}
		{
			Champ Nami;
			Nami.Name = "Nami";
			Spell NamiQ;
			NamiQ.name = "NamiQ";
			NamiQ.icon = "NamiQ";
			NamiQ.displayName = "Aqua Prison";
			NamiQ.missileName = "NamiQMissile";
			NamiQ.slot = SpellSlot::Q;
			NamiQ.type = SpellType::circular;
			NamiQ.range = 875;
			NamiQ.radius = 180;
			NamiQ.delay = 1;
			NamiQ.danger = 1;
			NamiQ.speed = MathHuge;
			NamiQ.cc = true;
			NamiQ.collisionMinions = false;
			NamiQ.windwall = true;
			NamiQ.hitbox = false;
			NamiQ.fow = true;
			NamiQ.exception = false;
			NamiQ.extend = false;
			Nami.Spells.push_back(NamiQ);
			Spell NamiRMissile;
			NamiRMissile.name = "NamiRMissile";
			NamiRMissile.icon = "NamiR";
			NamiRMissile.displayName = "Tidal Wave";
			NamiRMissile.missileName = "NamiRMissile";
			NamiRMissile.slot = SpellSlot::R;
			NamiRMissile.type = SpellType::linear;
			NamiRMissile.range = 2750;
			NamiRMissile.radius = 250;
			NamiRMissile.delay = 0.5;
			NamiRMissile.danger = 3;
			NamiRMissile.speed = 850;
			NamiRMissile.cc = true;
			NamiRMissile.collisionMinions = false;
			NamiRMissile.windwall = true;
			NamiRMissile.hitbox = true;
			NamiRMissile.fow = true;
			NamiRMissile.exception = false;
			NamiRMissile.extend = true;
			Nami.Spells.push_back(NamiRMissile);
			Core::SpellDB.emplace_back(Nami);
		}
		{
			Champ Nautilus;
			Nautilus.Name = "Nautilus";
			Spell NautilusAnchorDragMissile;
			NautilusAnchorDragMissile.name = "NautilusAnchorDragMissile";
			NautilusAnchorDragMissile.icon = "NautilusQ";
			NautilusAnchorDragMissile.displayName = "Dredge Line";
			NautilusAnchorDragMissile.missileName = "NautilusAnchorDragMissile";
			NautilusAnchorDragMissile.slot = SpellSlot::Q;
			NautilusAnchorDragMissile.type = SpellType::linear;
			NautilusAnchorDragMissile.range = 925;
			NautilusAnchorDragMissile.radius = 90;
			NautilusAnchorDragMissile.delay = 0.25;
			NautilusAnchorDragMissile.danger = 3;
			NautilusAnchorDragMissile.speed = 2000;
			NautilusAnchorDragMissile.cc = true;
			NautilusAnchorDragMissile.collisionMinions = true;
			NautilusAnchorDragMissile.windwall = true;
			NautilusAnchorDragMissile.hitbox = true;
			NautilusAnchorDragMissile.fow = true;
			NautilusAnchorDragMissile.exception = false;
			NautilusAnchorDragMissile.extend = true;
			Nautilus.Spells.push_back(NautilusAnchorDragMissile);
			Core::SpellDB.emplace_back(Nautilus);
		}
		{
			Champ Neeko;
			Neeko.Name = "Neeko";
			Spell NeekoQ;
			NeekoQ.name = "NeekoQ";
			NeekoQ.icon = "NeekoQ";
			NeekoQ.displayName = "Blooming Burst";
			NeekoQ.missileName = "NeekoQ";
			NeekoQ.slot = SpellSlot::Q;
			NeekoQ.type = SpellType::circular;
			NeekoQ.range = 800;
			NeekoQ.radius = 200;
			NeekoQ.delay = 0.25;
			NeekoQ.danger = 2;
			NeekoQ.speed = 1500;
			NeekoQ.cc = true;
			NeekoQ.collisionMinions = false;
			NeekoQ.windwall = true;
			NeekoQ.hitbox = false;
			NeekoQ.fow = true;
			NeekoQ.exception = false;
			NeekoQ.extend = false;
			Neeko.Spells.push_back(NeekoQ);
			Spell NeekoE;
			NeekoE.name = "NeekoE";
			NeekoE.icon = "NeekoE";
			NeekoE.displayName = "Tangle-Barbs";
			NeekoE.missileName = "NeekoE";
			NeekoE.slot = SpellSlot::E;
			NeekoE.type = SpellType::linear;
			NeekoE.range = 1000;
			NeekoE.radius = 70;
			NeekoE.delay = 0.25;
			NeekoE.danger = 1;
			NeekoE.speed = 1300;
			NeekoE.cc = true;
			NeekoE.collisionMinions = false;
			NeekoE.windwall = true;
			NeekoE.hitbox = true;
			NeekoE.fow = true;
			NeekoE.exception = false;
			NeekoE.extend = true;
			Neeko.Spells.push_back(NeekoE);
			Core::SpellDB.emplace_back(Neeko);
		}
		{
			Champ Nidalee;
			Nidalee.Name = "Nidalee";
			Spell JavelinToss;
			JavelinToss.name = "JavelinToss";
			JavelinToss.icon = "NidaleeQ";
			JavelinToss.displayName = "Javelin Toss";
			JavelinToss.missileName = "JavelinToss";
			JavelinToss.slot = SpellSlot::Q;
			JavelinToss.type = SpellType::linear;
			JavelinToss.range = 1500;
			JavelinToss.radius = 40;
			JavelinToss.delay = 0.25;
			JavelinToss.danger = 1;
			JavelinToss.speed = 1300;
			JavelinToss.cc = false;
			JavelinToss.collisionMinions = true;
			JavelinToss.windwall = true;
			JavelinToss.hitbox = true;
			JavelinToss.fow = true;
			JavelinToss.exception = false;
			JavelinToss.extend = true;
			Nidalee.Spells.push_back(JavelinToss);
			Spell Bushwhack;
			Bushwhack.name = "Bushwhack";
			Bushwhack.icon = "NidaleeW";
			Bushwhack.displayName = "Bushwhack";
			Bushwhack.missileName = "";
			Bushwhack.slot = SpellSlot::W;
			Bushwhack.type = SpellType::circular;
			Bushwhack.range = 900;
			Bushwhack.radius = 85;
			Bushwhack.delay = 1.25;
			Bushwhack.danger = 1;
			Bushwhack.speed = MathHuge;
			Bushwhack.cc = false;
			Bushwhack.collisionMinions = false;
			Bushwhack.windwall = false;
			Bushwhack.hitbox = false;
			Bushwhack.fow = false;
			Bushwhack.exception = false;
			Bushwhack.extend = false;
			Nidalee.Spells.push_back(Bushwhack);
			Spell Swipe;
			Swipe.name = "Swipe";
			Swipe.icon = "NidaleeE";
			Swipe.displayName = "Swipe";
			Swipe.missileName = "";
			Swipe.slot = SpellSlot::E;
			Swipe.type = SpellType::conic;
			Swipe.range = 350;
			Swipe.radius = 0;
			Swipe.delay = 0.25;
			Swipe.danger = 2;
			Swipe.speed = MathHuge;
			Swipe.cc = false;
			Swipe.collisionMinions = false;
			Swipe.windwall = false;
			Swipe.hitbox = false;
			Swipe.fow = false;
			Swipe.exception = false;
			Swipe.extend = true;
			Nidalee.Spells.push_back(Swipe);
			Core::SpellDB.emplace_back(Nidalee);
		}
		{
			Champ Nilah;
			Nilah.Name = "Nilah";
			Spell NilahQ;
			NilahQ.name = "NilahQ";
			NilahQ.icon = "NilahQ";
			NilahQ.displayName = "Formless Blade";
			NilahQ.missileName = "";
			NilahQ.slot = SpellSlot::Q;
			NilahQ.type = SpellType::linear;
			NilahQ.range = 600;
			NilahQ.radius = 150;
			NilahQ.delay = 0.25;
			NilahQ.danger = 2;
			NilahQ.speed = 500;
			NilahQ.cc = false;
			NilahQ.collisionMinions = false;
			NilahQ.windwall = false;
			NilahQ.hitbox = false;
			NilahQ.fow = true;
			NilahQ.exception = false;
			NilahQ.extend = false;
			Nilah.Spells.push_back(NilahQ);
			Spell NilahE;
			NilahE.name = "NilahE";
			NilahE.icon = "NilahE";
			NilahE.displayName = "Slipstream";
			NilahE.missileName = "";
			NilahE.slot = SpellSlot::E;
			NilahE.type = SpellType::linear;
			NilahE.range = 550;
			NilahE.radius = 150;
			NilahE.delay = 0.00;
			NilahE.danger = 2;
			NilahE.speed = 2200;
			NilahE.cc = false;
			NilahE.collisionMinions = false;
			NilahE.windwall = false;
			NilahE.hitbox = false;
			NilahE.fow = true;
			NilahE.exception = false;
			NilahE.extend = false;
			Nilah.Spells.push_back(NilahE);
			Spell NilahR;
			NilahR.name = "NilahR";
			NilahR.icon = "NilahR";
			NilahR.displayName = "Apotheosis";
			NilahR.missileName = "";
			NilahR.slot = SpellSlot::R;
			NilahR.type = SpellType::circular;
			NilahR.range = 0;
			NilahR.radius = 450;
			NilahR.delay = 1.0;
			NilahR.danger = 5;
			NilahR.speed = MathHuge;
			NilahR.cc = true;
			NilahR.collisionMinions = false;
			NilahR.windwall = false;
			NilahR.hitbox = false;
			NilahR.fow = true;
			NilahR.exception = false;
			NilahR.extend = false;
			Nilah.Spells.push_back(NilahR);
			Core::SpellDB.emplace_back(Nilah);
		}
		{
			Champ Nocturne;
			Nocturne.Name = "Nocturne";
			Spell NocturneDuskbringer;
			NocturneDuskbringer.name = "NocturneDuskbringer";
			NocturneDuskbringer.icon = "NocturneQ";
			NocturneDuskbringer.displayName = "Duskbringer";
			NocturneDuskbringer.missileName = "NocturneDuskbringer";
			NocturneDuskbringer.slot = SpellSlot::Q;
			NocturneDuskbringer.type = SpellType::linear;
			NocturneDuskbringer.range = 1200;
			NocturneDuskbringer.radius = 60;
			NocturneDuskbringer.delay = 0.25;
			NocturneDuskbringer.danger = 2;
			NocturneDuskbringer.speed = 1600;
			NocturneDuskbringer.cc = false;
			NocturneDuskbringer.collisionMinions = false;
			NocturneDuskbringer.windwall = true;
			NocturneDuskbringer.hitbox = true;
			NocturneDuskbringer.fow = true;
			NocturneDuskbringer.exception = false;
			NocturneDuskbringer.extend = true;
			Nocturne.Spells.push_back(NocturneDuskbringer);
			Core::SpellDB.emplace_back(Nocturne);
		}
		{
			Champ Nunu;
			Nunu.Name = "Nunu";
			Spell NunuR;
			NunuR.name = "NunuR";
			NunuR.icon = "NunuR";
			NunuR.displayName = "Absolute Zero";
			NunuR.missileName = "";
			NunuR.slot = SpellSlot::R;
			NunuR.type = SpellType::circular;
			NunuR.range = 0;
			NunuR.radius = 650;
			NunuR.delay = 3;
			NunuR.danger = 5;
			NunuR.speed = MathHuge;
			NunuR.cc = true;
			NunuR.collisionMinions = false;
			NunuR.windwall = false;
			NunuR.hitbox = false;
			NunuR.fow = false;
			NunuR.exception = false;
			NunuR.extend = false;
			Nunu.Spells.push_back(NunuR);
			Core::SpellDB.emplace_back(Nunu);
		}
		{
			Champ Olaf;
			Olaf.Name = "Olaf";
			Spell OlafAxeThrowCast;
			OlafAxeThrowCast.name = "OlafAxeThrowCast";
			OlafAxeThrowCast.icon = "OlafQ";
			OlafAxeThrowCast.displayName = "Undertow";
			OlafAxeThrowCast.missileName = "OlafAxeThrow";
			OlafAxeThrowCast.slot = SpellSlot::Q;
			OlafAxeThrowCast.type = SpellType::linear;
			OlafAxeThrowCast.range = 1000;
			OlafAxeThrowCast.radius = 90;
			OlafAxeThrowCast.delay = 0.25;
			OlafAxeThrowCast.danger = 2;
			OlafAxeThrowCast.speed = 1600;
			OlafAxeThrowCast.cc = true;
			OlafAxeThrowCast.collisionMinions = false;
			OlafAxeThrowCast.windwall = true;
			OlafAxeThrowCast.hitbox = true;
			OlafAxeThrowCast.fow = true;
			OlafAxeThrowCast.exception = false;
			OlafAxeThrowCast.extend = false;
			Olaf.Spells.push_back(OlafAxeThrowCast);
			Core::SpellDB.emplace_back(Olaf);
		}
		{
			Champ Orianna;
			Orianna.Name = "Orianna";
			Spell OrianaIzuna;
			OrianaIzuna.name = "OrianaIzuna";
			OrianaIzuna.icon = "OriannaQ";
			OrianaIzuna.displayName = "Command: Attack";
			OrianaIzuna.missileName = "OrianaIzuna";
			OrianaIzuna.slot = SpellSlot::Q;
			OrianaIzuna.type = SpellType::polygon;
			OrianaIzuna.range = 825;
			OrianaIzuna.radius = 80;
			OrianaIzuna.delay = 0;
			OrianaIzuna.danger = 2;
			OrianaIzuna.speed = 1400;
			OrianaIzuna.cc = false;
			OrianaIzuna.collisionMinions = false;
			OrianaIzuna.windwall = false;
			OrianaIzuna.hitbox = false;
			OrianaIzuna.fow = true;
			OrianaIzuna.exception = true;
			OrianaIzuna.extend = false;
			Orianna.Spells.push_back(OrianaIzuna);
			Core::SpellDB.emplace_back(Orianna);
		}
		{
			Champ Ornn;
			Ornn.Name = "Ornn";
			Spell OrnnQ;
			OrnnQ.name = "OrnnQ";
			OrnnQ.icon = "OrnnQ";
			OrnnQ.displayName = "Volcanic Rupture";
			OrnnQ.missileName = "";
			OrnnQ.slot = SpellSlot::Q;
			OrnnQ.type = SpellType::linear;
			OrnnQ.range = 800;
			OrnnQ.radius = 65;
			OrnnQ.delay = 0.3;
			OrnnQ.danger = 1;
			OrnnQ.speed = 1800;
			OrnnQ.cc = true;
			OrnnQ.collisionMinions = false;
			OrnnQ.windwall = true;
			OrnnQ.hitbox = true;
			OrnnQ.fow = false;
			OrnnQ.exception = false;
			OrnnQ.extend = true;
			Ornn.Spells.push_back(OrnnQ);
			Spell OrnnE;
			OrnnE.name = "OrnnE";
			OrnnE.icon = "OrnnE";
			OrnnE.displayName = "Searing Charge";
			OrnnE.missileName = "";
			OrnnE.slot = SpellSlot::E;
			OrnnE.type = SpellType::linear;
			OrnnE.range = 800;
			OrnnE.radius = 150;
			OrnnE.delay = 0.35;
			OrnnE.danger = 3;
			OrnnE.speed = 1600;
			OrnnE.cc = true;
			OrnnE.collisionMinions = false;
			OrnnE.windwall = false;
			OrnnE.hitbox = false;
			OrnnE.fow = false;
			OrnnE.exception = false;
			OrnnE.extend = true;
			Ornn.Spells.push_back(OrnnE);
			Spell OrnnRCharge;
			OrnnRCharge.name = "OrnnRCharge";
			OrnnRCharge.icon = "OrnnR";
			OrnnRCharge.displayName = "Call of the Forge God";
			OrnnRCharge.missileName = "";
			OrnnRCharge.slot = SpellSlot::R;
			OrnnRCharge.type = SpellType::linear;
			OrnnRCharge.range = 2500;
			OrnnRCharge.radius = 200;
			OrnnRCharge.delay = 0.5;
			OrnnRCharge.danger = 3;
			OrnnRCharge.speed = 1650;
			OrnnRCharge.cc = true;
			OrnnRCharge.collisionMinions = false;
			OrnnRCharge.windwall = true;
			OrnnRCharge.hitbox = true;
			OrnnRCharge.fow = false;
			OrnnRCharge.exception = false;
			OrnnRCharge.extend = true;
			Ornn.Spells.push_back(OrnnRCharge);
			Core::SpellDB.emplace_back(Ornn);
		}
		{
			Champ Pantheon;
			Pantheon.Name = "Pantheon";
			Spell PantheonQTap;
			PantheonQTap.name = "PantheonQTap";
			PantheonQTap.icon = "PantheonQ";
			PantheonQTap.displayName = "Comet Spear [Melee]";
			PantheonQTap.missileName = "";
			PantheonQTap.slot = SpellSlot::Q;
			PantheonQTap.type = SpellType::linear;
			PantheonQTap.range = 575;
			PantheonQTap.radius = 80;
			PantheonQTap.delay = 0.25;
			PantheonQTap.danger = 2;
			PantheonQTap.speed = MathHuge;
			PantheonQTap.cc = false;
			PantheonQTap.collisionMinions = false;
			PantheonQTap.windwall = false;
			PantheonQTap.hitbox = false;
			PantheonQTap.fow = false;
			PantheonQTap.exception = false;
			PantheonQTap.extend = true;
			Pantheon.Spells.push_back(PantheonQTap);
			Spell PantheonQMissile;
			PantheonQMissile.name = "PantheonQMissile";
			PantheonQMissile.icon = "PantheonQ";
			PantheonQMissile.displayName = "Comet Spear [Range]";
			PantheonQMissile.missileName = "PantheonQMissile";
			PantheonQMissile.slot = SpellSlot::Q;
			PantheonQMissile.type = SpellType::linear;
			PantheonQMissile.range = 1200;
			PantheonQMissile.radius = 60;
			PantheonQMissile.delay = 0.25;
			PantheonQMissile.danger = 2;
			PantheonQMissile.speed = 2700;
			PantheonQMissile.cc = false;
			PantheonQMissile.collisionMinions = false;
			PantheonQMissile.windwall = true;
			PantheonQMissile.hitbox = true;
			PantheonQMissile.fow = true;
			PantheonQMissile.exception = false;
			PantheonQMissile.extend = true;
			Pantheon.Spells.push_back(PantheonQMissile);
			Spell PantheonR;
			PantheonR.name = "PantheonR";
			PantheonR.icon = "PantheonR";
			PantheonR.displayName = "Grand Starfall";
			PantheonR.missileName = "";
			PantheonR.slot = SpellSlot::R;
			PantheonR.type = SpellType::linear;
			PantheonR.range = 1350;
			PantheonR.radius = 250;
			PantheonR.delay = 4;
			PantheonR.danger = 3;
			PantheonR.speed = 2250;
			PantheonR.cc = false;
			PantheonR.collisionMinions = false;
			PantheonR.windwall = true;
			PantheonR.hitbox = true;
			PantheonR.fow = false;
			PantheonR.exception = false;
			PantheonR.extend = false;
			Pantheon.Spells.push_back(PantheonR);
			Core::SpellDB.emplace_back(Pantheon);
		}
		{
			Champ Poppy;
			Poppy.Name = "Poppy";
			Spell PoppyQSpell;
			PoppyQSpell.name = "PoppyQSpell";
			PoppyQSpell.icon = "PoppyQ";
			PoppyQSpell.displayName = "Hammer Shock";
			PoppyQSpell.missileName = "";
			PoppyQSpell.slot = SpellSlot::Q;
			PoppyQSpell.type = SpellType::linear;
			PoppyQSpell.range = 430;
			PoppyQSpell.radius = 100;
			PoppyQSpell.delay = 0.332;
			PoppyQSpell.danger = 2;
			PoppyQSpell.speed = MathHuge;
			PoppyQSpell.cc = true;
			PoppyQSpell.collisionMinions = false;
			PoppyQSpell.windwall = false;
			PoppyQSpell.hitbox = false;
			PoppyQSpell.fow = false;
			PoppyQSpell.exception = false;
			PoppyQSpell.extend = true;
			Poppy.Spells.push_back(PoppyQSpell);
			Spell PoppyRSpell;
			PoppyRSpell.name = "PoppyRSpell";
			PoppyRSpell.icon = "PoppyR";
			PoppyRSpell.displayName = "Keeper's Verdict";
			PoppyRSpell.missileName = "PoppyRMissile";
			PoppyRSpell.slot = SpellSlot::R;
			PoppyRSpell.type = SpellType::linear;
			PoppyRSpell.range = 1200;
			PoppyRSpell.radius = 100;
			PoppyRSpell.delay = 0.33;
			PoppyRSpell.danger = 3;
			PoppyRSpell.speed = 2000;
			PoppyRSpell.cc = true;
			PoppyRSpell.collisionMinions = false;
			PoppyRSpell.windwall = true;
			PoppyRSpell.hitbox = true;
			PoppyRSpell.fow = true;
			PoppyRSpell.exception = false;
			PoppyRSpell.extend = true;
			Poppy.Spells.push_back(PoppyRSpell);
			Core::SpellDB.emplace_back(Poppy);
		}
		{
			Champ Pyke;
			Pyke.Name = "Pyke";
			Spell PykeQMelee;
			PykeQMelee.name = "PykeQMelee";
			PykeQMelee.icon = "PykeQ";
			PykeQMelee.displayName = "Bone Skewer [Melee]";
			PykeQMelee.missileName = "";
			PykeQMelee.slot = SpellSlot::Q;
			PykeQMelee.type = SpellType::linear;
			PykeQMelee.range = 400;
			PykeQMelee.radius = 70;
			PykeQMelee.delay = 0.25;
			PykeQMelee.danger = 2;
			PykeQMelee.speed = MathHuge;
			PykeQMelee.cc = true;
			PykeQMelee.collisionMinions = false;
			PykeQMelee.windwall = false;
			PykeQMelee.hitbox = false;
			PykeQMelee.fow = false;
			PykeQMelee.exception = false;
			PykeQMelee.extend = true;
			Pyke.Spells.push_back(PykeQMelee);
			Spell PykeQRange;
			PykeQRange.name = "PykeQRange";
			PykeQRange.icon = "PykeQ";
			PykeQRange.displayName = "Bone Skewer [Range]";
			PykeQRange.missileName = "PykeQRange";
			PykeQRange.slot = SpellSlot::Q;
			PykeQRange.type = SpellType::linear;
			PykeQRange.range = 1100;
			PykeQRange.radius = 70;
			PykeQRange.delay = 0.2;
			PykeQRange.danger = 2;
			PykeQRange.speed = 2000;
			PykeQRange.cc = true;
			PykeQRange.collisionMinions = true;
			PykeQRange.windwall = true;
			PykeQRange.hitbox = true;
			PykeQRange.fow = true;
			PykeQRange.exception = false;
			PykeQRange.extend = true;
			Pyke.Spells.push_back(PykeQRange);
			Spell PykeE;
			PykeE.name = "PykeE";
			PykeE.icon = "PykeE";
			PykeE.displayName = "Phantom Undertow";
			PykeE.missileName = "";
			PykeE.slot = SpellSlot::E;
			PykeE.type = SpellType::linear;
			PykeE.range = 12500;
			PykeE.radius = 110;
			PykeE.delay = 0;
			PykeE.danger = 2;
			PykeE.speed = 3000;
			PykeE.cc = true;
			PykeE.collisionMinions = false;
			PykeE.windwall = false;
			PykeE.hitbox = true;
			PykeE.fow = false;
			PykeE.exception = false;
			PykeE.extend = true;
			Pyke.Spells.push_back(PykeE);
			Spell PykeR;
			PykeR.name = "PykeR";
			PykeR.icon = "PykeR";
			PykeR.displayName = "Death from Below";
			PykeR.missileName = "";
			PykeR.slot = SpellSlot::R;
			PykeR.type = SpellType::circular;
			PykeR.range = 750;
			PykeR.radius = 100;
			PykeR.delay = 0.5;
			PykeR.danger = 5;
			PykeR.speed = MathHuge;
			PykeR.cc = false;
			PykeR.collisionMinions = false;
			PykeR.windwall = false;
			PykeR.hitbox = false;
			PykeR.fow = false;
			PykeR.exception = false;
			PykeR.extend = false;
			Pyke.Spells.push_back(PykeR);
			Core::SpellDB.emplace_back(Pyke);
		}
		{
			Champ Qiyana;
			Qiyana.Name = "Qiyana";
			Spell QiyanaQ;
			QiyanaQ.name = "QiyanaQ";
			QiyanaQ.icon = "QiyanaQ";
			QiyanaQ.displayName = "Edge of Ixtal";
			QiyanaQ.missileName = "";
			QiyanaQ.slot = SpellSlot::Q;
			QiyanaQ.type = SpellType::linear;
			QiyanaQ.range = 500;
			QiyanaQ.radius = 60;
			QiyanaQ.delay = 0.25;
			QiyanaQ.danger = 2;
			QiyanaQ.speed = MathHuge;
			QiyanaQ.cc = false;
			QiyanaQ.collisionMinions = false;
			QiyanaQ.windwall = false;
			QiyanaQ.hitbox = false;
			QiyanaQ.fow = false;
			QiyanaQ.exception = false;
			QiyanaQ.extend = true;
			Qiyana.Spells.push_back(QiyanaQ);
			Spell QiyanaQ_Grass;
			QiyanaQ_Grass.name = "QiyanaQ_Grass";
			QiyanaQ_Grass.icon = "QiyanaQGrass";
			QiyanaQ_Grass.displayName = "Edge of Ixtal [Grass]";
			QiyanaQ_Grass.missileName = "";
			QiyanaQ_Grass.slot = SpellSlot::Q;
			QiyanaQ_Grass.type = SpellType::linear;
			QiyanaQ_Grass.range = 925;
			QiyanaQ_Grass.radius = 70;
			QiyanaQ_Grass.delay = 0.25;
			QiyanaQ_Grass.danger = 2;
			QiyanaQ_Grass.speed = 1600;
			QiyanaQ_Grass.cc = false;
			QiyanaQ_Grass.collisionMinions = false;
			QiyanaQ_Grass.windwall = true;
			QiyanaQ_Grass.hitbox = true;
			QiyanaQ_Grass.fow = false;
			QiyanaQ_Grass.exception = false;
			QiyanaQ_Grass.extend = true;
			Qiyana.Spells.push_back(QiyanaQ_Grass);
			Spell QiyanaQ_Rock;
			QiyanaQ_Rock.name = "QiyanaQ_Rock";
			QiyanaQ_Rock.icon = "QiyanaQRock";
			QiyanaQ_Rock.displayName = "Edge of Ixtal [Rock]";
			QiyanaQ_Rock.missileName = "";
			QiyanaQ_Rock.slot = SpellSlot::Q;
			QiyanaQ_Rock.type = SpellType::linear;
			QiyanaQ_Rock.range = 925;
			QiyanaQ_Rock.radius = 70;
			QiyanaQ_Rock.delay = 0.25;
			QiyanaQ_Rock.danger = 2;
			QiyanaQ_Rock.speed = 1600;
			QiyanaQ_Rock.cc = false;
			QiyanaQ_Rock.collisionMinions = false;
			QiyanaQ_Rock.windwall = true;
			QiyanaQ_Rock.hitbox = true;
			QiyanaQ_Rock.fow = false;
			QiyanaQ_Rock.exception = false;
			QiyanaQ_Rock.extend = true;
			Qiyana.Spells.push_back(QiyanaQ_Rock);
			Spell QiyanaQ_Water;
			QiyanaQ_Water.name = "QiyanaQ_Water";
			QiyanaQ_Water.icon = "QiyanaQWater";
			QiyanaQ_Water.displayName = "Edge of Ixtal [Water]";
			QiyanaQ_Water.missileName = "";
			QiyanaQ_Water.slot = SpellSlot::Q;
			QiyanaQ_Water.type = SpellType::linear;
			QiyanaQ_Water.range = 925;
			QiyanaQ_Water.radius = 70;
			QiyanaQ_Water.delay = 0.25;
			QiyanaQ_Water.danger = 2;
			QiyanaQ_Water.speed = 1600;
			QiyanaQ_Water.cc = true;
			QiyanaQ_Water.collisionMinions = false;
			QiyanaQ_Water.windwall = true;
			QiyanaQ_Water.hitbox = true;
			QiyanaQ_Water.fow = false;
			QiyanaQ_Water.exception = false;
			QiyanaQ_Water.extend = true;
			Qiyana.Spells.push_back(QiyanaQ_Water);
			Spell QiyanaR;
			QiyanaR.name = "QiyanaR";
			QiyanaR.icon = "QiyanaR";
			QiyanaR.displayName = "Supreme Display of Talent";
			QiyanaR.missileName = "";
			QiyanaR.slot = SpellSlot::R;
			QiyanaR.type = SpellType::linear;
			QiyanaR.range = 950;
			QiyanaR.radius = 190;
			QiyanaR.delay = 0.25;
			QiyanaR.danger = 4;
			QiyanaR.speed = 2000;
			QiyanaR.cc = true;
			QiyanaR.collisionMinions = false;
			QiyanaR.windwall = true;
			QiyanaR.hitbox = true;
			QiyanaR.fow = false;
			QiyanaR.exception = false;
			QiyanaR.extend = true;
			Qiyana.Spells.push_back(QiyanaR);
			Core::SpellDB.emplace_back(Qiyana);
		}
		{
			Champ Quinn;
			Quinn.Name = "Quinn";
			Spell QuinnQ;
			QuinnQ.name = "QuinnQ";
			QuinnQ.icon = "QuinnQ";
			QuinnQ.displayName = "Blinding Assault";
			QuinnQ.missileName = "QuinnQ";
			QuinnQ.slot = SpellSlot::Q;
			QuinnQ.type = SpellType::linear;
			QuinnQ.range = 1025;
			QuinnQ.radius = 60;
			QuinnQ.delay = 0.25;
			QuinnQ.danger = 1;
			QuinnQ.speed = 1550;
			QuinnQ.cc = false;
			QuinnQ.collisionMinions = true;
			QuinnQ.windwall = true;
			QuinnQ.hitbox = true;
			QuinnQ.fow = true;
			QuinnQ.exception = false;
			QuinnQ.extend = true;
			Quinn.Spells.push_back(QuinnQ);
			Core::SpellDB.emplace_back(Quinn);
		}
		{
			Champ Rakan;
			Rakan.Name = "Rakan";
			Spell RakanQ;
			RakanQ.name = "RakanQ";
			RakanQ.icon = "RakanQ";
			RakanQ.displayName = "Gleaming Quill";
			RakanQ.missileName = "RakanQMis";
			RakanQ.slot = SpellSlot::Q;
			RakanQ.type = SpellType::linear;
			RakanQ.range = 850;
			RakanQ.radius = 65;
			RakanQ.delay = 0.25;
			RakanQ.danger = 1;
			RakanQ.speed = 1850;
			RakanQ.cc = false;
			RakanQ.collisionMinions = true;
			RakanQ.windwall = true;
			RakanQ.hitbox = true;
			RakanQ.fow = true;
			RakanQ.exception = false;
			RakanQ.extend = true;
			Rakan.Spells.push_back(RakanQ);
			Spell RakanW;
			RakanW.name = "RakanW";
			RakanW.icon = "RakanW";
			RakanW.displayName = "Grand Entrance";
			RakanW.missileName = "";
			RakanW.slot = SpellSlot::W;
			RakanW.type = SpellType::circular;
			RakanW.range = 650;
			RakanW.radius = 265;
			RakanW.delay = 0.7;
			RakanW.danger = 3;
			RakanW.speed = MathHuge;
			RakanW.cc = true;
			RakanW.collisionMinions = false;
			RakanW.windwall = false;
			RakanW.hitbox = false;
			RakanW.fow = false;
			RakanW.exception = false;
			RakanW.extend = false;
			Rakan.Spells.push_back(RakanW);
			Core::SpellDB.emplace_back(Rakan);
		}
		{
			Champ RekSai;
			RekSai.Name = "RekSai";
			Spell RekSaiQBurrowed;
			RekSaiQBurrowed.name = "RekSaiQBurrowed";
			RekSaiQBurrowed.icon = "RekSaiQ";
			RekSaiQBurrowed.displayName = "Prey Seeker";
			RekSaiQBurrowed.missileName = "RekSaiQBurrowedMis";
			RekSaiQBurrowed.slot = SpellSlot::Q;
			RekSaiQBurrowed.type = SpellType::linear;
			RekSaiQBurrowed.range = 1625;
			RekSaiQBurrowed.radius = 65;
			RekSaiQBurrowed.delay = 0.125;
			RekSaiQBurrowed.danger = 2;
			RekSaiQBurrowed.speed = 1950;
			RekSaiQBurrowed.cc = false;
			RekSaiQBurrowed.collisionMinions = true;
			RekSaiQBurrowed.windwall = true;
			RekSaiQBurrowed.hitbox = true;
			RekSaiQBurrowed.fow = true;
			RekSaiQBurrowed.exception = false;
			RekSaiQBurrowed.extend = true;
			RekSai.Spells.push_back(RekSaiQBurrowed);
			Core::SpellDB.emplace_back(RekSai);
		}
		{
			Champ Rell;
			Rell.Name = "Rell";
			Spell RellQ;
			RellQ.name = "RellQ";
			RellQ.icon = "RellQ";
			RellQ.displayName = "Shattering Strike";
			RellQ.missileName = "";
			RellQ.slot = SpellSlot::Q;
			RellQ.type = SpellType::linear;
			RellQ.range = 685;
			RellQ.radius = 80;
			RellQ.delay = 0.35;
			RellQ.danger = 2;
			RellQ.speed = MathHuge;
			RellQ.cc = false;
			RellQ.collisionMinions = false;
			RellQ.windwall = false;
			RellQ.hitbox = true;
			RellQ.fow = false;
			RellQ.exception = false;
			RellQ.extend = true;
			Rell.Spells.push_back(RellQ);
			Spell RellW;
			RellW.name = "RellW";
			RellW.icon = "RellW";
			RellW.displayName = "Crash Down";
			RellW.missileName = "";
			RellW.slot = SpellSlot::W;
			RellW.type = SpellType::linear;
			RellW.range = 500;
			RellW.radius = 200;
			RellW.delay = 0.625;
			RellW.danger = 3;
			RellW.speed = MathHuge;
			RellW.cc = true;
			RellW.collisionMinions = false;
			RellW.windwall = false;
			RellW.hitbox = true;
			RellW.fow = false;
			RellW.exception = false;
			RellW.extend = true;
			Rell.Spells.push_back(RellW);
			Spell RellE;
			RellE.name = "RellE";
			RellE.icon = "RellE";
			RellE.displayName = "Attract and Repel";
			RellE.missileName = "";
			RellE.slot = SpellSlot::E;
			RellE.type = SpellType::linear;
			RellE.range = 1500;
			RellE.radius = 250;
			RellE.delay = 0.35;
			RellE.danger = 3;
			RellE.speed = MathHuge;
			RellE.cc = true;
			RellE.collisionMinions = false;
			RellE.windwall = false;
			RellE.hitbox = true;
			RellE.fow = false;
			RellE.exception = false;
			RellE.extend = true;
			Rell.Spells.push_back(RellE);
			Spell RellR;
			RellR.name = "RellR";
			RellR.icon = "RellR";
			RellR.displayName = "Magnet Storm";
			RellR.missileName = "";
			RellR.slot = SpellSlot::R;
			RellR.type = SpellType::circular;
			RellR.range = 0;
			RellR.radius = 400;
			RellR.delay = 0.25;
			RellR.danger = 5;
			RellR.speed = MathHuge;
			RellR.cc = true;
			RellR.collisionMinions = false;
			RellR.windwall = false;
			RellR.hitbox = false;
			RellR.fow = false;
			RellR.exception = false;
			RellR.extend = false;
			Rell.Spells.push_back(RellR);
			Core::SpellDB.emplace_back(Rell);
		}
		{
			Champ Rengar;
			Rengar.Name = "Rengar";
			Spell RengarE;
			RengarE.name = "RengarE";
			RengarE.icon = "RengarE";
			RengarE.displayName = "Bola Strike";
			RengarE.missileName = "RengarEMis";
			RengarE.slot = SpellSlot::E;
			RengarE.type = SpellType::linear;
			RengarE.range = 1000;
			RengarE.radius = 70;
			RengarE.delay = 0.25;
			RengarE.danger = 1;
			RengarE.speed = 1500;
			RengarE.cc = true;
			RengarE.collisionMinions = true;
			RengarE.windwall = true;
			RengarE.hitbox = true;
			RengarE.fow = true;
			RengarE.exception = false;
			RengarE.extend = true;
			Rengar.Spells.push_back(RengarE);
			Core::SpellDB.emplace_back(Rengar);
		}
		{
			Champ Riven;
			Riven.Name = "Riven";
			Spell RivenIzunaBlade;
			RivenIzunaBlade.name = "RivenIzunaBlade";
			RivenIzunaBlade.icon = "RivenR";
			RivenIzunaBlade.displayName = "Wind Slash";
			RivenIzunaBlade.missileName = "";
			RivenIzunaBlade.slot = SpellSlot::R;
			RivenIzunaBlade.type = SpellType::conic;
			RivenIzunaBlade.range = 900;
			RivenIzunaBlade.radius = 0;
			RivenIzunaBlade.delay = 0.25;
			RivenIzunaBlade.danger = 5;
			RivenIzunaBlade.speed = 1600;
			RivenIzunaBlade.cc = false;
			RivenIzunaBlade.collisionMinions = false;
			RivenIzunaBlade.windwall = true;
			RivenIzunaBlade.hitbox = false;
			RivenIzunaBlade.fow = false;
			RivenIzunaBlade.exception = false;
			RivenIzunaBlade.extend = true;
			Riven.Spells.push_back(RivenIzunaBlade);
			Core::SpellDB.emplace_back(Riven);
		}
		{
			Champ Rumble;
			Rumble.Name = "Rumble";
			Spell RumbleGrenade;
			RumbleGrenade.name = "RumbleGrenade";
			RumbleGrenade.icon = "RumbleE";
			RumbleGrenade.displayName = "Electro Harpoon";
			RumbleGrenade.missileName = "RumbleGrenadeMissile";
			RumbleGrenade.slot = SpellSlot::E;
			RumbleGrenade.type = SpellType::linear;
			RumbleGrenade.range = 850;
			RumbleGrenade.radius = 60;
			RumbleGrenade.delay = 0.25;
			RumbleGrenade.danger = 2;
			RumbleGrenade.speed = 2000;
			RumbleGrenade.cc = true;
			RumbleGrenade.collisionMinions = true;
			RumbleGrenade.windwall = true;
			RumbleGrenade.hitbox = true;
			RumbleGrenade.fow = true;
			RumbleGrenade.exception = false;
			RumbleGrenade.extend = true;
			Rumble.Spells.push_back(RumbleGrenade);
			Core::SpellDB.emplace_back(Rumble);
		}
		{
			Champ Ryze;
			Ryze.Name = "Ryze";
			Spell RyzeQ;
			RyzeQ.name = "RyzeQ";
			RyzeQ.icon = "RyzeQ";
			RyzeQ.displayName = "Overload";
			RyzeQ.missileName = "RyzeQ";
			RyzeQ.slot = SpellSlot::Q;
			RyzeQ.type = SpellType::linear;
			RyzeQ.range = 1000;
			RyzeQ.radius = 55;
			RyzeQ.delay = 0.25;
			RyzeQ.danger = 1;
			RyzeQ.speed = 1700;
			RyzeQ.cc = false;
			RyzeQ.collisionMinions = true;
			RyzeQ.windwall = true;
			RyzeQ.hitbox = true;
			RyzeQ.fow = true;
			RyzeQ.exception = false;
			RyzeQ.extend = true;
			Ryze.Spells.push_back(RyzeQ);
			Core::SpellDB.emplace_back(Ryze);
		}
		{
			Champ Semira;
			Semira.Name = "Semira";
			Spell SemiraQGun;
			SemiraQGun.name = "SemiraQGun";
			SemiraQGun.icon = "SemiraQ";
			SemiraQGun.displayName = "Flair";
			SemiraQGun.missileName = "SamiraQGun";
			SemiraQGun.slot = SpellSlot::Q;
			SemiraQGun.type = SpellType::linear;
			SemiraQGun.range = 1000;
			SemiraQGun.radius = 60;
			SemiraQGun.delay = 0.25;
			SemiraQGun.danger = 1;
			SemiraQGun.speed = 2600;
			SemiraQGun.cc = false;
			SemiraQGun.collisionMinions = true;
			SemiraQGun.windwall = true;
			SemiraQGun.hitbox = true;
			SemiraQGun.fow = true;
			SemiraQGun.exception = false;
			SemiraQGun.extend = true;
			Semira.Spells.push_back(SemiraQGun);
			Core::SpellDB.emplace_back(Semira);
		}
		{
			Champ Sejuani;
			Sejuani.Name = "Sejuani";
			Spell SejuaniR;
			SejuaniR.name = "SejuaniR";
			SejuaniR.icon = "SejuaniR";
			SejuaniR.displayName = "Glacial Prison";
			SejuaniR.missileName = "SejuaniRMissile";
			SejuaniR.slot = SpellSlot::R;
			SejuaniR.type = SpellType::linear;
			SejuaniR.range = 1300;
			SejuaniR.radius = 120;
			SejuaniR.delay = 0.25;
			SejuaniR.danger = 5;
			SejuaniR.speed = 1600;
			SejuaniR.cc = true;
			SejuaniR.collisionMinions = false;
			SejuaniR.windwall = true;
			SejuaniR.hitbox = true;
			SejuaniR.fow = true;
			SejuaniR.exception = false;
			SejuaniR.extend = true;
			Sejuani.Spells.push_back(SejuaniR);
			Core::SpellDB.emplace_back(Sejuani);
		}
		{
			Champ Senna;
			Senna.Name = "Senna";
			Spell SennaQCast;
			SennaQCast.name = "SennaQCast";
			SennaQCast.icon = "SennaQ";
			SennaQCast.displayName = "Piercing Darkness";
			SennaQCast.missileName = "";
			SennaQCast.slot = SpellSlot::Q;
			SennaQCast.type = SpellType::linear;
			SennaQCast.range = 1400;
			SennaQCast.radius = 80;
			SennaQCast.delay = 0.4;
			SennaQCast.danger = 2;
			SennaQCast.speed = MathHuge;
			SennaQCast.cc = false;
			SennaQCast.collisionMinions = false;
			SennaQCast.windwall = false;
			SennaQCast.hitbox = true;
			SennaQCast.fow = false;
			SennaQCast.exception = false;
			SennaQCast.extend = true;
			Senna.Spells.push_back(SennaQCast);
			Spell SennaW;
			SennaW.name = "SennaW";
			SennaW.icon = "SennaW";
			SennaW.displayName = "Last Embrace";
			SennaW.missileName = "SennaW";
			SennaW.slot = SpellSlot::W;
			SennaW.type = SpellType::linear;
			SennaW.range = 1300;
			SennaW.radius = 60;
			SennaW.delay = 0.25;
			SennaW.danger = 1;
			SennaW.speed = 1150;
			SennaW.cc = true;
			SennaW.collisionMinions = true;
			SennaW.windwall = true;
			SennaW.hitbox = true;
			SennaW.fow = true;
			SennaW.exception = false;
			SennaW.extend = true;
			Senna.Spells.push_back(SennaW);
			Spell SennaR;
			SennaR.name = "SennaR";
			SennaR.icon = "SennaR";
			SennaR.displayName = "Dawning Shadow";
			SennaR.missileName = "SennaRWarningMis";
			SennaR.slot = SpellSlot::R;
			SennaR.type = SpellType::linear;
			SennaR.range = 12500;
			SennaR.radius = 180;
			SennaR.delay = 1;
			SennaR.danger = 4;
			SennaR.speed = 20000;
			SennaR.cc = false;
			SennaR.collisionMinions = false;
			SennaR.windwall = true;
			SennaR.hitbox = true;
			SennaR.fow = true;
			SennaR.exception = false;
			SennaR.extend = true;
			Senna.Spells.push_back(SennaR);
			Core::SpellDB.emplace_back(Senna);
		}
		{
			Champ Seraphine;
			Seraphine.Name = "Seraphine";
			Spell SeraphineQCast;
			SeraphineQCast.name = "SeraphineQCast";
			SeraphineQCast.icon = "SeraphineQ";
			SeraphineQCast.displayName = "High Note";
			SeraphineQCast.missileName = "SeraphineQInitialMissile";
			SeraphineQCast.slot = SpellSlot::Q;
			SeraphineQCast.type = SpellType::circular;
			SeraphineQCast.range = 900;
			SeraphineQCast.radius = 350;
			SeraphineQCast.delay = 0.25;
			SeraphineQCast.danger = 2;
			SeraphineQCast.speed = 1200;
			SeraphineQCast.cc = false;
			SeraphineQCast.collisionMinions = false;
			SeraphineQCast.windwall = true;
			SeraphineQCast.hitbox = false;
			SeraphineQCast.fow = true;
			SeraphineQCast.exception = false;
			SeraphineQCast.extend = false;
			Seraphine.Spells.push_back(SeraphineQCast);
			Spell SeraphineECast;
			SeraphineECast.name = "SeraphineECast";
			SeraphineECast.icon = "SeraphineE";
			SeraphineECast.displayName = "Beat Drop";
			SeraphineECast.missileName = "SeraphineEMissile";
			SeraphineECast.slot = SpellSlot::E;
			SeraphineECast.type = SpellType::linear;
			SeraphineECast.range = 1300;
			SeraphineECast.radius = 70;
			SeraphineECast.delay = 0.25;
			SeraphineECast.danger = 1;
			SeraphineECast.speed = 1200;
			SeraphineECast.cc = true;
			SeraphineECast.collisionMinions = false;
			SeraphineECast.windwall = true;
			SeraphineECast.hitbox = true;
			SeraphineECast.fow = true;
			SeraphineECast.exception = false;
			SeraphineECast.extend = true;
			Seraphine.Spells.push_back(SeraphineECast);
			Spell SeraphineR;
			SeraphineR.name = "SeraphineR";
			SeraphineR.icon = "SeraphineR";
			SeraphineR.displayName = "Encore";
			SeraphineR.missileName = "SeraphineR";
			SeraphineR.slot = SpellSlot::R;
			SeraphineR.type = SpellType::linear;
			SeraphineR.range = 1300;
			SeraphineR.radius = 160;
			SeraphineR.delay = 0.5;
			SeraphineR.danger = 3;
			SeraphineR.speed = 1600;
			SeraphineR.cc = true;
			SeraphineR.collisionMinions = false;
			SeraphineR.windwall = true;
			SeraphineR.hitbox = true;
			SeraphineR.fow = true;
			SeraphineR.exception = false;
			SeraphineR.extend = true;
			Seraphine.Spells.push_back(SeraphineR);
			Core::SpellDB.emplace_back(Seraphine);
		}
		{
			Champ Sett;
			Sett.Name = "Sett";
			Spell SettW;
			SettW.name = "SettW";
			SettW.icon = "SettW";
			SettW.displayName = "Haymaker";
			SettW.missileName = "";
			SettW.slot = SpellSlot::W;
			SettW.type = SpellType::polygon;
			SettW.range = 790;
			SettW.radius = 160;
			SettW.delay = 0.75;
			SettW.danger = 2;
			SettW.speed = MathHuge;
			SettW.cc = false;
			SettW.collisionMinions = false;
			SettW.windwall = false;
			SettW.hitbox = false;
			SettW.fow = false;
			SettW.exception = false;
			SettW.extend = true;
			Sett.Spells.push_back(SettW);
			Spell SettE;
			SettE.name = "SettE";
			SettE.icon = "SettE";
			SettE.displayName = "Facebreaker";
			SettE.missileName = "";
			SettE.slot = SpellSlot::E;
			SettE.type = SpellType::polygon;
			SettE.range = 490;
			SettE.radius = 175;
			SettE.delay = 0.25;
			SettE.danger = 3;
			SettE.speed = MathHuge;
			SettE.cc = true;
			SettE.collisionMinions = false;
			SettE.windwall = false;
			SettE.hitbox = false;
			SettE.fow = false;
			SettE.exception = false;
			SettE.extend = true;
			Sett.Spells.push_back(SettE);
			Core::SpellDB.emplace_back(Sett);
		}
		{
			Champ Shyvana;
			Shyvana.Name = "Shyvana";
			Spell ShyvanaFireball;
			ShyvanaFireball.name = "ShyvanaFireball";
			ShyvanaFireball.icon = "ShyvanaE";
			ShyvanaFireball.displayName = "Flame Breath [Standard]";
			ShyvanaFireball.missileName = "ShyvanaFireballMissile";
			ShyvanaFireball.slot = SpellSlot::E;
			ShyvanaFireball.type = SpellType::linear;
			ShyvanaFireball.range = 925;
			ShyvanaFireball.radius = 60;
			ShyvanaFireball.delay = 0.25;
			ShyvanaFireball.danger = 1;
			ShyvanaFireball.speed = 1575;
			ShyvanaFireball.cc = false;
			ShyvanaFireball.collisionMinions = false;
			ShyvanaFireball.windwall = true;
			ShyvanaFireball.hitbox = false;
			ShyvanaFireball.fow = true;
			ShyvanaFireball.exception = false;
			ShyvanaFireball.extend = true;
			Shyvana.Spells.push_back(ShyvanaFireball);
			Spell ShyvanaFireballDragon2;
			ShyvanaFireballDragon2.name = "ShyvanaFireballDragon2";
			ShyvanaFireballDragon2.icon = "ShyvanaE";
			ShyvanaFireballDragon2.displayName = "Flame Breath [Dragon]";
			ShyvanaFireballDragon2.missileName = "ShyvanaFireballDragonMissile";
			ShyvanaFireballDragon2.slot = SpellSlot::E;
			ShyvanaFireballDragon2.type = SpellType::linear;
			ShyvanaFireballDragon2.range = 975;
			ShyvanaFireballDragon2.radius = 60;
			ShyvanaFireballDragon2.delay = 0.333;
			ShyvanaFireballDragon2.danger = 2;
			ShyvanaFireballDragon2.speed = 1575;
			ShyvanaFireballDragon2.cc = false;
			ShyvanaFireballDragon2.collisionMinions = false;
			ShyvanaFireballDragon2.windwall = true;
			ShyvanaFireballDragon2.hitbox = false;
			ShyvanaFireballDragon2.fow = true;
			ShyvanaFireballDragon2.exception = false;
			ShyvanaFireballDragon2.extend = true;
			Shyvana.Spells.push_back(ShyvanaFireballDragon2);
			Spell ShyvanaTransformLeap;
			ShyvanaTransformLeap.name = "ShyvanaTransformLeap";
			ShyvanaTransformLeap.icon = "ShyvanaR";
			ShyvanaTransformLeap.displayName = "Transform Leap";
			ShyvanaTransformLeap.missileName = "";
			ShyvanaTransformLeap.slot = SpellSlot::R;
			ShyvanaTransformLeap.type = SpellType::linear;
			ShyvanaTransformLeap.range = 850;
			ShyvanaTransformLeap.radius = 150;
			ShyvanaTransformLeap.delay = 0.25;
			ShyvanaTransformLeap.danger = 4;
			ShyvanaTransformLeap.speed = 700;
			ShyvanaTransformLeap.cc = true;
			ShyvanaTransformLeap.collisionMinions = false;
			ShyvanaTransformLeap.windwall = false;
			ShyvanaTransformLeap.hitbox = false;
			ShyvanaTransformLeap.fow = false;
			ShyvanaTransformLeap.exception = false;
			ShyvanaTransformLeap.extend = true;
			Shyvana.Spells.push_back(ShyvanaTransformLeap);
			Core::SpellDB.emplace_back(Shyvana);
		}
		{
			Champ Sion;
			Sion.Name = "Sion";
			Spell SionQ;
			SionQ.name = "SionQ";
			SionQ.icon = "SionQ";
			SionQ.displayName = "Decimating Smash";
			SionQ.missileName = "";
			SionQ.slot = SpellSlot::Q;
			SionQ.type = SpellType::linear;
			SionQ.range = 750;
			SionQ.radius = 150;
			SionQ.delay = 2;
			SionQ.danger = 3;
			SionQ.speed = MathHuge;
			SionQ.cc = true;
			SionQ.collisionMinions = false;
			SionQ.windwall = false;
			SionQ.hitbox = false;
			SionQ.fow = false;
			SionQ.exception = false;
			SionQ.extend = true;
			Sion.Spells.push_back(SionQ);
			Spell SionE;
			SionE.name = "SionE";
			SionE.icon = "SionE";
			SionE.displayName = "Roar of the Slayer";
			SionE.missileName = "SionEMissile";
			SionE.slot = SpellSlot::E;
			SionE.type = SpellType::linear;
			SionE.range = 800;
			SionE.radius = 80;
			SionE.delay = 0.25;
			SionE.danger = 2;
			SionE.speed = 1800;
			SionE.cc = true;
			SionE.collisionMinions = false;
			SionE.windwall = true;
			SionE.hitbox = false;
			SionE.fow = true;
			SionE.exception = false;
			SionE.extend = true;
			Sion.Spells.push_back(SionE);
			Core::SpellDB.emplace_back(Sion);
		}
		{
			Champ Sivir;
			Sivir.Name = "Sivir";
			Spell SivirQ;
			SivirQ.name = "SivirQ";
			SivirQ.icon = "SivirQ";
			SivirQ.displayName = "Boomerang Blade";
			SivirQ.missileName = "SivirQMissile";
			SivirQ.slot = SpellSlot::Q;
			SivirQ.type = SpellType::linear;
			SivirQ.range = 1250;
			SivirQ.radius = 90;
			SivirQ.delay = 0.25;
			SivirQ.danger = 2;
			SivirQ.speed = 1350;
			SivirQ.cc = false;
			SivirQ.collisionMinions = false;
			SivirQ.windwall = true;
			SivirQ.hitbox = false;
			SivirQ.fow = true;
			SivirQ.exception = false;
			SivirQ.extend = true;
			Sivir.Spells.push_back(SivirQ);
			Core::SpellDB.emplace_back(Sivir);
		}
		{
			Champ Skarner;
			Skarner.Name = "Skarner";
			Spell SkarnerFractureMissile;
			SkarnerFractureMissile.name = "SkarnerFractureMissile";
			SkarnerFractureMissile.icon = "SkarnerE";
			SkarnerFractureMissile.displayName = "Fracture";
			SkarnerFractureMissile.missileName = "SkarnerFractureMissile";
			SkarnerFractureMissile.slot = SpellSlot::E;
			SkarnerFractureMissile.type = SpellType::linear;
			SkarnerFractureMissile.range = 1000;
			SkarnerFractureMissile.radius = 70;
			SkarnerFractureMissile.delay = 0.25;
			SkarnerFractureMissile.danger = 1;
			SkarnerFractureMissile.speed = 1500;
			SkarnerFractureMissile.cc = true;
			SkarnerFractureMissile.collisionMinions = false;
			SkarnerFractureMissile.windwall = true;
			SkarnerFractureMissile.hitbox = false;
			SkarnerFractureMissile.fow = true;
			SkarnerFractureMissile.exception = false;
			SkarnerFractureMissile.extend = true;
			Skarner.Spells.push_back(SkarnerFractureMissile);
			Core::SpellDB.emplace_back(Skarner);
		}
		{
			Champ Sona;
			Sona.Name = "Sona";
			Spell SonaR;
			SonaR.name = "SonaR";
			SonaR.icon = "SonaR";
			SonaR.displayName = "Crescendo";
			SonaR.missileName = "SonaRMissile";
			SonaR.slot = SpellSlot::R;
			SonaR.type = SpellType::linear;
			SonaR.range = 1000;
			SonaR.radius = 140;
			SonaR.delay = 0.25;
			SonaR.danger = 5;
			SonaR.speed = 2400;
			SonaR.cc = true;
			SonaR.collisionMinions = false;
			SonaR.windwall = true;
			SonaR.hitbox = false;
			SonaR.fow = true;
			SonaR.exception = false;
			SonaR.extend = true;
			Sona.Spells.push_back(SonaR);
			Core::SpellDB.emplace_back(Sona);
		}
		{
			Champ Soraka;
			Soraka.Name = "Soraka";
			Spell SorakaQ;
			SorakaQ.name = "SorakaQ";
			SorakaQ.icon = "SorakaQ";
			SorakaQ.displayName = "Starcall";
			SorakaQ.missileName = "SorakaQMissile";
			SorakaQ.slot = SpellSlot::Q;
			SorakaQ.type = SpellType::circular;
			SorakaQ.range = 810;
			SorakaQ.radius = 235;
			SorakaQ.delay = 0.25;
			SorakaQ.danger = 2;
			SorakaQ.speed = 1150;
			SorakaQ.cc = true;
			SorakaQ.collisionMinions = false;
			SorakaQ.windwall = false;
			SorakaQ.hitbox = false;
			SorakaQ.fow = true;
			SorakaQ.exception = false;
			SorakaQ.extend = false;
			Soraka.Spells.push_back(SorakaQ);
			Core::SpellDB.emplace_back(Soraka);
		}
		{
			Champ Swain;
			Swain.Name = "Swain";
			Spell SwainQ;
			SwainQ.name = "SwainQ";
			SwainQ.icon = "SwainQ";
			SwainQ.displayName = "Death's Hand";
			SwainQ.missileName = "";
			SwainQ.slot = SpellSlot::Q;
			SwainQ.type = SpellType::conic;
			SwainQ.range = 725;
			SwainQ.radius = 0;
			SwainQ.delay = 0.25;
			SwainQ.danger = 2;
			SwainQ.speed = 5000;
			SwainQ.cc = false;
			SwainQ.collisionMinions = false;
			SwainQ.windwall = true;
			SwainQ.hitbox = false;
			SwainQ.fow = false;
			SwainQ.exception = false;
			SwainQ.extend = true;
			Swain.Spells.push_back(SwainQ);
			Spell SwainW;
			SwainW.name = "SwainW";
			SwainW.icon = "SwainW";
			SwainW.displayName = "Vision of Empire";
			SwainW.missileName = "";
			SwainW.slot = SpellSlot::W;
			SwainW.type = SpellType::circular;
			SwainW.range = 3500;
			SwainW.radius = 300;
			SwainW.delay = 1.5;
			SwainW.danger = 1;
			SwainW.speed = MathHuge;
			SwainW.cc = true;
			SwainW.collisionMinions = false;
			SwainW.windwall = false;
			SwainW.hitbox = false;
			SwainW.fow = false;
			SwainW.exception = false;
			SwainW.extend = false;
			Swain.Spells.push_back(SwainW);
			Spell SwainE;
			SwainE.name = "SwainE";
			SwainE.icon = "SwainE";
			SwainE.displayName = "Nevermove";
			SwainE.missileName = "";
			SwainE.slot = SpellSlot::E;
			SwainE.type = SpellType::linear;
			SwainE.range = 850;
			SwainE.radius = 85;
			SwainE.delay = 0.25;
			SwainE.danger = 2;
			SwainE.speed = 1800;
			SwainE.cc = true;
			SwainE.collisionMinions = false;
			SwainE.windwall = true;
			SwainE.hitbox = false;
			SwainE.fow = false;
			SwainE.exception = false;
			SwainE.extend = true;
			Swain.Spells.push_back(SwainE);
			Core::SpellDB.emplace_back(Swain);
		}
		{
			Champ Sylas;
			Sylas.Name = "Sylas";
			Spell SylasQ;
			SylasQ.name = "SylasQ";
			SylasQ.icon = "SylasQ";
			SylasQ.displayName = "Chain Lash";
			SylasQ.missileName = "";
			SylasQ.slot = SpellSlot::Q;
			SylasQ.type = SpellType::polygon;
			SylasQ.range = 775;
			SylasQ.radius = 45;
			SylasQ.delay = 0.4;
			SylasQ.danger = 2;
			SylasQ.speed = MathHuge;
			SylasQ.cc = true;
			SylasQ.collisionMinions = false;
			SylasQ.windwall = false;
			SylasQ.hitbox = false;
			SylasQ.fow = false;
			SylasQ.exception = false;
			SylasQ.extend = false;
			Sylas.Spells.push_back(SylasQ);
			Spell SylasE2;
			SylasE2.name = "SylasE2";
			SylasE2.icon = "SylasE";
			SylasE2.displayName = "Abduct";
			SylasE2.missileName = "SylasE2Mis";
			SylasE2.slot = SpellSlot::E;
			SylasE2.type = SpellType::linear;
			SylasE2.range = 850;
			SylasE2.radius = 60;
			SylasE2.delay = 0.25;
			SylasE2.danger = 2;
			SylasE2.speed = 1600;
			SylasE2.cc = true;
			SylasE2.collisionMinions = true;
			SylasE2.windwall = true;
			SylasE2.hitbox = true;
			SylasE2.fow = true;
			SylasE2.exception = false;
			SylasE2.extend = true;
			Sylas.Spells.push_back(SylasE2);
			Core::SpellDB.emplace_back(Sylas);
		}
		{
			Champ Syndra;
			Syndra.Name = "Syndra";
			Spell SyndraQSpell;
			SyndraQSpell.name = "SyndraQSpell";
			SyndraQSpell.icon = "SyndraQ";
			SyndraQSpell.displayName = "Dark Sphere";
			SyndraQSpell.missileName = "SyndraQSpell";
			SyndraQSpell.slot = SpellSlot::Q;
			SyndraQSpell.type = SpellType::circular;
			SyndraQSpell.range = 800;
			SyndraQSpell.radius = 200;
			SyndraQSpell.delay = 0.625;
			SyndraQSpell.danger = 2;
			SyndraQSpell.speed = MathHuge;
			SyndraQSpell.cc = false;
			SyndraQSpell.collisionMinions = false;
			SyndraQSpell.windwall = false;
			SyndraQSpell.hitbox = false;
			SyndraQSpell.fow = true;
			SyndraQSpell.exception = true;
			SyndraQSpell.extend = false;
			Syndra.Spells.push_back(SyndraQSpell);
			Spell SyndraE;
			SyndraE.name = "SyndraE";
			SyndraE.icon = "SyndraE";
			SyndraE.displayName = "Scatter the Weak [Standard]";
			SyndraE.missileName = "";
			SyndraE.slot = SpellSlot::E;
			SyndraE.type = SpellType::conic;
			SyndraE.range = 700;
			SyndraE.radius = 0;
			SyndraE.delay = 0.25;
			SyndraE.danger = 3;
			SyndraE.speed = 1600;
			SyndraE.cc = true;
			SyndraE.collisionMinions = false;
			SyndraE.windwall = true;
			SyndraE.hitbox = false;
			SyndraE.fow = false;
			SyndraE.exception = false;
			SyndraE.extend = true;
			Syndra.Spells.push_back(SyndraE);
			Spell SyndraESphereMissile;
			SyndraESphereMissile.name = "SyndraESphereMissile";
			SyndraESphereMissile.icon = "SyndraQ";
			SyndraESphereMissile.displayName = "Scatter the Weak [Sphere]";
			SyndraESphereMissile.missileName = "SyndraESphereMissile";
			SyndraESphereMissile.slot = SpellSlot::E;
			SyndraESphereMissile.type = SpellType::linear;
			SyndraESphereMissile.range = 1250;
			SyndraESphereMissile.radius = 100;
			SyndraESphereMissile.delay = 0.25;
			SyndraESphereMissile.danger = 3;
			SyndraESphereMissile.speed = 2000;
			SyndraESphereMissile.cc = true;
			SyndraESphereMissile.collisionMinions = false;
			SyndraESphereMissile.windwall = true;
			SyndraESphereMissile.hitbox = false;
			SyndraESphereMissile.fow = true;
			SyndraESphereMissile.exception = true;
			SyndraESphereMissile.extend = false;
			Syndra.Spells.push_back(SyndraESphereMissile);
			Core::SpellDB.emplace_back(Syndra);
		}
		{
			Champ TahmKench;
			TahmKench.Name = "TahmKench";
			Spell TahmKenchQ;
			TahmKenchQ.name = "TahmKenchQ";
			TahmKenchQ.icon = "TahmKenchQ";
			TahmKenchQ.displayName = "Tongue Lash";
			TahmKenchQ.missileName = "TahmKenchQMissile";
			TahmKenchQ.slot = SpellSlot::Q;
			TahmKenchQ.type = SpellType::linear;
			TahmKenchQ.range = 900;
			TahmKenchQ.radius = 70;
			TahmKenchQ.delay = 0.25;
			TahmKenchQ.danger = 2;
			TahmKenchQ.speed = 2800;
			TahmKenchQ.cc = true;
			TahmKenchQ.collisionMinions = true;
			TahmKenchQ.windwall = true;
			TahmKenchQ.hitbox = false;
			TahmKenchQ.fow = true;
			TahmKenchQ.exception = false;
			TahmKenchQ.extend = true;
			TahmKench.Spells.push_back(TahmKenchQ);
			Core::SpellDB.emplace_back(TahmKench);
		}
		{
			Champ Taliyah;
			Taliyah.Name = "Taliyah";
			Spell TaliyahQMis;
			TaliyahQMis.name = "TaliyahQMis";
			TaliyahQMis.icon = "TaliyahQ";
			TaliyahQMis.displayName = "Threaded Volley";
			TaliyahQMis.missileName = "TaliyahQMis";
			TaliyahQMis.slot = SpellSlot::Q;
			TaliyahQMis.type = SpellType::linear;
			TaliyahQMis.range = 1000;
			TaliyahQMis.radius = 100;
			TaliyahQMis.delay = 0;
			TaliyahQMis.danger = 2;
			TaliyahQMis.speed = 3600;
			TaliyahQMis.cc = false;
			TaliyahQMis.collisionMinions = true;
			TaliyahQMis.windwall = true;
			TaliyahQMis.hitbox = false;
			TaliyahQMis.fow = true;
			TaliyahQMis.exception = true;
			TaliyahQMis.extend = true;
			Taliyah.Spells.push_back(TaliyahQMis);
			Spell TaliyahWVC;
			TaliyahWVC.name = "TaliyahWVC";
			TaliyahWVC.icon = "TaliyahW";
			TaliyahWVC.displayName = "Seismic Shove";
			TaliyahWVC.missileName = "";
			TaliyahWVC.slot = SpellSlot::W;
			TaliyahWVC.type = SpellType::circular;
			TaliyahWVC.range = 900;
			TaliyahWVC.radius = 150;
			TaliyahWVC.delay = 0.85;
			TaliyahWVC.danger = 1;
			TaliyahWVC.speed = MathHuge;
			TaliyahWVC.cc = true;
			TaliyahWVC.collisionMinions = false;
			TaliyahWVC.windwall = false;
			TaliyahWVC.hitbox = false;
			TaliyahWVC.fow = false;
			TaliyahWVC.exception = false;
			TaliyahWVC.extend = false;
			Taliyah.Spells.push_back(TaliyahWVC);
			Spell TaliyahE;
			TaliyahE.name = "TaliyahE";
			TaliyahE.icon = "TaliyahE";
			TaliyahE.displayName = "Unraveled Earth";
			TaliyahE.missileName = "";
			TaliyahE.slot = SpellSlot::E;
			TaliyahE.type = SpellType::conic;
			TaliyahE.range = 800;
			TaliyahE.radius = 0;
			TaliyahE.delay = 0.45;
			TaliyahE.danger = 2;
			TaliyahE.speed = 2000;
			TaliyahE.cc = true;
			TaliyahE.collisionMinions = false;
			TaliyahE.windwall = false;
			TaliyahE.hitbox = false;
			TaliyahE.fow = false;
			TaliyahE.exception = false;
			TaliyahE.extend = true;
			Taliyah.Spells.push_back(TaliyahE);
			Spell TaliyahR;
			TaliyahR.name = "TaliyahR";
			TaliyahR.icon = "TaliyahR";
			TaliyahR.displayName = "Weaver's Wall";
			TaliyahR.missileName = "TaliyahRMis";
			TaliyahR.slot = SpellSlot::R;
			TaliyahR.type = SpellType::linear;
			TaliyahR.range = 3000;
			TaliyahR.radius = 120;
			TaliyahR.delay = 1;
			TaliyahR.danger = 1;
			TaliyahR.speed = 1700;
			TaliyahR.cc = true;
			TaliyahR.collisionMinions = false;
			TaliyahR.windwall = false;
			TaliyahR.hitbox = false;
			TaliyahR.fow = true;
			TaliyahR.exception = false;
			TaliyahR.extend = true;
			Taliyah.Spells.push_back(TaliyahR);
			Core::SpellDB.emplace_back(Taliyah);
		}
		{
			Champ Talon;
			Talon.Name = "Talon";
			Spell TalonW;
			TalonW.name = "TalonW";
			TalonW.icon = "TalonW";
			TalonW.displayName = "Rake";
			TalonW.missileName = "TalonWMissileOne";
			TalonW.slot = SpellSlot::W;
			TalonW.type = SpellType::conic;
			TalonW.range = 650;
			TalonW.radius = 75;
			TalonW.delay = 0.25;
			TalonW.danger = 2;
			TalonW.speed = 2500;
			TalonW.cc = true;
			TalonW.collisionMinions = false;
			TalonW.windwall = true;
			TalonW.hitbox = false;
			TalonW.fow = true;
			TalonW.exception = false;
			TalonW.extend = true;
			Talon.Spells.push_back(TalonW);
			Core::SpellDB.emplace_back(Talon);
		}
		{
			Champ Thresh;
			Thresh.Name = "Thresh";
			Spell ThreshQ;
			ThreshQ.name = "ThreshQInternal";
			ThreshQ.icon = "ThreshQ";
			ThreshQ.displayName = "Death Sentence";
			ThreshQ.missileName = "ThreshQMissile";
			ThreshQ.slot = SpellSlot::Q;
			ThreshQ.type = SpellType::linear;
			ThreshQ.range = 1100;
			ThreshQ.radius = 70;
			ThreshQ.delay = 0.5;
			ThreshQ.danger = 1;
			ThreshQ.speed = 1900;
			ThreshQ.cc = true;
			ThreshQ.collisionMinions = true;
			ThreshQ.windwall = true;
			ThreshQ.hitbox = false;
			ThreshQ.fow = true;
			ThreshQ.exception = true;
			ThreshQ.extend = true;
			Thresh.Spells.push_back(ThreshQ);
			Spell ThreshEFlay;
			ThreshEFlay.name = "ThreshEFlay";
			ThreshEFlay.icon = "ThreshE";
			ThreshEFlay.displayName = "Flay";
			ThreshEFlay.missileName = "";
			ThreshEFlay.slot = SpellSlot::E;
			ThreshEFlay.type = SpellType::polygon;
			ThreshEFlay.range = 500;
			ThreshEFlay.radius = 110;
			ThreshEFlay.delay = 0.389;
			ThreshEFlay.danger = 3;
			ThreshEFlay.speed = MathHuge;
			ThreshEFlay.cc = true;
			ThreshEFlay.collisionMinions = true;
			ThreshEFlay.windwall = false;
			ThreshEFlay.hitbox = false;
			ThreshEFlay.fow = false;
			ThreshEFlay.exception = false;
			ThreshEFlay.extend = true;
			Thresh.Spells.push_back(ThreshEFlay);
			Core::SpellDB.emplace_back(Thresh);
		}
		{
			Champ Tristana;
			Tristana.Name = "Tristana";
			Spell TristanaW;
			TristanaW.name = "TristanaW";
			TristanaW.icon = "TristanaW";
			TristanaW.displayName = "Rocket Jump";
			TristanaW.missileName = "";
			TristanaW.slot = SpellSlot::W;
			TristanaW.type = SpellType::circular;
			TristanaW.range = 900;
			TristanaW.radius = 300;
			TristanaW.delay = 0.25;
			TristanaW.danger = 2;
			TristanaW.speed = 1100;
			TristanaW.cc = true;
			TristanaW.collisionMinions = false;
			TristanaW.windwall = false;
			TristanaW.hitbox = false;
			TristanaW.fow = false;
			TristanaW.exception = false;
			TristanaW.extend = false;
			Tristana.Spells.push_back(TristanaW);
			Core::SpellDB.emplace_back(Tristana);
		}
		{
			Champ Tryndamere;
			Tryndamere.Name = "Tryndamere";
			Spell TryndamereE;
			TryndamereE.name = "TryndamereE";
			TryndamereE.icon = "TryndamereE";
			TryndamereE.displayName = "Spinning Slash";
			TryndamereE.missileName = "";
			TryndamereE.slot = SpellSlot::E;
			TryndamereE.type = SpellType::linear;
			TryndamereE.range = 660;
			TryndamereE.radius = 225;
			TryndamereE.delay = 0;
			TryndamereE.danger = 2;
			TryndamereE.speed = 1300;
			TryndamereE.cc = false;
			TryndamereE.collisionMinions = false;
			TryndamereE.windwall = false;
			TryndamereE.hitbox = false;
			TryndamereE.fow = false;
			TryndamereE.exception = false;
			TryndamereE.extend = true;
			Tryndamere.Spells.push_back(TryndamereE);
			Core::SpellDB.emplace_back(Tryndamere);
		}
		{
			Champ TwistedFate;
			TwistedFate.Name = "TwistedFate";
			Spell WildCards;
			WildCards.name = "WildCards";
			WildCards.icon = "TwistedFateQ";
			WildCards.displayName = "Wild Cards";
			WildCards.missileName = "SealFateMissile";
			WildCards.slot = SpellSlot::Q;
			WildCards.type = SpellType::threeway;
			WildCards.range = 1450;
			WildCards.radius = 40;
			WildCards.delay = 0.25;
			WildCards.danger = 1;
			WildCards.speed = 1000;
			WildCards.cc = false;
			WildCards.collisionMinions = false;
			WildCards.windwall = true;
			WildCards.hitbox = false;
			WildCards.fow = true;
			WildCards.exception = false;
			WildCards.extend = true;
			TwistedFate.Spells.push_back(WildCards);
			Core::SpellDB.emplace_back(TwistedFate);
		}
		{
			Champ Urgot;
			Urgot.Name = "Urgot";
			Spell UrgotQ;
			UrgotQ.name = "UrgotQ";
			UrgotQ.icon = "UrgotQ";
			UrgotQ.displayName = "Corrosive Charge";
			UrgotQ.missileName = "UrgotQMissile";
			UrgotQ.slot = SpellSlot::Q;
			UrgotQ.type = SpellType::circular;
			UrgotQ.range = 800;
			UrgotQ.radius = 180;
			UrgotQ.delay = 0.6;
			UrgotQ.danger = 2;
			UrgotQ.speed = MathHuge;
			UrgotQ.cc = true;
			UrgotQ.collisionMinions = false;
			UrgotQ.windwall = true;
			UrgotQ.hitbox = false;
			UrgotQ.fow = true;
			UrgotQ.exception = false;
			UrgotQ.extend = false;
			Urgot.Spells.push_back(UrgotQ);
			Spell UrgotE;
			UrgotE.name = "UrgotE";
			UrgotE.icon = "UrgotE";
			UrgotE.displayName = "Disdain";
			UrgotE.missileName = "";
			UrgotE.slot = SpellSlot::E;
			UrgotE.type = SpellType::linear;
			UrgotE.range = 475;
			UrgotE.radius = 100;
			UrgotE.delay = 0.45;
			UrgotE.danger = 2;
			UrgotE.speed = 1540;
			UrgotE.cc = true;
			UrgotE.collisionMinions = false;
			UrgotE.windwall = false;
			UrgotE.hitbox = false;
			UrgotE.fow = false;
			UrgotE.exception = false;
			UrgotE.extend = true;
			Urgot.Spells.push_back(UrgotE);
			Spell UrgotR;
			UrgotR.name = "UrgotR";
			UrgotR.icon = "UrgotR";
			UrgotR.displayName = "Fear Beyond Death";
			UrgotR.missileName = "UrgotR";
			UrgotR.slot = SpellSlot::R;
			UrgotR.type = SpellType::linear;
			UrgotR.range = 1600;
			UrgotR.radius = 80;
			UrgotR.delay = 0.5;
			UrgotR.danger = 4;
			UrgotR.speed = 3200;
			UrgotR.cc = true;
			UrgotR.collisionMinions = false;
			UrgotR.windwall = true;
			UrgotR.hitbox = false;
			UrgotR.fow = true;
			UrgotR.exception = false;
			UrgotR.extend = true;
			Urgot.Spells.push_back(UrgotR);
			Core::SpellDB.emplace_back(Urgot);
		}
		{
			Champ Varus;
			Varus.Name = "Varus";
			Spell VarusQMissile;
			VarusQMissile.name = "VarusQMissile";
			VarusQMissile.icon = "VarusQ";
			VarusQMissile.displayName = "Piercing Arrow";
			VarusQMissile.missileName = "VarusQMissile";
			VarusQMissile.slot = SpellSlot::Q;
			VarusQMissile.type = SpellType::linear;
			VarusQMissile.range = 1525;
			VarusQMissile.radius = 70;
			VarusQMissile.delay = 0;
			VarusQMissile.danger = 1;
			VarusQMissile.speed = 1900;
			VarusQMissile.cc = false;
			VarusQMissile.collisionMinions = false;
			VarusQMissile.windwall = true;
			VarusQMissile.hitbox = false;
			VarusQMissile.fow = true;
			VarusQMissile.exception = true;
			VarusQMissile.extend = true;
			Varus.Spells.push_back(VarusQMissile);
			Spell VarusE;
			VarusE.name = "VarusE";
			VarusE.icon = "VarusE";
			VarusE.displayName = "Hail of Arrows";
			VarusE.missileName = "VarusEMissile";
			VarusE.slot = SpellSlot::E;
			VarusE.type = SpellType::circular;
			VarusE.range = 925;
			VarusE.radius = 260;
			VarusE.delay = 0.242;
			VarusE.danger = 3;
			VarusE.speed = 1500;
			VarusE.cc = true;
			VarusE.collisionMinions = false;
			VarusE.windwall = true;
			VarusE.hitbox = false;
			VarusE.fow = true;
			VarusE.exception = false;
			VarusE.extend = true;
			Varus.Spells.push_back(VarusE);
			Spell VarusR;
			VarusR.name = "VarusR";
			VarusR.icon = "VarusR";
			VarusR.displayName = "Chain of Corruption";
			VarusR.missileName = "VarusRMissile";
			VarusR.slot = SpellSlot::R;
			VarusR.type = SpellType::linear;
			VarusR.range = 1200;
			VarusR.radius = 120;
			VarusR.delay = 0.25;
			VarusR.danger = 4;
			VarusR.speed = 1500;
			VarusR.cc = true;
			VarusR.collisionMinions = false;
			VarusR.windwall = true;
			VarusR.hitbox = false;
			VarusR.fow = true;
			VarusR.exception = false;
			VarusR.extend = true;
			Varus.Spells.push_back(VarusR);
			Core::SpellDB.emplace_back(Varus);
		}
		{
			Champ Veigar;
			Veigar.Name = "Veigar";
			Spell VeigarBalefulStrike;
			VeigarBalefulStrike.name = "VeigarBalefulStrike";
			VeigarBalefulStrike.icon = "VeigarQ";
			VeigarBalefulStrike.displayName = "Baleful Strike";
			VeigarBalefulStrike.missileName = "VeigarBalefulStrikeMis";
			VeigarBalefulStrike.slot = SpellSlot::Q;
			VeigarBalefulStrike.type = SpellType::linear;
			VeigarBalefulStrike.range = 900;
			VeigarBalefulStrike.radius = 70;
			VeigarBalefulStrike.delay = 0.25;
			VeigarBalefulStrike.danger = 2;
			VeigarBalefulStrike.speed = 2200;
			VeigarBalefulStrike.cc = false;
			VeigarBalefulStrike.collisionMinions = false;
			VeigarBalefulStrike.windwall = true;
			VeigarBalefulStrike.hitbox = false;
			VeigarBalefulStrike.fow = true;
			VeigarBalefulStrike.exception = false;
			VeigarBalefulStrike.extend = true;
			Veigar.Spells.push_back(VeigarBalefulStrike);
			Spell VeigarDarkMatter;
			VeigarDarkMatter.name = "VeigarDarkMatter";
			VeigarDarkMatter.icon = "VeigarW";
			VeigarDarkMatter.displayName = "Dark Matter";
			VeigarDarkMatter.missileName = "";
			VeigarDarkMatter.slot = SpellSlot::W;
			VeigarDarkMatter.type = SpellType::circular;
			VeigarDarkMatter.range = 900;
			VeigarDarkMatter.radius = 200;
			VeigarDarkMatter.delay = 1.25;
			VeigarDarkMatter.danger = 1;
			VeigarDarkMatter.speed = MathHuge;
			VeigarDarkMatter.cc = false;
			VeigarDarkMatter.collisionMinions = false;
			VeigarDarkMatter.windwall = false;
			VeigarDarkMatter.hitbox = false;
			VeigarDarkMatter.fow = false;
			VeigarDarkMatter.exception = false;
			VeigarDarkMatter.extend = false;
			Veigar.Spells.push_back(VeigarDarkMatter);
			Core::SpellDB.emplace_back(Veigar);
		}
		{
			Champ Velkoz;
			Velkoz.Name = "Velkoz";
			Spell VelkozQMissileSplit;
			VelkozQMissileSplit.name = "VelkozQMissileSplit";
			VelkozQMissileSplit.icon = "VelkozQ2";
			VelkozQMissileSplit.displayName = "Plasma Fission [Split]";
			VelkozQMissileSplit.missileName = "VelkozQMissileSplit";
			VelkozQMissileSplit.slot = SpellSlot::Q;
			VelkozQMissileSplit.type = SpellType::linear;
			VelkozQMissileSplit.range = 1100;
			VelkozQMissileSplit.radius = 45;
			VelkozQMissileSplit.delay = 0;
			VelkozQMissileSplit.danger = 2;
			VelkozQMissileSplit.speed = 2100;
			VelkozQMissileSplit.cc = true;
			VelkozQMissileSplit.collisionMinions = true;
			VelkozQMissileSplit.windwall = true;
			VelkozQMissileSplit.hitbox = false;
			VelkozQMissileSplit.fow = true;
			VelkozQMissileSplit.exception = true;
			VelkozQMissileSplit.extend = false;
			Velkoz.Spells.push_back(VelkozQMissileSplit);
			Spell VelkozQ;
			VelkozQ.name = "VelkozQ";
			VelkozQ.icon = "VelkozQ";
			VelkozQ.displayName = "Plasma Fission";
			VelkozQ.missileName = "VelkozQMissile";
			VelkozQ.slot = SpellSlot::Q;
			VelkozQ.type = SpellType::linear;
			VelkozQ.range = 1050;
			VelkozQ.radius = 50;
			VelkozQ.delay = 0.25;
			VelkozQ.danger = 1;
			VelkozQ.speed = 1300;
			VelkozQ.cc = true;
			VelkozQ.collisionMinions = true;
			VelkozQ.windwall = true;
			VelkozQ.hitbox = false;
			VelkozQ.fow = true;
			VelkozQ.exception = false;
			VelkozQ.extend = true;
			Velkoz.Spells.push_back(VelkozQ);
			Spell VelkozW;
			VelkozW.name = "VelkozW";
			VelkozW.icon = "VelkozW";
			VelkozW.displayName = "Void Rift";
			VelkozW.missileName = "VelkozWMissile";
			VelkozW.slot = SpellSlot::W;
			VelkozW.type = SpellType::linear;
			VelkozW.range = 1050;
			VelkozW.radius = 87.5;
			VelkozW.delay = 0.25;
			VelkozW.danger = 1;
			VelkozW.speed = 1700;
			VelkozW.cc = false;
			VelkozW.collisionMinions = false;
			VelkozW.windwall = true;
			VelkozW.hitbox = false;
			VelkozW.fow = true;
			VelkozW.exception = false;
			VelkozW.extend = true;
			Velkoz.Spells.push_back(VelkozW);
			Spell VelkozE;
			VelkozE.name = "VelkozE";
			VelkozE.icon = "VelkozE";
			VelkozE.displayName = "Tectonic Disruption";
			VelkozE.missileName = "";
			VelkozE.slot = SpellSlot::E;
			VelkozE.type = SpellType::circular;
			VelkozE.range = 800;
			VelkozE.radius = 185;
			VelkozE.delay = 0.8;
			VelkozE.danger = 2;
			VelkozE.speed = MathHuge;
			VelkozE.cc = true;
			VelkozE.collisionMinions = false;
			VelkozE.windwall = false;
			VelkozE.hitbox = false;
			VelkozE.fow = false;
			VelkozE.exception = false;
			VelkozE.extend = false;
			Velkoz.Spells.push_back(VelkozE);
			Core::SpellDB.emplace_back(Velkoz);
		}
		{
			Champ Vi;
			Vi.Name = "Vi";
			Spell ViQ;
			ViQ.name = "ViQ";
			ViQ.icon = "ViQ";
			ViQ.displayName = "Vault Breaker";
			ViQ.missileName = "";
			ViQ.slot = SpellSlot::Q;
			ViQ.type = SpellType::linear;
			ViQ.range = 725;
			ViQ.radius = 90;
			ViQ.delay = 0;
			ViQ.danger = 2;
			ViQ.speed = 1500;
			ViQ.cc = true;
			ViQ.collisionMinions = false;
			ViQ.windwall = false;
			ViQ.hitbox = false;
			ViQ.fow = false;
			ViQ.exception = false;
			ViQ.extend = true;
			Vi.Spells.push_back(ViQ);
			Core::SpellDB.emplace_back(Vi);
		}
		{
			Champ Viego;
			Viego.Name = "Viego";

			Spell Q;
			Q.name = "ViegoQ";
			Q.icon = "ViegoQ";
			Q.displayName = "ViegoQ";
			Q.slot = SpellSlot::Q;
			Q.type = SpellType::linear;
			Q.speed = MathHuge;
			Q.range = 600;
			Q.delay = 0.4f;
			Q.radius = 70;
			Q.danger = 4;
			Q.cc = false;
			Q.collisionMinions = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Viego.Spells.emplace_back(Q);

			Spell ViegoW;
			ViegoW.name = "ViegoW1";
			ViegoW.icon = "ViegoW";
			ViegoW.displayName = "Spectral Maw";
			ViegoW.missileName = "ViegoWMis";
			ViegoW.slot = SpellSlot::W;
			ViegoW.type = SpellType::linear;
			ViegoW.range = 760;
			ViegoW.radius = 90;
			ViegoW.delay = 0;
			ViegoW.danger = 3;
			ViegoW.cc = true;
			ViegoW.speed = 1300;
			ViegoW.collisionMinions = true;
			ViegoW.collisionChamps = true;
			ViegoW.windwall = true;
			ViegoW.hitbox = false;
			ViegoW.fow = true;
			ViegoW.exception = false;
			ViegoW.extend = true;
			Viego.Spells.push_back(ViegoW);

			Spell R;
			R.name = "ViegoR";
			R.icon = "ViegoR";
			R.displayName = "ViegoR";
			R.slot = SpellSlot::R;
			R.type = SpellType::circular;
			R.speed = MathHuge;
			R.range = 500;
			R.delay = 0.6f;
			R.radius = 280;
			R.danger = 3;
			R.cc = false;
			R.collisionMinions = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;
			Viego.Spells.emplace_back(R);

			Core::SpellDB.emplace_back(Viego);
		}
		{
			Champ Viktor;
			Viktor.Name = "Viktor";
			Spell ViktorGravitonField;
			ViktorGravitonField.name = "ViktorGravitonField";
			ViktorGravitonField.icon = "ViktorW";
			ViktorGravitonField.displayName = "Graviton Field";
			ViktorGravitonField.missileName = "";
			ViktorGravitonField.slot = SpellSlot::W;
			ViktorGravitonField.type = SpellType::circular;
			ViktorGravitonField.range = 800;
			ViktorGravitonField.radius = 270;
			ViktorGravitonField.delay = 1.75;
			ViktorGravitonField.danger = 1;
			ViktorGravitonField.speed = MathHuge;
			ViktorGravitonField.cc = true;
			ViktorGravitonField.collisionMinions = false;
			ViktorGravitonField.windwall = false;
			ViktorGravitonField.hitbox = false;
			ViktorGravitonField.fow = false;
			ViktorGravitonField.exception = false;
			ViktorGravitonField.extend = false;
			Viktor.Spells.push_back(ViktorGravitonField);
			Spell ViktorDeathRayMissile;
			ViktorDeathRayMissile.name = "ViktorDeathRay";
			ViktorDeathRayMissile.icon = "ViktorE";
			ViktorDeathRayMissile.displayName = "Death Ray";
			ViktorDeathRayMissile.missileName = "ViktorDeathRayMissile";
			ViktorDeathRayMissile.slot = SpellSlot::E;
			ViktorDeathRayMissile.type = SpellType::linear;
			ViktorDeathRayMissile.range = 700;
			ViktorDeathRayMissile.radius = 80;
			ViktorDeathRayMissile.delay = 0;
			ViktorDeathRayMissile.danger = 2;
			ViktorDeathRayMissile.speed = 1050;
			ViktorDeathRayMissile.cc = false;
			ViktorDeathRayMissile.collisionMinions = false;
			ViktorDeathRayMissile.windwall = true;
			ViktorDeathRayMissile.hitbox = false;
			ViktorDeathRayMissile.fow = true;
			ViktorDeathRayMissile.exception = true;
			ViktorDeathRayMissile.extend = true;
			ViktorDeathRayMissile.useEndPos2 = true;
			Viktor.Spells.push_back(ViktorDeathRayMissile);
			Core::SpellDB.emplace_back(Viktor);
		}
		{
			Champ Warwick;
			Warwick.Name = "Warwick";
			Spell WarwickR;
			WarwickR.name = "WarwickR";
			WarwickR.icon = "WarwickR";
			WarwickR.displayName = "Infinite Duress";
			WarwickR.missileName = "";
			WarwickR.slot = SpellSlot::R;
			WarwickR.type = SpellType::linear;
			WarwickR.range = 3000;
			WarwickR.radius = 55;
			WarwickR.delay = 0.1;
			WarwickR.danger = 4;
			WarwickR.speed = 1800;
			WarwickR.cc = true;
			WarwickR.collisionMinions = false;
			WarwickR.windwall = false;
			WarwickR.hitbox = false;
			WarwickR.fow = false;
			WarwickR.exception = false;
			WarwickR.extend = true;
			Warwick.Spells.push_back(WarwickR);
			Core::SpellDB.emplace_back(Warwick);
		}
		{
			Champ Xayah;
			Xayah.Name = "Xayah";
			Spell XayahQ;
			XayahQ.name = "XayahQ";
			XayahQ.icon = "XayahQ";
			XayahQ.displayName = "Double Daggers";
			XayahQ.missileName = "XayahQ";
			XayahQ.slot = SpellSlot::Q;
			XayahQ.type = SpellType::linear;
			XayahQ.range = 1100;
			XayahQ.radius = 45;
			XayahQ.delay = 0.5;
			XayahQ.danger = 1;
			XayahQ.speed = 2075;
			XayahQ.cc = false;
			XayahQ.collisionMinions = false;
			XayahQ.windwall = true;
			XayahQ.hitbox = false;
			XayahQ.fow = true;
			XayahQ.exception = false;
			XayahQ.extend = true;
			Xayah.Spells.push_back(XayahQ);
			Core::SpellDB.emplace_back(Xayah);
		}
		{
			Champ Xerath;
			Xerath.Name = "Xerath";
			Spell XerathArcaneBarrage2;
			XerathArcaneBarrage2.name = "XerathArcaneBarrage2";
			XerathArcaneBarrage2.icon = "XerathW";
			XerathArcaneBarrage2.displayName = "Arcane Barrage";
			XerathArcaneBarrage2.missileName = "";
			XerathArcaneBarrage2.slot = SpellSlot::W;
			XerathArcaneBarrage2.type = SpellType::circular;
			XerathArcaneBarrage2.range = 1000;
			XerathArcaneBarrage2.radius = 235;
			XerathArcaneBarrage2.delay = 0.75;
			XerathArcaneBarrage2.danger = 3;
			XerathArcaneBarrage2.speed = MathHuge;
			XerathArcaneBarrage2.cc = true;
			XerathArcaneBarrage2.collisionMinions = false;
			XerathArcaneBarrage2.windwall = false;
			XerathArcaneBarrage2.hitbox = false;
			XerathArcaneBarrage2.fow = false;
			XerathArcaneBarrage2.exception = false;
			XerathArcaneBarrage2.extend = false;
			Xerath.Spells.push_back(XerathArcaneBarrage2);
			Spell XerathMageSpear;
			XerathMageSpear.name = "XerathMageSpear";
			XerathMageSpear.icon = "XerathE";
			XerathMageSpear.displayName = "Mage Spear";
			XerathMageSpear.missileName = "XerathMageSpearMissile";
			XerathMageSpear.slot = SpellSlot::E;
			XerathMageSpear.type = SpellType::linear;
			XerathMageSpear.range = 1050;
			XerathMageSpear.radius = 60;
			XerathMageSpear.delay = 0.2;
			XerathMageSpear.danger = 1;
			XerathMageSpear.speed = 1400;
			XerathMageSpear.cc = true;
			XerathMageSpear.collisionMinions = true;
			XerathMageSpear.windwall = true;
			XerathMageSpear.hitbox = false;
			XerathMageSpear.fow = true;
			XerathMageSpear.exception = false;
			XerathMageSpear.extend = true;
			Xerath.Spells.push_back(XerathMageSpear);
			Spell XerathLocusPulse;
			XerathLocusPulse.name = "XerathLocusPulse";
			XerathLocusPulse.icon = "XerathR";
			XerathLocusPulse.displayName = "Rite of the Arcane";
			XerathLocusPulse.missileName = "XerathLocusPulse";
			XerathLocusPulse.slot = SpellSlot::R;
			XerathLocusPulse.type = SpellType::circular;
			XerathLocusPulse.range = 5000;
			XerathLocusPulse.radius = 200;
			XerathLocusPulse.delay = 0.7;
			XerathLocusPulse.danger = 3;
			XerathLocusPulse.speed = MathHuge;
			XerathLocusPulse.cc = false;
			XerathLocusPulse.collisionMinions = false;
			XerathLocusPulse.windwall = false;
			XerathLocusPulse.hitbox = false;
			XerathLocusPulse.fow = true;
			XerathLocusPulse.exception = true;
			XerathLocusPulse.extend = false;
			Xerath.Spells.push_back(XerathLocusPulse);
			Core::SpellDB.emplace_back(Xerath);
		}
		{
			Champ XinZhao;
			XinZhao.Name = "XinZhao";
			Spell XinZhaoW;
			XinZhaoW.name = "XinZhaoW";
			XinZhaoW.icon = "XinZhaoW";
			XinZhaoW.displayName = "Wind Becomes Lightning";
			XinZhaoW.missileName = "";
			XinZhaoW.slot = SpellSlot::W;
			XinZhaoW.type = SpellType::linear;
			XinZhaoW.range = 900;
			XinZhaoW.radius = 40;
			XinZhaoW.delay = 0.5;
			XinZhaoW.danger = 1;
			XinZhaoW.speed = 5000;
			XinZhaoW.cc = true;
			XinZhaoW.collisionMinions = false;
			XinZhaoW.windwall = false;
			XinZhaoW.hitbox = false;
			XinZhaoW.fow = false;
			XinZhaoW.exception = false;
			XinZhaoW.extend = true;
			XinZhao.Spells.push_back(XinZhaoW);
			Core::SpellDB.emplace_back(XinZhao);
		}
		{
			Champ Yasuo;
			Yasuo.Name = "Yasuo";
			Spell YasuoQ1;
			YasuoQ1.name = "YasuoQ1";
			YasuoQ1.icon = "YasuoQ1";
			YasuoQ1.displayName = "Steel Tempest";
			YasuoQ1.missileName = "";
			YasuoQ1.slot = SpellSlot::Q;
			YasuoQ1.type = SpellType::linear;
			YasuoQ1.range = 475;
			YasuoQ1.radius = 40;
			YasuoQ1.delay = 0.25;
			YasuoQ1.danger = 1;
			YasuoQ1.speed = 1500;
			YasuoQ1.cc = false;
			YasuoQ1.collisionMinions = false;
			YasuoQ1.windwall = false;
			YasuoQ1.hitbox = true;
			YasuoQ1.fow = false;
			YasuoQ1.exception = false;
			YasuoQ1.extend = true;
			Yasuo.Spells.push_back(YasuoQ1);
			Spell YasuoQ2;
			YasuoQ2.name = "YasuoQ2";
			YasuoQ2.icon = "YasuoQ2";
			YasuoQ2.displayName = "Steel Wind Rising";
			YasuoQ2.missileName = "";
			YasuoQ2.slot = SpellSlot::Q;
			YasuoQ2.type = SpellType::linear;
			YasuoQ2.range = 475;
			YasuoQ2.radius = 40;
			YasuoQ2.delay = 0.25;
			YasuoQ2.danger = 1;
			YasuoQ2.speed = 1500;
			YasuoQ2.cc = false;
			YasuoQ2.collisionMinions = false;
			YasuoQ2.windwall = false;
			YasuoQ2.hitbox = true;
			YasuoQ2.fow = false;
			YasuoQ2.exception = false;
			YasuoQ2.extend = true;
			Yasuo.Spells.push_back(YasuoQ2);
			Spell YasuoQ3;
			YasuoQ3.name = "YasuoQ3";
			YasuoQ3.icon = "YasuoQ3";
			YasuoQ3.displayName = "Gathering Storm";
			YasuoQ3.missileName = "YasuoQ3Mis";
			YasuoQ3.slot = SpellSlot::Q;
			YasuoQ3.type = SpellType::linear;
			YasuoQ3.range = 1100;
			YasuoQ3.radius = 90;
			YasuoQ3.delay = 0.03;
			YasuoQ3.danger = 2;
			YasuoQ3.speed = 1200;
			YasuoQ3.cc = true;
			YasuoQ3.collisionMinions = false;
			YasuoQ3.windwall = true;
			YasuoQ3.hitbox = true;
			YasuoQ3.fow = true;
			YasuoQ3.exception = false;
			YasuoQ3.extend = true;
			Yasuo.Spells.push_back(YasuoQ3);
			Core::SpellDB.emplace_back(Yasuo);
		}
		{
			Champ Yone;
			Yone.Name = "Yone";
			Spell YoneQ;
			YoneQ.name = "YoneQ";
			YoneQ.icon = "YoneQ";
			YoneQ.displayName = "Mortal Steel [Sword]";
			YoneQ.missileName = "";
			YoneQ.slot = SpellSlot::Q;
			YoneQ.type = SpellType::linear;
			YoneQ.range = 450;
			YoneQ.radius = 40;
			YoneQ.delay = 0.25;
			YoneQ.danger = 1;
			YoneQ.speed = MathHuge;
			YoneQ.cc = false;
			YoneQ.collisionMinions = false;
			YoneQ.windwall = false;
			YoneQ.hitbox = true;
			YoneQ.fow = false;
			YoneQ.exception = false;
			YoneQ.extend = true;
			Yone.Spells.push_back(YoneQ);
			Spell YoneQ3;
			YoneQ3.name = "YoneQ3";
			YoneQ3.icon = "YoneQ3";
			YoneQ3.displayName = "Mortal Steel [Storm]";
			YoneQ3.missileName = "YoneQ3Missile";
			YoneQ3.slot = SpellSlot::Q;
			YoneQ3.type = SpellType::linear;
			YoneQ3.range = 1050;
			YoneQ3.radius = 80;
			YoneQ3.delay = 0.25;
			YoneQ3.danger = 2;
			YoneQ3.speed = 1500;
			YoneQ3.cc = true;
			YoneQ3.collisionMinions = false;
			YoneQ3.windwall = true;
			YoneQ3.hitbox = true;
			YoneQ3.fow = true;
			YoneQ3.exception = false;
			YoneQ3.extend = true;
			Yone.Spells.push_back(YoneQ3);
			Spell YoneW;
			YoneW.name = "YoneW";
			YoneW.icon = "YoneW";
			YoneW.displayName = "Spirit Cleave";
			YoneW.missileName = "";
			YoneW.slot = SpellSlot::W;
			YoneW.type = SpellType::conic;
			YoneW.range = 600;
			YoneW.radius = 0;
			YoneW.delay = 0.375;
			YoneW.danger = 1;
			YoneW.speed = MathHuge;
			YoneW.cc = false;
			YoneW.collisionMinions = false;
			YoneW.windwall = false;
			YoneW.hitbox = false;
			YoneW.fow = false;
			YoneW.exception = false;
			YoneW.extend = true;
			Yone.Spells.push_back(YoneW);
			Spell YoneR;
			YoneR.name = "YoneR";
			YoneR.icon = "YoneR";
			YoneR.displayName = "Fate Sealed";
			YoneR.missileName = "";
			YoneR.slot = SpellSlot::R;
			YoneR.type = SpellType::linear;
			YoneR.range = 1000;
			YoneR.radius = 112.5;
			YoneR.delay = 0.75;
			YoneR.danger = 5;
			YoneR.speed = MathHuge;
			YoneR.cc = true;
			YoneR.collisionMinions = false;
			YoneR.windwall = false;
			YoneR.hitbox = true;
			YoneR.fow = false;
			YoneR.exception = false;
			YoneR.extend = true;
			Yone.Spells.push_back(YoneR);
			Core::SpellDB.emplace_back(Yone);
		}
		{
			Champ Zac;
			Zac.Name = "Zac";
			Spell ZacQ;
			ZacQ.name = "ZacQ";
			ZacQ.icon = "ZacQ";
			ZacQ.displayName = "Stretching Strikes";
			ZacQ.missileName = "ZacQMissile";
			ZacQ.slot = SpellSlot::Q;
			ZacQ.type = SpellType::linear;
			ZacQ.range = 800;
			ZacQ.radius = 120;
			ZacQ.delay = 0.33;
			ZacQ.danger = 2;
			ZacQ.speed = 2800;
			ZacQ.cc = true;
			ZacQ.collisionMinions = false;
			ZacQ.windwall = true;
			ZacQ.hitbox = false;
			ZacQ.fow = true;
			ZacQ.exception = false;
			ZacQ.extend = true;
			Zac.Spells.push_back(ZacQ);
			Core::SpellDB.emplace_back(Zac);
		}
		{
			Champ Zed;
			Zed.Name = "Zed";
			Spell ZedQ;
			ZedQ.name = "ZedQ";
			ZedQ.icon = "ZedQ";
			ZedQ.displayName = "Razor Shuriken";
			ZedQ.missileName = "ZedQMissile";
			ZedQ.slot = SpellSlot::Q;
			ZedQ.type = SpellType::linear;
			ZedQ.range = 900;
			ZedQ.radius = 50;
			ZedQ.delay = 0.25;
			ZedQ.danger = 1;
			ZedQ.speed = 1700;
			ZedQ.cc = false;
			ZedQ.collisionMinions = false;
			ZedQ.windwall = true;
			ZedQ.hitbox = true;
			ZedQ.fow = true;
			ZedQ.exception = true;
			ZedQ.extend = true;
			Zed.Spells.push_back(ZedQ);
			Core::SpellDB.emplace_back(Zed);
		}
		{
			Champ Zeri;
			Zeri.Name = "Zeri";
			Spell ZeriQ;
			ZeriQ.name = "ZeriQ";
			ZeriQ.icon = "ZeriQ";
			ZeriQ.displayName = "Burst Fire";
			ZeriQ.missileName = "ZeriQMissile";
			ZeriQ.slot = SpellSlot::Q;
			ZeriQ.type = SpellType::linear;
			ZeriQ.range = 840;
			ZeriQ.radius = 80;
			ZeriQ.delay = 0.25;
			ZeriQ.danger = 2;
			ZeriQ.speed = 1500;
			ZeriQ.cc = false;
			ZeriQ.collisionMinions = true;
			ZeriQ.windwall = true;
			ZeriQ.hitbox = true;
			ZeriQ.fow = true;
			ZeriQ.exception = true;
			ZeriQ.extend = true;
			Zeri.Spells.push_back(ZeriQ);
			Core::SpellDB.emplace_back(Zeri);
		}
		{
			Champ Ziggs;
			Ziggs.Name = "Ziggs";
			Spell ZiggsQ;
			ZiggsQ.name = "ZiggsQ";
			ZiggsQ.icon = "ZiggsQ";
			ZiggsQ.displayName = "Bouncing Bomb";
			ZiggsQ.missileName = "ZiggsQSpell";
			ZiggsQ.slot = SpellSlot::Q;
			ZiggsQ.type = SpellType::polygon;
			ZiggsQ.range = 850;
			ZiggsQ.radius = 150;
			ZiggsQ.delay = 0.25;
			ZiggsQ.danger = 1;
			ZiggsQ.speed = 1750;
			ZiggsQ.cc = false;
			ZiggsQ.collisionMinions = true;
			ZiggsQ.windwall = true;
			ZiggsQ.hitbox = false;
			ZiggsQ.fow = true;
			ZiggsQ.exception = false;
			ZiggsQ.extend = false;
			Ziggs.Spells.push_back(ZiggsQ);
			Spell ZiggsW;
			ZiggsW.name = "ZiggsW";
			ZiggsW.icon = "ZiggsW";
			ZiggsW.displayName = "Satchel Charge";
			ZiggsW.missileName = "ZiggsW";
			ZiggsW.slot = SpellSlot::W;
			ZiggsW.type = SpellType::circular;
			ZiggsW.range = 1000;
			ZiggsW.radius = 240;
			ZiggsW.delay = 0.25;
			ZiggsW.danger = 2;
			ZiggsW.speed = 1750;
			ZiggsW.cc = true;
			ZiggsW.collisionMinions = false;
			ZiggsW.windwall = true;
			ZiggsW.hitbox = false;
			ZiggsW.fow = true;
			ZiggsW.exception = false;
			ZiggsW.extend = false;
			Ziggs.Spells.push_back(ZiggsW);
			Spell ZiggsE;
			ZiggsE.name = "ZiggsE";
			ZiggsE.icon = "ZiggsE";
			ZiggsE.displayName = "Hexplosive Minefield";
			ZiggsE.missileName = "ZiggsE";
			ZiggsE.slot = SpellSlot::E;
			ZiggsE.type = SpellType::circular;
			ZiggsE.range = 900;
			ZiggsE.radius = 250;
			ZiggsE.delay = 0.25;
			ZiggsE.danger = 2;
			ZiggsE.speed = 1800;
			ZiggsE.cc = true;
			ZiggsE.collisionMinions = false;
			ZiggsE.windwall = true;
			ZiggsE.hitbox = false;
			ZiggsE.fow = true;
			ZiggsE.exception = false;
			ZiggsE.extend = false;
			Ziggs.Spells.push_back(ZiggsE);
			Spell ZiggsR;
			ZiggsR.name = "ZiggsR";
			ZiggsR.icon = "ZiggsR";
			ZiggsR.displayName = "Mega Inferno Bomb";
			ZiggsR.missileName = "ZiggsRBoom";
			ZiggsR.slot = SpellSlot::R;
			ZiggsR.type = SpellType::circular;
			ZiggsR.range = 5000;
			ZiggsR.radius = 480;
			ZiggsR.delay = 0.375;
			ZiggsR.danger = 4;
			ZiggsR.speed = 1550;
			ZiggsR.cc = false;
			ZiggsR.collisionMinions = false;
			ZiggsR.windwall = false;
			ZiggsR.hitbox = false;
			ZiggsR.fow = true;
			ZiggsR.exception = false;
			ZiggsR.extend = false;
			Ziggs.Spells.push_back(ZiggsR);
			Core::SpellDB.emplace_back(Ziggs);
		}
		{
			Champ Zilean;
			Zilean.Name = "Zilean";
			Spell ZileanQ;
			ZileanQ.name = "ZileanQ";
			ZileanQ.icon = "ZileanQ";
			ZileanQ.displayName = "Time Bomb";
			ZileanQ.missileName = "ZileanQMissile";
			ZileanQ.slot = SpellSlot::Q;
			ZileanQ.type = SpellType::circular;
			ZileanQ.range = 900;
			ZileanQ.radius = 150;
			ZileanQ.delay = 0.8;
			ZileanQ.danger = 2;
			ZileanQ.speed = MathHuge;
			ZileanQ.cc = true;
			ZileanQ.collisionMinions = false;
			ZileanQ.windwall = true;
			ZileanQ.hitbox = false;
			ZileanQ.fow = true;
			ZileanQ.exception = false;
			ZileanQ.extend = false;
			Zilean.Spells.push_back(ZileanQ);
			Core::SpellDB.emplace_back(Zilean);
		}
		{
			Champ Zoe;
			Zoe.Name = "Zoe";
			Spell ZoeQMissile;
			ZoeQMissile.name = "ZoeQMissile";
			ZoeQMissile.icon = "ZoeQ1";
			ZoeQMissile.displayName = "Paddle Star [First]";
			ZoeQMissile.missileName = "ZoeQMissile";
			ZoeQMissile.slot = SpellSlot::Q;
			ZoeQMissile.type = SpellType::linear;
			ZoeQMissile.range = 800;
			ZoeQMissile.radius = 50;
			ZoeQMissile.delay = 0.25;
			ZoeQMissile.danger = 1;
			ZoeQMissile.speed = 1200;
			ZoeQMissile.cc = false;
			ZoeQMissile.collisionMinions = true;
			ZoeQMissile.windwall = true;
			ZoeQMissile.hitbox = false;
			ZoeQMissile.fow = true;
			ZoeQMissile.exception = true;
			ZoeQMissile.extend = true;
			Zoe.Spells.push_back(ZoeQMissile);
			Spell ZoeQMis2;
			ZoeQMis2.name = "ZoeQMis2";
			ZoeQMis2.icon = "ZoeQ2";
			ZoeQMis2.displayName = "Paddle Star [Second]";
			ZoeQMis2.missileName = "ZoeQMis2";
			ZoeQMis2.slot = SpellSlot::Q;
			ZoeQMis2.type = SpellType::linear;
			ZoeQMis2.range = 1600;
			ZoeQMis2.radius = 70;
			ZoeQMis2.delay = 0;
			ZoeQMis2.danger = 2;
			ZoeQMis2.speed = 2500;
			ZoeQMis2.cc = false;
			ZoeQMis2.collisionMinions = true;
			ZoeQMis2.windwall = true;
			ZoeQMis2.hitbox = false;
			ZoeQMis2.fow = true;
			ZoeQMis2.exception = true;
			ZoeQMis2.extend = true;
			Zoe.Spells.push_back(ZoeQMis2);
			Spell ZoeE;
			ZoeE.name = "ZoeE";
			ZoeE.icon = "ZoeE";
			ZoeE.displayName = "Sleepy Trouble Bubble";
			ZoeE.missileName = "ZoeEMis";
			ZoeE.slot = SpellSlot::E;
			ZoeE.type = SpellType::linear;
			ZoeE.range = 800;
			ZoeE.radius = 50;
			ZoeE.delay = 0.3;
			ZoeE.danger = 2;
			ZoeE.speed = 1700;
			ZoeE.cc = true;
			ZoeE.collisionMinions = true;
			ZoeE.windwall = true;
			ZoeE.hitbox = false;
			ZoeE.fow = true;
			ZoeE.exception = false;
			ZoeE.extend = true;
			Zoe.Spells.push_back(ZoeE);
			Core::SpellDB.emplace_back(Zoe);
		}
		{
			Champ Zyra;
			Zyra.Name = "Zyra";
			Spell ZyraQ;
			ZyraQ.name = "ZyraQ";
			ZyraQ.icon = "ZyraQ";
			ZyraQ.displayName = "Deadly Spines";
			ZyraQ.missileName = "";
			ZyraQ.slot = SpellSlot::Q;
			ZyraQ.type = SpellType::rectangular;
			ZyraQ.range = 800;
			ZyraQ.radius = 200;
			ZyraQ.delay = 0.825;
			ZyraQ.danger = 1;
			ZyraQ.speed = MathHuge;
			ZyraQ.cc = false;
			ZyraQ.collisionMinions = false;
			ZyraQ.windwall = false;
			ZyraQ.hitbox = false;
			ZyraQ.fow = false;
			ZyraQ.exception = false;
			ZyraQ.extend = false;
			Zyra.Spells.push_back(ZyraQ);
			Spell ZyraE;
			ZyraE.name = "ZyraE";
			ZyraE.icon = "ZyraE";
			ZyraE.displayName = "Grasping Roots";
			ZyraE.missileName = "ZyraE";
			ZyraE.slot = SpellSlot::E;
			ZyraE.type = SpellType::linear;
			ZyraE.range = 1100;
			ZyraE.radius = 70;
			ZyraE.delay = 0.25;
			ZyraE.danger = 1;
			ZyraE.speed = 1150;
			ZyraE.cc = true;
			ZyraE.collisionMinions = false;
			ZyraE.windwall = true;
			ZyraE.hitbox = false;
			ZyraE.fow = true;
			ZyraE.exception = false;
			ZyraE.extend = true;
			Zyra.Spells.push_back(ZyraE);
			Spell ZyraR;
			ZyraR.name = "ZyraR";
			ZyraR.icon = "ZyraR";
			ZyraR.displayName = "Stranglethorns";
			ZyraR.missileName = "";
			ZyraR.slot = SpellSlot::R;
			ZyraR.type = SpellType::circular;
			ZyraR.range = 700;
			ZyraR.radius = 500;
			ZyraR.delay = 2;
			ZyraR.danger = 4;
			ZyraR.speed = MathHuge;
			ZyraR.cc = true;
			ZyraR.collisionMinions = false;
			ZyraR.windwall = false;
			ZyraR.hitbox = false;
			ZyraR.fow = false;
			ZyraR.exception = false;
			ZyraR.extend = false;
			Zyra.Spells.push_back(ZyraR);
			Core::SpellDB.emplace_back(Zyra);
		}
	}


}