/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SlaangorFiendbloods.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool SlaangorFiendbloods::s_registered = false;

    SlaangorFiendbloods::SlaangorFiendbloods() :
            SlaaneshBase("Slaangor Fiendbloods", 8, g_wounds, 6, 5, false),
            m_claws(Weapon::Type::Melee, "Razor-sharp Claw(s)", 2, 3, 4, 3, -1, 1),
            m_gildedWeapon(Weapon::Type::Melee, "Gilded Weapon", 1, 2, 4, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, BEASTS_OF_CHAOS, SLAANGOR_FIENDBLOODS};
        m_weapons = {&m_claws, &m_gildedWeapon};
    }

    bool SlaangorFiendbloods::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto slake = new Model(g_basesize, wounds());
        slake->addMeleeWeapon(&m_claws);
        slake->addMeleeWeapon(&m_gildedWeapon);
        addModel(slake);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_claws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SlaangorFiendbloods::Create(const ParameterList &parameters) {
        auto unit = new SlaangorFiendbloods();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SlaangorFiendbloods::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Slaangor Fiendbloods", factoryMethod);
        }
    }

    int SlaangorFiendbloods::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int SlaangorFiendbloods::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        // Slaughter At Any Cost
        if (m_charged && !weapon->isMissile()) {
            attacks++;
        }

        return attacks;
    }

    Wounds SlaangorFiendbloods::onEndCombat(PlayerId player) {
        auto wounds = EventInterface::onEndCombat(player);

        // Obsessive Violence
        auto target = m_meleeTarget;

        if (!target) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (!units.empty()) {
                target = units.front();
            }
        }

        if (target) {
            Dice::RollResult result;
            Dice::RollD6(remainingModels(), result);
            target->applyDamage({0, result.rollsGE(4)}, this);
        }
        return wounds;
    }

} // namespace Slaanesh