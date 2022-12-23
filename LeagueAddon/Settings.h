#pragma once
#include "Input.h"

namespace Settings {
	namespace Global {
		inline bool useIssueOrder = true;
		inline bool useInput = true;
		inline bool useCastSpell;
		inline bool useNewCastSpell;
	}

	namespace Binding {
		inline HKey Spell1 = HKey::Q;
		inline HKey Spell2 = HKey::W;
		inline HKey Spell3 = HKey::E;
		inline HKey Spell4 = HKey::R;
		inline HKey Spell5 = HKey::D;
		inline HKey Spell6 = HKey::F;

		inline HKey Slot1 = HKey::Z;
		inline HKey Slot2 = HKey::X;
		inline HKey Slot3 = HKey::C;
		inline HKey Slot4 = HKey::V;
		inline HKey Slot5 = HKey::B;
		inline HKey Slot6 = HKey::N;

		static HKey GetSpellSlotBinding(int spellslot) {
			switch (spellslot) {
			case 0:
				return Spell1;
				break;
			case 1:
				return Spell2;
				break;
			case 2:
				return Spell3;
				break;
			case 3:
				return Spell4;
				break;
			case 4:
				return Spell5;
				break;
			case 5:
				return Spell6;
				break;
			}
		}
	}
}