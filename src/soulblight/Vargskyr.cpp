/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Vargskyr.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 100;

    bool Vargskyr::s_registered = false;

    Unit *Vargskyr::Create(const ParameterList &parameters) {
        return new Vargskyr();
    }

    int Vargskyr::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void Vargskyr::Init() {
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
            s_registered = UnitFactory::Register("Vargskyr", factoryMethod);
        }
    }

    Vargskyr::Vargskyr() :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Vargskyr", 8, g_wounds, 10, 5, false, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, VAMPIRE, VARGSKYR};
        m_weapons = {&m_talons, &m_maw};

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talons);
        model->addMeleeWeapon(&m_maw);
        addModel(model);
    }

} // namespace Soulblight
