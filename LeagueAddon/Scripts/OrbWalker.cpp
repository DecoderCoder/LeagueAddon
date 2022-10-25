#include "OrbWalker.h"
#include "../Functions.h"
#include "../Utils.h"
#include "../TargetSelector.h"
#include "../EventManager/EventManager.h"
#include "../Render.h"
#include "../Input.h"
#include "../Evade/Evade.h"

std::string OrbWalker::AttackResets[] =
{
	xorstr("dariusnoxiantacticsonh"), xorstr("fiorae"), xorstr("garenq"), xorstr("gravesmove"),
	xorstr("hecarimrapidslash"), xorstr("jaxempowertwo"), xorstr("jaycehypercharge"),
	xorstr("leonashieldofdaybreak"), xorstr("luciane"), xorstr("monkeykingdoubleattack"),
	xorstr("mordekaisermaceofspades"), xorstr("nasusq"), xorstr("nautiluspiercinggaze"),
	xorstr("netherblade"), xorstr("gangplankqwrapper"), xorstr("powerfist"),
	xorstr("renektonpreexecute"), xorstr("rengarq"), xorstr("shyvanadoubleattack"),
	xorstr("sivirw"), xorstr("takedown"), xorstr("talonnoxiandiplomacy"),
	xorstr("trundletrollsmash"), xorstr("vaynetumble"), xorstr("vie"), xorstr("volibearq"),
	xorstr("xenzhaocombotarget"), xorstr("yorickspectral"), xorstr("reksaiq"),
	xorstr("itemtitanichydracleave"), xorstr("masochism"), xorstr("illaoiw"),
	xorstr("elisespiderw"), xorstr("fiorae"), xorstr("meditate"), xorstr("sejuaninorthernwinds"),
	xorstr("asheq"), xorstr("camilleq"), xorstr("camilleq2")
};

bool OrbWalker::CanAttack() {
	//if (Reseted)
	//	return true;
	if (Local->GetChampionName() == "Jhin")
	{
		if (Local->BuffManager.hasBuff("JhinPassiveReload"))
		{
			return false;
		}
	}
	//if (dravenMode) {
	//	if (!Local->BuffManager.hasBuff("DravenSpinning"))
	//	{
	//		return false;
	//	}
	//}
	if (vayneQMode) {
		// vaynetumblebonus  vaynetumblefade
		if (Local->BuffManager.hasBuff("vaynetumblefade"))
		{
			return false;
		}
	}
	return Function::GameTimeTick() + Function::GetPing() / 2.f >= LastAttackCommandT + Function::GetAttackDelay(Local) * 1000.f;
}

bool OrbWalker::CanMove(float extraWindup) {
	return Function::GameTimeTick() + Function::GetPing() / 2 >= LastAttackCommandT + Function::GetAttackCastDelay(Local) * 1000.f + extraWindup || Local->GetChampionName() == "Kalista";
}

void OrbWalker::Orbwalk(GameObject* target, float extraWindup) {
	if (Function::GameTimeTick() - LastAttackCommandT < 70 + min(60, Function::GetPing()))
	{
		return;
	}

	if (Evade::Core::Evading && Evade::Core::EvadeSpells)
		return;
	if (target && CanAttack()) {
		//Vector2 w2s = riot_render->WorldToScreen(target->Position);
		if (useIssueOrder)
			if (Function::IsMinion(target))
				Function::IssueOrder(Local, 3, &target->Position, target, true, true, false);
			else {
				//int selectedBefore = Function::GetSelected();
				Function::IssueOrder(Local, 3, &target->Position, target, true, false, false);
				//Function::SetSelected(selectedBefore);
			}
		else {
			Vector3 w2s;
			Function::World2Screen(&target->Position, &w2s);
			Input::Move(w2s.x, w2s.y);
			Input::PressRightClick();
		}
		//Function::IssueOrder(EOrderType::attack, w2s.x, w2s.y);
		LastAttackCommandT = Function::GameTimeTick() - Function::GetPing() / 2;
	}
	if (CanMove(extraWindup) && LastMoveCommandT < Function::GameTimeTick())
	{
		//Vector2 w2s = riot_render->WorldToScreen(Functions::GetMouseWorldPosition());
		//Functions::IssueOrder(EOrderType::move, w2s.x, w2s.y);
		if (useIssueOrder)
			Function::IssueOrder(Local, 2, Function::GetMouseWorldPosition(), NULL, true, true, false);
		else
		{
			Input::PressRightClick();
		}
		LastMoveCommandT = Function::GameTimeTick() + 90;
	}
}

