#include "Visual.h"



std::map<int, float> distanceToHero;
bool lastSelectedHighlightChangeColorChanged = false;
float lastSelectedHighlightChange = 0;

void Visual::Initialize() {

	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, OnDraw);

	for (auto obj : ObjectManager::HeroList()) {
		if (obj->IsEnemyTo(Local)) {
			distanceToHero.insert({ obj->NetworkID, 0 });
		}
	}
}


void Visual::OnThread() {

}

void Visual::OnMenu() {
	if (ImGui::CollapsingHeader("Visual")) {
		if (ImGui::TreeNode("ESP")) {
			ImGui::Checkbox("ESP", &ESP);
			if (ESP) {
				ImGui::SliderInt("ESP Distance", &ESP_Distance, 600, 15000);
			}
			ImGui::Checkbox("Show own attack radius", &ShowAttackRadius);
			ImGui::Checkbox("Show enemy bounding radius", &ShowBoundingRadius);
			ImGui::Checkbox("Highlight selected", &HighlightSelected);
			ImGui::TreePop();
		}
	}
}

void Visual::OnDraw() {
	Vector3 lw2s; // Local World 2 Screen
	Function::World2Screen(&Local->Position, &lw2s);

	Render::BeginOverlay();
	if (ESP) {
		for (auto obj : ObjectManager::HeroList()) {
			if (obj->IsEnemyTo(Local) && obj->IsVisible && Function::IsAlive(obj) && obj->IsVisible) {
				float distance = Local->Position.distanceTo(obj->Position);
				if (distance < ESP_Distance) {
					Vector3 ew2s; // Enemy W2S
					Function::World2Screen(&obj->Position, &ew2s);

					if (distanceToHero[obj->NetworkID] > distance)
						Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(231, 76, 60), 2);
					else
						Render::Draw_Line(lw2s.x, lw2s.y, ew2s.x, ew2s.y, ImColor(243, 156, 18), 2);


					Vector3 nameCoord = Local->Position.Extend(obj->Position, Local->AttackRange);

					Vector3 nameCoordw2s;
					Function::World2Screen(&nameCoord, &nameCoordw2s);

					Render::Draw_Text_Centered(ImVec2(nameCoordw2s.x, nameCoordw2s.y), ImVec2(50, 20), obj->GetChampionName(), 0xFFFFFFFF, 15);
				}
				distanceToHero[obj->NetworkID] = distance;
			}
		}
	}

	if (ShowAttackRadius) {
		Render::Draw_Circle3D(Local->Position, Local->AttackRange + Function::GetBoundingRadius(Local), ImColor(243, 156, 18), 3);
	}

	if (ShowBoundingRadius) {
		for (auto obj : ObjectManager::HeroList()) {
			if (obj->IsEnemyTo(Local) && obj->IsVisible && Function::IsAlive(obj) && obj->IsVisible)
				Render::Draw_Circle3D(obj->Position, Function::GetBoundingRadius(obj), ImColor::ImColor(255, 255, 255), 1);
		}
	}

	if (HighlightSelected) {
		int selected = Function::GetSelected();
		
		if (selected != 0) {
			if (GetTickCount() > lastSelectedHighlightChange) {
				// changeColor
				lastSelectedHighlightChangeColorChanged = !lastSelectedHighlightChangeColorChanged;
				lastSelectedHighlightChange = GetTickCount() + 500;
			}
			GameObject* selectedObj = ObjectManager::GetObjectByIndex(selected);
			if (selectedObj != nullptr)
				if (lastSelectedHighlightChangeColorChanged) {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(52, 152, 219), 2);
				}
				else {
					Render::Draw_Circle3D(selectedObj->Position, Function::GetBoundingRadius(selectedObj), ImColor(236, 240, 241), 2);
				}
		}
	}

	Render::EndOverlay();
}