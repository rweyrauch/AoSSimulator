/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/KosargiNightguard.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_unitSize = 2;
    static const int g_pointsPerUnit = 100;

    bool KosargiNightguard::s_registered = false;

    Unit *KosargiNightguard::Create(const ParameterList &parameters) {
        return new KosargiNightguard();
    }

    int KosargiNightguard::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void KosargiNightguard::Init() {
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
            s_registered = UnitFactory::Register("Kosargi Nightguard", factoryMethod);
        }
    }

    KosargiNightguard::KosargiNightguard() :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Kosargi Nightguard", 5, g_wounds, 10, 5, false, g_pointsPerUnit) {

        m_keywords = {DEATH, OGOR, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, DEADWALKERS, KOSARGI_NIGHTGUARD};
        m_weapons = {&m_bardiche};

        for (auto i = 0; i < g_unitSize; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bardiche);
            addModel(model);
        }
    }

} // namespace Soulblight
