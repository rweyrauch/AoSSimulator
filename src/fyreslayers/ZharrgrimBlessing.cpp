/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/ZharrgrimBlessing.h>

namespace Fyreslayers {

    Prayer *CreateZharrgrimBlessing(Blessing which, Unit *priest) {
        switch (which) {
            case Blessing::Molten_Infusion:
                return nullptr;
            case Blessing::Searing_Heat:
                return new BuffModifierPrayer(priest, "Searing Heat", 3, 18, To_Hit_Melee, -1, Abilities::Target::Enemy); // TODO: Also debuff To_Hit_Missile
            case Blessing::Prayer_Of_Ash:
                return new BuffModifierPrayer(priest, "Prayer of Ash", 3, 18, To_Save, 1, Abilities::Target::SelfAndFriendly);
            case Blessing::Ember_Storm:
                return nullptr;
            case Blessing::Prayer_Of_Grimnirs_Fury:
                return nullptr;
            case Blessing::Gilded_Claws:
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }

} // namespace Khorne