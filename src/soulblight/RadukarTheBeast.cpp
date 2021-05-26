/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/RadukarTheBeast.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 0;

    bool RadukarTheBeast::s_registered = false;

    Unit *RadukarTheBeast::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new RadukarTheBeast(general);
    }

    int RadukarTheBeast::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void RadukarTheBeast::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Radukar The Beast", factoryMethod);
        }
    }

    RadukarTheBeast::RadukarTheBeast(bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Radukar The Beast", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, VAMPIRE_LORD, RADUKAR_THE_BEAST};
        m_weapons = {&m_claws, &m_blade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_claws);
        model->setName("Radukar The Beast");
        addModel(model);
    }

    void RadukarTheBeast::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
