/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    Spell::Result PowerOfHysh::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
         // TODO: implement
         return Result::Failed;
    }

    PowerOfHysh::PowerOfHysh(Unit *caster) :
            Spell(caster, "Power of Hysh", 6, 3) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
    }

} // namespace LuminethRealmLords

