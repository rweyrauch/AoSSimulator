/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/NeaveBlacktalon.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool NeaveBlacktalon::s_registered = false;

    NeaveBlacktalon::NeaveBlacktalon() :
            StormcastEternal("Neave Blacktalon", 6, g_wounds, 9, 3, false),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 3, 0, 1),
            m_whirlwindAxes(Weapon::Type::Melee, "The Whirlwind Axes", 1, 7, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, HERO, KNIGHT_ZEPHYROS,
                      NEAVE_BLACKTALON};
        m_weapons = {&m_boltstormPistol, &m_whirlwindAxes};
        m_battleFieldRole = Role::Leader;

        // Tireless hunter
        m_runAndShoot = true;
    }

    void NeaveBlacktalon::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_boltstormPistol);
        model->addMeleeWeapon(&m_whirlwindAxes);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *NeaveBlacktalon::Create(const ParameterList &parameters) {
        auto unit = new NeaveBlacktalon();

        unit->setStormhost(Stormhost::Hammers_Of_Sigmar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void NeaveBlacktalon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Neave Blacktalon", factoryMethod);
        }
    }

    int NeaveBlacktalon::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Lightning-fast Strikes
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);
        if (charged()) {
            attacks += 1;
        }
        return attacks;
    }

    Wounds NeaveBlacktalon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Nemesis
        if (target->hasKeyword(HERO)) {
            return {weapon->damage() + 1, 0};
        }
        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int NeaveBlacktalon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals