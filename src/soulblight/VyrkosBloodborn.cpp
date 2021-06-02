/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/VyrkosBloodborn.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_unitSize = 3;
    static const int g_pointsPerUnit = 100;

    bool VyrkosBloodborn::s_registered = false;

    Unit *VyrkosBloodborn::Create(const ParameterList &parameters) {
        return new VyrkosBloodborn();
    }

    int VyrkosBloodborn::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void VyrkosBloodborn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Vyrkos Bloodborn", factoryMethod);
        }
    }

    VyrkosBloodborn::VyrkosBloodborn() :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Vyrkos Bloodborn", 10, g_wounds, 10, 6, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, VYRKOS_BLOOD_BORN};
        m_weapons = {&m_blade};

        for (auto i = 0; i < g_unitSize; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }
    }

    Wounds VyrkosBloodborn::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SoulblightBase::applyWoundSave(wounds, attackingUnit);
        // Shadowfast
        int numSixes = 0;
        return ignoreWounds(totalWounds, 5, numSixes);
    }

} // namespace Soulblight
