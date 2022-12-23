#include "AddonEngine.h"

using namespace std;

Vector3 vector22;

DWORD* __fastcall OnProcessSpellCast(void* thisptr, void* edx, int state, SpellInfo* spellCastInfo, int a6) noexcept
{
	MessageBoxA(0, spellCastInfo->BasicAttackSpellData->Name.c_str(), "", 0);
	return 0;
}

void* HookVTableFunction2(void* pVTable, void* fnHookFunc, int nOffset) // https://guidedhacking.com/threads/how-to-hook-directinput-emulate-key-presses.14011/
{
	DWORD alloc = (DWORD)VirtualAlloc(NULL, 0x1F8, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	intptr_t ptrVtable = *((intptr_t*)pVTable);
	memcpy((void*)alloc, (void*)ptrVtable, 0x1F8);

	DWORD* vTable = (DWORD*)alloc;
	vTable[nOffset] = (DWORD)fnHookFunc;

	*(DWORD*)pVTable = (DWORD)alloc;
	return 0;
	//intptr_t ptrFunction = ptrVtable + sizeof(intptr_t) * nOffset;
	//intptr_t ptrOriginal = *((intptr_t*)ptrFunction);
	//MEMORY_BASIC_INFORMATION mbi;
	////VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
	//DWORD old;
	//VirtualProtect((void*)ptrFunction, 4, PAGE_EXECUTE_READWRITE, &old);
	//*((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;

	//VirtualProtect((void*)ptrFunction, 4, old, &old);
	//return (void*)ptrOriginal;
	
}

void DrawMenu() {
	BlockInput(true);

	ImGui::Begin("LeagueAddon");

	if (Hooks::Hooked) {
		ImGui::Text(("Local ID: " + to_string(Local->NetworkID)).c_str());		
		if (GetAsyncKeyStateN(VK_HOME)) {
			for (auto obj : ObjectManager::HeroList()) {
				GameObject* lp = obj;
				//MessageBoxA(0, to_hex((int)(void*)((int)lp + 0xF30)).c_str(), "", 0);
				HookVTableFunction2((void*)((int)lp + 0xF30), &OnProcessSpellCast, 29);
			}
		}

		/*ImGui::Text(("x" + to_string(ai->Facing.x)).c_str());
		ImGui::Text(("y" + to_string(ai->Facing.y)).c_str());
		ImGui::Text(("z" + to_string(ai->Facing.z)).c_str());*/

		//ImGui::InputInt("SpellNum", &spellNum);
		//ImGui::InputInt("castType", &castType);
		//if (ImGui::Button("CastSpell")) {
		////	MessageBoxA(0, to_hex(get_peb2()).c_str(), "get_peb", 0);
		//	CastSpell(spellNum, castType);
		//}


		//Function::World2Screen(&Local->Position, &vector22);
		//ImGui::Text(("OX: " + to_string(vector22.x)).c_str());
		//ImGui::Text(("OY: " + to_string(vector22.y)).c_str());
		//ImGui::Text(("OZ: " + to_string(vector22.z)).c_str());


		//if (ImGui::CollapsingHeader("Missiles")) {
		//	auto missiles = ObjectManager::MissileList();
		//	ImGui::Text(to_string(missiles.size()).c_str());
		//	for (auto obj : missiles) {
		//		if (!IsBadReadPtr(obj, 1)) {
		//			Vector3 w2s;
		//			Vector3* spellPos = (Vector3*)((int)obj + 0xD8);
		//			//Vector3* spellEndPos = (Vector3*)((int)obj + 0x2DC);
		//			Function::World2Screen(spellPos, &w2s);
		//			ImGui::Text(GetStr((DWORD)obj + 0x54));

		//			Render::BeginOverlay();

		//			Render::Rect(obj->CurrentPos, obj->EndPos, 0, 100, ImColor(255, 0, 0));
		//			Render::Draw_Text(w2s.x, w2s.y, GetStr((DWORD)obj + 0x54));

		//			

		//			Render::EndOverlay();
		//		}
		//		//else
		//		//	ImGui::Text(to_hex((int)obj).c_str());
		//	}
		//}	



		EventManager::Trigger(EventManager::EventType::OnMenu);

		if (ImGui::CollapsingHeader("Settings")) {
			if (ImGui::TreeNode("Main settings")) {
				ImGui::Checkbox("Use IssueOrder", &Settings::Global::useIssueOrder);
				if (ImGui::BeginCombo("CastSpell", Settings::Global::useInput ? "Input" : (Settings::Global::useNewCastSpell ? "NewCastSpell" : (Settings::Global::useCastSpell ? "CastSpell" : "None")))) {
					if (ImGui::Selectable("Input", Settings::Global::useInput)) {
						Settings::Global::useInput = true;
						Settings::Global::useNewCastSpell = false;
						Settings::Global::useCastSpell = false;
					}
					if (ImGui::Selectable("NewCastSpell", Settings::Global::useNewCastSpell)) {
						Settings::Global::useInput = false;
						Settings::Global::useNewCastSpell = true;
						Settings::Global::useCastSpell = false;
					}
					if (ImGui::Selectable("CastSpell", Settings::Global::useCastSpell)) {
						Settings::Global::useInput = false;
						Settings::Global::useNewCastSpell = false;
						Settings::Global::useCastSpell = true;
					}
					ImGui::EndCombo();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Target Selector")) {
				if (ImGui::BeginCombo("Target mode 1", TargetSelector::targetingModeLabels[(int)TargetSelector::mode])) {
					for (int i = 0; i < 10; i++) {
						if (ImGui::Selectable(TargetSelector::targetingModeLabels[i], (int)TargetSelector::mode == i)) {
							TargetSelector::mode = (TargetingMode)i;
						}
					}
					ImGui::EndCombo();
				}
				if (TargetSelector::mode == TargetingMode::AutoPriority) {
					if (ImGui::BeginCombo("Target mode 2", TargetSelector::targetingModeLabels[(int)TargetSelector::mode2])) {
						for (int i = 1; i < 10; i++) {
							if (ImGui::Selectable(TargetSelector::targetingModeLabels[i], (int)TargetSelector::mode2 == i)) {
								TargetSelector::mode2 = (TargetingMode)i;
							}
						}
						ImGui::EndCombo();
					}
					ImGui::Separator();

					ImGui::Text("Drag the hero at this label to attack priority");

					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
						if (payload != NULL) {
							if (*(int*)payload->Data >= 10 && *(int*)payload->Data < 20) {
								int pos = *(int*)payload->Data - 10;
								TargetSelector::attackOrder.insert(TargetSelector::attackOrder.begin() + 0, TargetSelector::attackOrderIgnore[pos]);
								TargetSelector::attackOrderIgnore.erase(TargetSelector::attackOrderIgnore.begin() + pos);
							}
							else if (*(int*)payload->Data >= 20) {
								int pos = *(int*)payload->Data - 20;
								TargetSelector::attackOrder.insert(TargetSelector::attackOrder.begin() + 0, TargetSelector::attackOrderLast[pos]);
								TargetSelector::attackOrderLast.erase(TargetSelector::attackOrderLast.begin() + pos);
							}
						}
						ImGui::EndDragDropTarget();
					}

					int secondTarget = 0;
					for (int i = 0; i < TargetSelector::attackOrder.size(); i++) {
						ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrder[i]->GetChampionName()).c_str());
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
							ImGui::SetDragDropPayload("TARGETORDER", &i, sizeof(int));
							ImGui::Text(TargetSelector::attackOrder[i]->GetChampionName().c_str());
							ImGui::EndDragDropSource();
							Render::BeginOverlay();
							for (auto hero : ObjectManager::HeroList()) {
								if (hero->NetworkID == TargetSelector::attackOrder[i]->NetworkID) {
									Render::Draw_Circle3DFilled(hero->Position, hero->GetBoundingRadius(), ImColor(255, 0, 0));
									break;
								}

							}
							Render::EndOverlay();
						}

						if (ImGui::BeginDragDropTarget())
						{
							const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
							if (payload != NULL) {
								if (*(int*)payload->Data >= 10 && *(int*)payload->Data < 20) {
									int pos = *(int*)payload->Data - 10;
									TargetSelector::attackOrder.insert(TargetSelector::attackOrder.begin() + i + 1, TargetSelector::attackOrderIgnore[pos]);
									TargetSelector::attackOrderIgnore.erase(TargetSelector::attackOrderIgnore.begin() + pos);
								}
								else if (*(int*)payload->Data >= 20) {
									int pos = *(int*)payload->Data - 20;
									TargetSelector::attackOrder.insert(TargetSelector::attackOrder.begin() + i + 1, TargetSelector::attackOrderLast[pos]);
									TargetSelector::attackOrderLast.erase(TargetSelector::attackOrderLast.begin() + pos);
								}
								else {
									GameObject* swap = TargetSelector::attackOrder[i];
									TargetSelector::attackOrder[i] = TargetSelector::attackOrder[*(int*)payload->Data];
									TargetSelector::attackOrder[*(int*)payload->Data] = swap;
								}
							}
							ImGui::EndDragDropTarget();
						}
					}
					ImGui::Separator();
					ImGui::Text("Drag the hero at this label to ignore priority");

					if (ImGui::BeginDragDropTarget()) {

						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
						if (payload != NULL) {
							if (*(int*)payload->Data < 10) {
								TargetSelector::attackOrderIgnore.push_back(TargetSelector::attackOrder[*(int*)payload->Data]);
								TargetSelector::attackOrder.erase(TargetSelector::attackOrder.begin() + *(int*)payload->Data);
							}
							else if (*(int*)payload->Data >= 20) {
								TargetSelector::attackOrderIgnore.push_back(TargetSelector::attackOrderLast[*(int*)payload->Data - 20]);
								TargetSelector::attackOrderLast.erase(TargetSelector::attackOrderLast.begin() + *(int*)payload->Data - 20);
							}
						}
					}
					for (int i = 0; i < TargetSelector::attackOrderIgnore.size(); i++) {
						ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrderIgnore[i]->GetChampionName()).c_str());
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
							secondTarget = i + 10;
							ImGui::SetDragDropPayload("TARGETORDER", &secondTarget, sizeof(int));
							ImGui::Text(TargetSelector::attackOrderIgnore[secondTarget - 10]->GetChampionName().c_str());
							ImGui::EndDragDropSource();
						}
					}
					ImGui::Separator();

					ImGui::Text("Drag the hero at this label to attack last");

					if (ImGui::BeginDragDropTarget()) {

						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TARGETORDER");
						if (payload != NULL && *(int*)payload->Data < 20) {
							if (*(int*)payload->Data < 10) {	// 1
								TargetSelector::attackOrderLast.push_back(TargetSelector::attackOrder[*(int*)payload->Data]);
								TargetSelector::attackOrder.erase(TargetSelector::attackOrder.begin() + *(int*)payload->Data);
							}
							else {								// 2
								TargetSelector::attackOrderLast.push_back(TargetSelector::attackOrderIgnore[*(int*)payload->Data - 10]);
								TargetSelector::attackOrderIgnore.erase(TargetSelector::attackOrderIgnore.begin() + *(int*)payload->Data - 10);
							}
						}
					}
					for (int i = 0; i < TargetSelector::attackOrderLast.size(); i++) {
						ImGui::Text(("[" + to_string(i) + "] " + TargetSelector::attackOrderLast[i]->GetChampionName()).c_str());
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
							secondTarget = i + 20;
							ImGui::SetDragDropPayload("TARGETORDER", &secondTarget, sizeof(int));
							ImGui::Text(TargetSelector::attackOrderLast[secondTarget - 20]->GetChampionName().c_str());
							ImGui::EndDragDropSource();
						}
					}
					ImGui::Separator();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Debug")) {
				if (ImGui::Checkbox("Debug mode", &Debug::Enabled)) {
					Debug::detectedSpells.clear();
					Debug::detectedSpellsPtr.clear();
				}
				ImGui::Separator();
				ImGui::Checkbox("Draw objects info", &Debug::drawObjectInfo);
				if (ImGui::TreeNode("Object info")) {
					ImGui::Checkbox("Buffs", &Debug::drawObjectInfoBuffs);
					ImGui::TreePop();
				}
				ImGui::Checkbox("Draw spells direction", &Debug::drawSpellDirection);
				ImGui::TreePop();
			}


		}
	}

	ImGui::End();
}

bool AddonEngine::Initialize() {
	Local = ObjectManager::GetLocalPlayer();
	TargetSelector::Initialize();

	// Init interface

	//

	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &DrawMenu);

	//// Scripts

	if (Local->GetChampionName() == "Twitch")
		Twitch::Initialize();
	if (Local->GetChampionName() == "Garen")
		Garen::Initialize();
	if (Local->GetChampionName() == "Akali")
		Akali::Initialize();
	if (Local->GetChampionName() == "Yasuo")
		Yasuo::Initialize();
	if (Local->GetChampionName() == "Draven")
		Draven::Initialize();
	if (Local->GetChampionName() == "Vayne")
		Vayne::Initialize();
	if (Local->GetChampionName() == "Tristana")
		Tristana::Initialize();

	//// Main code

	Visual::Initialize();
	OrbWalker::Initialize();
	Evade::Core::Initalize();
	//JustEvade::Core::Initialize();
	SkinChanger::Initialize();
	Misc::Initialize();

	Debug::Initialize();

	return true;
}

void AddonEngine::Uninitialize() {
	EventManager::Trigger(EventManager::EventType::OnUnload);
}