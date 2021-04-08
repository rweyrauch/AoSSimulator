/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/GarreksReavers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 60;

    bool GarreksReavers::s_registered = false;

    Unit *GarreksReavers::Create(const ParameterList &parameters) {
        return new GarreksReavers();
    }

    void GarreksReavers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GarreksReavers::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    GarreksReavers::ComputePoints,
                    {
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Garrek's Reavers", factoryMethod);
        }
    }

    GarreksReavers::GarreksReavers() :
            KhorneBase("Garrek's Reavers", 6, g_wounds, 6, 6, false),
            m_garreksAxe(Weapon::Type::Melee, "Garrek's Blooddrinker Axe", 1, 3, 3, 4, -1, 1),
            m_karusAxe(Weapon::Type::Melee, "Karsus' Chained Axe", 2, RAND_D3, 4, 4, 0, 1),
            m_saeksAxe(Weapon::Type::Melee, "Saek's Meatripper Axe", 1, 1, 3, 4, -1, 1),
            m_blades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, BLOODREAVERS, GARREKS_REAVERS};
        m_weapons = {&m_garreksAxe, &m_karusAxe, &m_saeksAxe, &m_blades};

        setSlaughterHost(SlaughterHost::Goretide);

        auto garrek = new Model(g_basesize, wounds());
        garrek->setName("Garrek");
        garrek->addMeleeWeapon(&m_garreksAxe);
        addModel(garrek);

        auto karsus = new Model(g_basesize, wounds());
        karsus->setName("Karsus");
        karsus->addMeleeWeapon(&m_karusAxe);
        addModel(karsus);

        auto saek = new Model(g_basesize, wounds());
        saek->setName("Saek");
        saek->addMeleeWeapon(&m_saeksAxe);
        addModel(saek);

        auto targor = new Model(g_basesize, wounds());
        targor->setName("Targor");
        targor->addMeleeWeapon(&m_blades);
        addModel(targor);

        auto arnulf = new Model(g_basesize, wounds());
        arnulf->setName("Arnulf");
        arnulf->addMeleeWeapon(&m_blades);
        addModel(arnulf);

        m_points = g_pointsPerUnit;
    }

    Rerolls GarreksReavers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaver Blades
        if (weapon->name() == m_blades.name()) {
            return Rerolls::Ones;
        }

        return KhorneBase::toHitRerolls(weapon, unit);
    }

    int GarreksReavers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);

        // Frenzied Devotion
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0);
        for (auto ip : units) {
            if (ip->hasKeyword(KHORNE) && ip->hasKeyword(TOTEM)) {
                attacks += 1;
                break;
            }
        }

        return attacks;
    }

    Wounds GarreksReavers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_garreksAxe.name())) {
            return {0, 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int GarreksReavers::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void GarreksReavers::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                     const Wounds &weaponDamage) {
        KhorneBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Grisly Trophies
        if ((numSlain > 0) && (weapon->name() == m_garreksAxe.name())) {
            buffAbility(Ability::Ignore_Battleshock, 1, {Phase::Battleshock, m_battleRound, owningPlayer()});
        }
    }

} // namespace Khorne
