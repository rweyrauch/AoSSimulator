/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "DeathLore.h"

namespace Death {

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Overwhelming_Dread:
            case Lore::Fading_Vigour:
            case Lore::Spectral_Grasp:
            case Lore::Prison_Of_Grief:
            case Lore::Decrepify:
            case Lore::Soul_Harvest:
            case Lore::Blades_Of_Shyish:
            case Lore::Spirit_Gale:
            case Lore::Vile_Transference:
            case Lore::Amethystine_Pinions:
            case Lore::Soulpike:
            case Lore::Amaranthine_Orb:
                break;
            default:
                break;
        }
        return nullptr;
    }

} // namespace Death