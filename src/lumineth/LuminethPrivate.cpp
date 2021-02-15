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

    Spell::Result PowerOfHysh::cast(Unit *target, int round) {
        auto result = Result::Failed;
        if (m_caster->remainingModels() >= 5) {
            const int castingRoll = m_caster->rollCasting();
            if (castingRoll >= m_castingValue) {
                bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
                if (!unbound) {
                    SimLog(Verbosity::Narrative, "%s spell %s was cast.\n", m_caster->name().c_str(),
                           name().c_str());
                    result = Result::Success;
                } else {
                    SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(),
                           name().c_str());
                    result = Result::Unbound;
                }
            } else {
                SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(),
                       name().c_str(),
                       castingRoll, m_castingValue);
            }
        }
        return result;
    }

    PowerOfHysh::PowerOfHysh(Unit *caster) :
            Spell(caster, "Power of Hysh", 6, 3.0f) {
        m_allowedTargets = Spell::Target::SelfAndFriendly;
    }

} // namespace LuminethRealmLords

