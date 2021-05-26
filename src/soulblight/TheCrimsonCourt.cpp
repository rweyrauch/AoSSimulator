/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/TheCrimsonCourt.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 60;

    bool TheCrimsonCourt::s_registered = false;

    Unit *TheCrimsonCourt::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        return new TheCrimsonCourt(bloodline);
    }

    int TheCrimsonCourt::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void TheCrimsonCourt::Init() {
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
            s_registered = UnitFactory::Register("The Crimson Court", factoryMethod);
        }
    }

    TheCrimsonCourt::TheCrimsonCourt(CursedBloodline bloodline):
        SoulblightBase(bloodline, "The Crimson Court", 6, g_wounds, 10, 4, true, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, THE_CRIMSON_COURT};
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

    void TheCrimsonCourt::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
