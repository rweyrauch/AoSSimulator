/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightZephyros.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool KnightZephyros::s_registered = false;

    KnightZephyros::KnightZephyros() :
            StormcastEternal("Knight-Zephyros", 6, WOUNDS, 9, 3, false),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 3, 0, 1),
            m_tempestAxes(Weapon::Type::Melee, "Tempest Axe", 1, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_ZEPHYROS};
        m_weapons = {&m_boltstormPistol, &m_tempestAxes};

        // Tireless hunder
        m_runAndShoot = true;
    }

    bool KnightZephyros::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_boltstormPistol);
        model->addMeleeWeapon(&m_tempestAxes);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KnightZephyros::Create(const ParameterList &parameters) {
        auto unit = new KnightZephyros();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KnightZephyros::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Zephyros", factoryMethod);
        }
    }

    int KnightZephyros::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Lightning-fast Strikes
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);
        if (charged()) {
            attacks += 1;
        }
        return attacks;
    }

    int KnightZephyros::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals