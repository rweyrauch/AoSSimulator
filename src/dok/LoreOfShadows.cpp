/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "LoreOfShadows.h"

namespace DaughtersOfKhaine {

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Steed_Of_Shadows:
                return new BuffModifierSpell(caster, "Steed of Shadows", 6, 0, Move_Distance, 16, Spell::Target::Self);
                break;
            case Lore::Pit_Of_Shades:
                break;
            case Lore::Mirror_Dance:
                break;
            case Lore::The_Withering:
                break;
            case Lore::Mindrazor:
                break;
            case Lore::Shroud_Of_Despair:
                break;
            default:
                break;
        }
        return nullptr;
    }

}
