/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/SkullCannons.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * MAX_UNIT_SIZE;

    bool SkullCannons::s_registered = false;

    SkullCannons::SkullCannons() :
            KhorneBase("Skull Cannons", 8, WOUNDS, 10, 4, false),
            m_burningSkulls(Weapon::Type::Missile, "Burning Skulls", 30, 1, 3, 3, -2, RAND_D6),
            m_hellblades(Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1),
            m_gnashingMaw(Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, SKULL_CANNONS};
        m_weapons = {&m_burningSkulls, &m_hellblades, &m_gnashingMaw};
        m_battleFieldRole = Artillery;
        m_hasMount = true;
    }

    bool SkullCannons::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_burningSkulls);
            model->addMeleeWeapon(&m_hellblades);
            model->addMeleeWeapon(&m_gnashingMaw);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SkullCannons::Create(const ParameterList &parameters) {
        auto unit = new SkullCannons();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkullCannons::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SkullCannons::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    SkullCannons::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skull Cannons", factoryMethod);
        }
    }

    Wounds SkullCannons::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SkullCannons::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto modifier = Unit::toHitModifier(weapon, target);

        // Burning Skulls
        if ((weapon->name() == m_burningSkulls.name()) && (target->remainingModels() >= 10)) {
            modifier += 1;
        }
        return modifier;
    }

    int SkullCannons::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Khorne