void OrbWalker::OnUpdate()
{
	if (!Helper::isValidUnit(_prevMinion))
		_prevMinion = nullptr;

	//if (Evade::Core::Evading || Evade::Core::IsDangerous(Evade::Core::PreviousPos))
	//{
	//	_mode = OrbwalkingMode::None;
	//	return;
	//}

	if (GetAsyncKeyState(VK_SPACE)) {
		_mode = OrbwalkingMode::Combo;
	}
	else if (GetAsyncKeyState(VK_CONTROL)) {
		_mode = OrbwalkingMode::LaneClear;
	}
	else if (GetAsyncKeyState(VK_LMENU)) {
		_mode = OrbwalkingMode::LastHit;
	}
	else
	{
		_mode = OrbwalkingMode::None;
	}

	if (!Function::IsAlive(Local))
		_mode = OrbwalkingMode::None;

	if (Function::IsChatOpen() || !IsLeagueInForeground())
		_mode = OrbwalkingMode::None;

	if (_mode == OrbwalkingMode::Combo && lockCamera)
		Function::LockCamera(true);

	if (_mode == OrbwalkingMode::None) {
		Function::LockCamera(false);
	}

	if (OrbWalker::useIssueOrder && GetAsyncKeyState(VK_SPACE) && (!GetAsyncKeyState(0x51) && !GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x45) && !GetAsyncKeyState(0x52) && !GetAsyncKeyState(0x44) && !GetAsyncKeyState(0x46)))
		Function::BlockInput(true);
	else
		Function::BlockInput(false);

	try
	{
		if (_mode == OrbwalkingMode::None)
		{
			return;
		}

		MinionListAA = filter(ObjectManager::MinionList(), [&](GameObject* minion)
			{
				if (!Helper::isValidUnit(minion) || !minion->IsInRange(Local, Local->AttackRange, true))
					return false;
				return true;
			});

		Orbwalk(GetTarget(), 90);
	}
	catch (...)
	{
		//console.Print("Orbwalking delayed");
	}
}

float OrbWalker::GetMyProjectileSpeed()
{
	//Utils::Log(to_hex((int)Function::GetBasicAttack(Local)));
	return Local->IsMelee() || Local->GetChampionName() == "Azir" || Local->GetChampionName() == "Thresh" || Local->GetChampionName() == "Velkoz" ||
		Local->GetChampionName() == "Viktor" && Local->BuffManager.hasBuff("ViktorPowerTransferReturn")
		? FLT_MAX
		: Function::GetBasicAttack(Local)->Resource->MissileSpeed;
}

bool OrbWalker::ShouldWait()
{
	int attackCalc = (int)(Function::GetAttackDelay(Local) * 1000 * 1.6) + Function::GetPing() / 2 + 1000 * 500 / (int)GetMyProjectileSpeed();
	for (auto minion : MinionListAA)
	{
		if (HealthPrediction::LaneClearHealthPrediction(minion, attackCalc, 0) <= Helper::CalcAutoAttackDamage(Local, minion))
			return true;
	}
	return false;
}

