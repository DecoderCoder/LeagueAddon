#include "Yasuo.h"
//#include "HealthBar.h"
#include <thread>
#include <chrono>
#include "../AddonEngine.h"
#include "../Structs.h"
#include "../ObjectManager.h"
#include "../Input.h"
#include "../EventManager/EventManager.h"

//#include "Prediction.h"

Vector3 Predict(Vector3 startPoint, Vector3 velocity, double milliseconds) {
	milliseconds /= 1000;
	milliseconds *= 16;
	return startPoint + (velocity * milliseconds);
}

GameObject* GetNearestToCursor(POINT cursorPos, std::vector<GameObject*>* objects, double nearestDistance) {
	if (objects->size() > 0) {
		GameObject* nearest = NULL;
		/*		for (int i = 0; i < objects->size(); i++)
				{
					if (obj.NetworkId != CheatEngine::localPlayer.NetworkId) {
						nearest = &obj;
						break;
					}
				}	*/
		for (auto obj : ObjectManager::HeroList()) {
			if (obj->NetworkID != Local->NetworkID) {
				Vector3 cursor = Vector3(cursorPos.x, cursorPos.y, 0);
				Vector3 lpW2S;
				Vector3 objW2S;
				Function::World2Screen(&Local->Position, &lpW2S);
				Function::World2Screen(&obj->Position, &objW2S);
				if (cursor.Distance(objW2S) < nearestDistance) {
					if (Function::IsAlive(obj) && obj->IsVisible && obj->IsEnemyTo(Local)) {
						nearest = obj;
						nearestDistance = lpW2S.Distance(objW2S);
					}

				}
			}
		}
		return nearest;
	}
	return NULL;
}

void Yasuo::MainThread() {

	CSpellSlot* QSpell = Local->SpellBook.GetSpellSlotByID(0);
	CSpellSlot* WSpell = Local->SpellBook.GetSpellSlotByID(1);
	CSpellSlot* ESpell = Local->SpellBook.GetSpellSlotByID(2);
	CSpellSlot* RSpell = Local->SpellBook.GetSpellSlotByID(3);
	POINT cursorPos;
	typedef BOOL(__stdcall* fGetCursorPos) (LPPOINT lpPoint);
	if (Q_Aim && (QSpell->GetName() == "YasuoQ1Wrapper" || QSpell->GetName() == "YasuoQ2Wrapper") && GetAsyncKeyState('Q') && QSpell->IsReady()) {

		((fGetCursorPos)Input::oGetCursorPos)(&cursorPos);
		std::list<GameObject*> nearPlayers;
		for (auto obj : ObjectManager::HeroList()) {
			if (Local->Position.Distance(obj->Position) < 520) {
				nearPlayers.push_back(obj);
			}
		}
		if (nearPlayers.size() == 0) {
			nearPlayers = ObjectManager::HeroList();
		}
		GameObject* obj = GetNearestToCursor(cursorPos, (vector<GameObject*>*) & nearPlayers, 520);
		if (obj != NULL) {
			AIManager* aiManager = obj->GetAIManager();
			if (obj != NULL && obj->NetworkID != Local->NetworkID) {
				Vector3 pos = obj->Position;
				Vector3 predictedPath = Predict(pos, aiManager->Velocity, 350 - (Local->AttackSpeedMulti * 100) / 2);
				Vector3 w2s;
				Function::World2Screen(&predictedPath, &w2s);
				if (Local->Position.Distance(predictedPath) <= 520) {
					//Functions.WorldToScreen(&pos, &w2s);
					if (w2s.x >= 0 && w2s.y >= 0 && w2s.x <= Render::RenderWidth && w2s.y <= Render::RenderHeight) {
						Input::Move(w2s.x, w2s.y);
						//Functions.PrintChat(Offset::Chat, ("Input::Move called Input::mMouse [" + to_string(Input::mMouseX) + "] [" + to_string(Input::mMouseY) + "]").c_str(), 0xFFFFFF);
						Q_Pressed = Function::GameTimeTick();
					}
				}
			}
		}
	}
}

void Yasuo::OnMenu() {
	if (Local->NetworkID != -1 && Local->GetChampionName() == "Yasuo") {
		if (ImGui::CollapsingHeader("Yasuo")) {
			ImGui::Checkbox("Q Aim Assist", &Q_Aim);
			//ImGui::Checkbox("Q last hit [Heroes]", &Q_Lasthit);

			//ImGui::Checkbox("Show skill damage", &SkillDamage);

			//ImGui::Checkbox("Ult Autocast", &AutoCast);
			//ImGui::Checkbox("Show skill Damage", &SkillDamage);
			ImGui::Separator();
		}
	}
}

void Yasuo::OnDraw() {
	MainThread();
	//int ultLvl = CheatEngine::localPlayer.GetSpellBook()->GetSpellSlotByID(3)->GetLevel();
	//ImGui::Begin("Overlay", &CheatEngine::OverlayOpen, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoTitleBar);
	Render::BeginOverlay();

	if (Yasuo::Q_Aim)
		for (auto obj : ObjectManager::HeroList()) {
			if (obj->IsEnemyTo(Local) && Function::IsAlive(obj)) {
				AIManager* aiManager = obj->GetAIManager();				
				Vector3 pos = obj->Position;
				//Vector3 w2sPos = obj.GetHpBarPosition();
				//Vector3 HpBarPosition = obj.GetHpBarPosition();
				Vector3 w2sPos;
				Function::World2Screen(&pos, &w2sPos);

				Vector3 predictedPath = Predict(pos, aiManager->Velocity, 350 - (Local->AttackSpeedMulti * 100) / 2);
				Vector3 w2sPrediction;
				Function::World2Screen(&predictedPath, &w2sPrediction);
				ImColor color;
				if (Local->Position.Distance(predictedPath) < 520)
					color = ImColor(192, 57, 43);
				else
					color = ImColor(41, 128, 185);
				Render::Draw_Line(ImVec2(w2sPos.x, w2sPos.y), ImVec2(w2sPrediction.x, w2sPrediction.y), color, 1);
				Render::Draw_Circle3D(predictedPath, 10, color, 1);
			}
		}

	Render::EndOverlay();
}

void Yasuo::Initialize() {
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, OnDraw);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
}