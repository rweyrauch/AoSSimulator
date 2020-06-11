/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVenator.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool KnightVenator::s_registered = false;

    KnightVenator::KnightVenator() :
            StormcastEternal("Knight-Venator", 12, WOUNDS, 9, 3, true),
            m_realmhuntersBow(Weapon::Type::Missile, "Realmhunter's Bow", 30, 3, 2, 3, -1, 1),
            m_beakAndTalonsMissile(Weapon::Type::Missile, "Celestial Beak and Talons", 30, 3, 4, 3, 0, 1),
            m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 2, 3, 4, 0, 1),
            m_beakAndTalons(Weapon::Type::Melee, "Celestial Beak and Talons", 1, 3, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VENATOR};
        m_weapons = {&m_realmhuntersBow, &m_beakAndTalonsMissile, &m_bowStave, &m_beakAndTalons};
    }

    bool KnightVenator::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_realmhuntersBow);
        model->addMissileWeapon(&m_beakAndTalonsMissile);
        model->addMeleeWeapon(&m_bowStave);
        model->addMeleeWeapon(&m_beakAndTalons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KnightVenator::Create(const ParameterList &parameters) {
        auto unit = new KnightVenator();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KnightVenator::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Venator", factoryMethod);
        }
    }

    int KnightVenator::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Celestial Strike
        if ((woundRoll == 6) && (weapon->name() == m_beakAndTalonsMissile.name())) {
            return -3;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int KnightVenator::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }


} // namespace StormcastEternals