GameObject* OrbWalker::GetTarget()
{
	GameObject* result = nullptr;

	if (_mode == OrbwalkingMode::Combo || _mode == OrbwalkingMode::LaneClear)
	{
		if (selectedPriority) {
			if (int selectedId = Function::GetSelected()) {
				GameObject* object = ObjectManager::GetObjectByIndex(selectedId);
				if (object->IsEnemyTo(Local) && Helper::isValidUnit(object) && object->IsInRange(Local, Local->AttackRange, true)) {
					return object;
				}
			}
		}
		GameObject* target = TargetSelector::GetTarget(Local->AttackRange, DamageType::Physical);
		if (target != nullptr)
			return target;
	}
	if (_mode == OrbwalkingMode::LaneClear || _mode == OrbwalkingMode::Mixed || _mode == OrbwalkingMode::LastHit || _mode == OrbwalkingMode::Freeze)
	{
		auto newList = MinionListAA;
		newList.sort([&](GameObject* first, GameObject* next) {
			return HealthPrediction::GetHealthPrediction(first, 1200) < HealthPrediction::GetHealthPrediction(next, 1200) && first->MaxHealth > next->MaxHealth;
			});
		for (auto minion : newList)
		{
			//if (!minion->IsNeutral())
			{
				float t = (int)(Function::GetAttackCastDelay(Local) * 1000) + BrainFarmInt + Function::GetPing() / 2 + 1000 * (int)max(0, Local->Position.Distance(minion->Position) - Local->GetBoundingRadius()) / (int)GetMyProjectileSpeed();

				if (_mode == OrbwalkingMode::Freeze)
					t += 200 + Function::GetPing() / 2;

				float predHealth = HealthPrediction::GetHealthPrediction(minion, t, 0);

				bool killable = predHealth <= Helper::CalcAutoAttackDamage(Local, minion);
				if (_mode == OrbwalkingMode::Freeze)
				{
					if (minion->Health + (minion->Armor + minion->BonusArmor) < 50 || predHealth <= 50)
					{
						return minion;
					}
				}
				else
				{
					if (CanAttack())
					{
						DelayOnFire = t + Function::GameTimeTick();
						DelayOnFireId = minion->NetworkID;
					}

					if (predHealth <= 0)
					{
						if (HealthPrediction::GetHealthPrediction(minion, t - 50, 0) > 0)
						{
							return minion;
						}
					}

					else if (killable)
					{
						return minion;
					}
				}
			}
		}
	}
	if (CanAttack())
	{
		DelayOnFire = 0;
	}

	if (_mode != OrbwalkingMode::LastHit)
	{
		GameObject* target = TargetSelector::GetTarget(Local->AttackRange, DamageType::Physical);
		if (Helper::isValidUnit(target) && target->IsInRange(Local, Local->AttackRange, true))
		{
			return target;
		}
	}

	if (_mode == OrbwalkingMode::LaneClear || _mode == OrbwalkingMode::Mixed) {
		for (GameObject* building : filter(ObjectManager::TurretList(), [&](GameObject* Building) {return Helper::isValidUnit(Building, Local->AttackRange) && Local->IsInRange(Building, Local->AttackRange, true); }))
			return building;
	}

	if (_mode == OrbwalkingMode::LaneClear)
	{
		if (!ShouldWait())
		{
			if (selectedPriority) {
				if (int selectedId = Function::GetSelected()) {
					GameObject* object = ObjectManager::GetObjectByIndex(selectedId);
					if ((object->IsEnemyTo(Local) || object->IsNeutral()) && Helper::isValidUnit(object) && object->IsInRange(Local, Local->AttackRange, true)) {
						return object;
					}
				}
			}
			if (_prevMinion && Helper::isValidUnit(_prevMinion) && _prevMinion->IsInRange(Local, Local->AttackRange, true))
			{
				float predHealth = HealthPrediction::LaneClearHealthPrediction(_prevMinion, (int)(Function::GetAttackDelay(Local) * 1000 * 2.f), 0);
				if (predHealth >= 2 * Local->BaseAttackDamage + Local->BonusAttackDamage || abs(predHealth - _prevMinion->Health) < FLT_EPSILON)
					return _prevMinion;
			}
			MinionListAA.sort([&](GameObject* first, GameObject* next) {
				return first->MaxHealth > next->MaxHealth;
				});
			for (auto minion : MinionListAA)
			{
				if (!Function::IsMinion(minion))
					continue;
				float predHealth = HealthPrediction::LaneClearHealthPrediction(minion, (int)(Function::GetAttackDelay(Local) * 1000 * 2.f, 0));
				if (predHealth >= 2 * Local->BaseAttackDamage + Local->BonusAttackDamage || abs(predHealth - minion->Health) < FLT_EPSILON)
					result = minion;
			}

			if (result)
				_prevMinion = result;
		}
	}
	return result;
}

