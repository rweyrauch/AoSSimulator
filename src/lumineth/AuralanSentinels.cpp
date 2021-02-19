/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/AuralanSentinels.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 280;

    bool AuralanSentinels::s_registered = false;

    AuralanSentinels::AuralanSentinels() :
            LuminethBase("Vanari Auralan Sentinels", 6, g_wounds, 6, 5, false),
            m_bowAimed(Weapon::Type::Missile, "Auralan Bow: Aimed", 18, 1, 3, 4, -1, 1),
            m_bowLofted(Weapon::Type::Missile, "Auralan Bow: Lofted", 30, 1, 4, 4, 0, 1),
            m_blade(Weapon::Type::Melee, "Champion's Blade", 1, 2, 3, 4, -1, 1),
            m_dagger(Weapon::Type::Melee, "Vanari Dagger", 1, 1, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, AURALAN_SENTINELS, Sunmetal_Weapons};
        m_weapons = {&m_bowAimed, &m_bowLofted, &m_blade, &m_dagger};
        m_battleFieldRole = Role::Other;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool AuralanSentinels::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto highSentinel = new Model(g_basesize, wounds());
        highSentinel->addMeleeWeapon(&m_blade);
        highSentinel->setName("High Sentinel");
        addModel(highSentinel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bowAimed);
            model->addMissileWeapon(&m_bowLofted);
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }

        m_bowAimed.activate(true);
        m_bowLofted.activate(false);

        m_knownSpells.push_back(std::make_unique<PowerOfHysh>(this));

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AuralanSentinels::Create(const ParameterList &parameters) {
        auto unit = new AuralanSentinels();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuralanSentinels::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Auralan Sentinels", factoryMethod);
        }
    }

    int AuralanSentinels::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void AuralanSentinels::onRestore() {
        LuminethBase::onRestore();

        m_powerOfHyshActive = false;
    }

    void AuralanSentinels::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        if (spell->name() == "Power of Hysh") {
            m_powerOfHyshActive = true;
        }
    }

    Wounds AuralanSentinels::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        auto hitRollThreshold = 6;
        if (m_powerOfHyshActive) hitRollThreshold = 5;

        if ((hitRoll >= hitRollThreshold) && ((weapon->name() == m_bowLofted.name()) || (weapon->name() == m_bowAimed.name()))) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void AuralanSentinels::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_powerOfHyshActive = false;
    }

    void AuralanSentinels::onStartShooting(PlayerId player) {
        // Many-stringed Weapon
        auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (nearestUnit) {
            double rangeTo = distanceTo(nearestUnit);
            if (rangeTo < (double) m_bowAimed.range()) {
                m_bowAimed.activate(true);
                m_bowLofted.activate(false);
            } else {
                m_bowAimed.activate(false);
                m_bowLofted.activate(true);
            }
        }
    }

} // namespace LuminethRealmLords