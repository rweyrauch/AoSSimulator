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
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 7;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * g_maxUnitSize;

    bool SkullCannons::s_registered = false;

    SkullCannons::SkullCannons() :
            KhorneBase("Skull Cannons", 8, g_wounds, 10, 4, false),
            m_burningSkulls(Weapon::Type::Missile, "Burning Skulls", 30, 1, 3, 3, -2, RAND_D6),
            m_hellblades(Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1),
            m_gnashingMaw(Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, SKULL_CANNONS};
        m_weapons = {&m_burningSkulls, &m_hellblades, &m_gnashingMaw};
        m_battleFieldRole = Artillery;
        m_hasMount = true;
        m_gnashingMaw.setMount(true);
    }

    bool SkullCannons::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
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
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
        auto modifier = KhorneBase::toHitModifier(weapon, target);

        // Burning Skulls
        if ((weapon->name() == m_burningSkulls.name()) && (target->remainingModels() >= 10)) {
            modifier += 1;
        }
        return modifier;
    }

    int SkullCannons::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void SkullCannons::onEnemyModelSlain(int numModels, Unit *enemyUnit, Wounds::Source source) {
        if (numModels && !m_hasFoughtTwice) {
            // Grind Their Bones, Seize Their Skulls
            int numSlain;
            fight(-1, m_meleeTarget, numSlain);
            m_hasFoughtTwice = true;
        }
        KhorneBase::onEnemyModelSlain(numModels, enemyUnit, source);
    }

    void SkullCannons::onStartCombat(PlayerId player) {
        KhorneBase::onStartCombat(player);

        m_hasFoughtTwice = false;
    }

} // namespace Khorne