void OrbWalker::OnDraw()
{
	if (!enabled)
		return;
	OnUpdate();
	//Render::BeginOverlay();

	//Render::Draw_Circle3D(Local->Position, (float)(Local->AttackRange + Function::GetBoundingRadius(Local)), ImColor::ImColor(255, 255, 255), 1);

	/*for (auto obj : ObjectManager::HeroList()) {
		if (obj->IsEnemyTo(Local) && obj->IsVisible && Function::IsAlive(obj))
			Render::Draw_Circle3D(obj->Position, Function::GetBoundingRadius(obj), ImColor::ImColor(255, 255, 255), 1);
	}*/

	//Render::EndOverlay();
	//render.draw_circle(Local->Position, Local->AttackRange + Local->GetBoundingRadius(), ImColor(0, 255, 0));
}

void OrbWalker::OnMenu()
{
	if (ImGui::CollapsingHeader("OrbWalker")) {
		ImGui::Checkbox("Enabled", &OrbWalker::enabled);
		ImGui::Checkbox("Prioritize selected", &OrbWalker::selectedPriority);
		ImGui::Checkbox("Use IssueOrder (RISK OF BAN !!! )", &OrbWalker::useIssueOrder);
		ImGui::Checkbox("Lock camera on hero", &OrbWalker::lockCamera);
	}
}


void OrbWalker::OnProcessSpell(void* spellBook, SpellInfo* castInfo) {

	if (Local->Index != castInfo->source_id)
		return;

	if (castInfo->Slot == kSpellSlot::SpellSlot_SpecialAttack)
	{
		LastAttackCommandT = 0;
		return;
	}

	//for (auto obj : AttackResets) {
	//	if (StringContains(castInfo->BasicAttackSpellData->Name, obj, true)) {
	//		Reseted = true;
	//		LastAttackCommandT = 0;
	//		return;
	//	}
	//}

	if (castInfo->BasicAttackSpellData != nullptr && (castInfo->IsBasicAttack || castInfo->IsSpecialAttack || castInfo->IsHeadshotAttack))
	{
		Reseted = false;
		LastAttackCommandT = Function::GameTimeTick() - Function::GetPing() / 2;
		LastMoveCommandT = 0;
	}

}

void OrbWalker::OnDeleteObject(void* thisPtr, GameObject* obj)
{}

void OrbWalker::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnProcessSpell, OnProcessSpell);
	EventManager::AddEventHandler(EventManager::EventType::OnDeleteObject, OnDeleteObject);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, OnDraw);

	LastMoveCommandT = 0;
	LastAttackCommandT = 0;

}

void OrbWalker::OnLoad()
{
	LastMoveCommandT = 0;
	LastAttackCommandT = 0;
}

void OrbWalker::OnUnload()
{
	EventManager::RemoveEventHandler(EventManager::EventType::OnProcessSpell, OnProcessSpell);
	EventManager::AddEventHandler(EventManager::EventType::OnDeleteObject, OnDeleteObject);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, OnDraw);
}