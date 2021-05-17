/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/TheSepulchralGuard.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 80;

    bool TheSepulchralGuard::s_registered = false;

    Unit *TheSepulchralGuard::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        return new TheSepulchralGuard(bloodline);
    }

    int TheSepulchralGuard::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void TheSepulchralGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("The Sepulchral Guard", factoryMethod);
        }
    }

    TheSepulchralGuard::TheSepulchralGuard(CursedBloodline bloodline):
        SoulblightBase(bloodline, "The Sepulchral Guard", 4, g_wounds, 10, 6, false, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, DEATHRATTLE_SKELETONS, THE_SEPULCHRAL_GUARD};
        m_weapons = {&m_mace, &m_blades, &m_bludgeon};

        auto gorath = new Model(g_basesize, wounds()+2);
        gorath->addMeleeWeapon(&m_mace);
        gorath->setName("Gorath the Enforcer");
        addModel(gorath);

        auto vellas = new Model(g_basesize, wounds());
        vellas->addMeleeWeapon(&m_blades);
        vellas->setName("Vellas von Faine");
        addModel(vellas);

        auto ennias = new Model(g_basesize, wounds());
        ennias->addMeleeWeapon(&m_bludgeon);
        ennias->setName("Ennias Curse-born");
        addModel(ennias);
    }

} // namespace Soulblight
