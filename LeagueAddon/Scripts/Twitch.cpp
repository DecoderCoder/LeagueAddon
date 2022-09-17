#include "Twitch.h"

void Twitch::OnDraw()
{
	Render::BeginOverlay();
	if (_eDamage) {
		CSpellSlot* spellE = Local->SpellBook.GetSpellSlotByID(2);
		if (spellE->GetLevel() > 0 && spellE->IsReady()) {
			float baseDmg = 20 + (10 * (spellE->GetLevel() - 1));
			float baseStackDmg = 15 + (5 * (spellE->GetLevel() - 1));
			float baseStackAddPhysDmg = Local->BonusAttackDamage * 0.35; // Base stack additional Physical damage
			float baseStackAddMagDmg = Local->BaseAbilityDamage * 0.33;

			for (auto obj : ObjectManager::HeroList()) {
				if (obj->IsEnemyTo(Local) && Function::IsAlive(obj) && obj->Position.Distance(Local->Position) < 1200) {
					HealthBar healthBar = HealthBar(HealthBarType::UnderHero);
					BuffManager* buffMng = &obj->BuffManager;
					BuffEntry* venom = buffMng->getBuff("TwitchDeadlyVenom");
					if (venom) {
						float objHealth = obj->Health + obj->Shield;
						int stacks = venom->getCount();
						float physDamage = Helper::calcReducedDamage(baseDmg + ((baseStackDmg + baseStackAddPhysDmg) * stacks), obj->Armor, Local->ArmorPenMod, Local->ArmorPen, false);
						float magDamage = Helper::calcReducedDamage(baseStackAddMagDmg * stacks, obj->MagicRes, Local->MagicPenMod, Local->MagicPenFlat, false);

						ImColor color;
						if (obj->Health + obj->Shield < physDamage + magDamage)
							color = ImColor::ImColor(192, 57, 43);
						else if (stacks == 6)
							color = ImColor::ImColor(243, 156, 18); 
						else color = ImColor::ImColor(52, 152, 219);
						healthBar.AddRightArea(physDamage + magDamage, color);

						healthBar.Draw(obj->Position, obj->Health + obj->Shield, obj->MaxHealth);
					}
				}
			}
		}

	}
	Render::EndOverlay();
}

void Twitch::OnMenu()
{
	if (ImGui::CollapsingHeader("Twitch")) {
		ImGui::Checkbox("Show E Damage", &_eDamage);
	}
}

void Twitch::Initialize()
{
	EventManager::AddEventHandler(EventManager::EventType::OnMenu, &OnMenu);
	EventManager::AddEventHandler(EventManager::EventType::OnDraw, &OnDraw);
}
