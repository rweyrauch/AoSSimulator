/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/DireWolves.h>
#include <UnitFactory.h>
#include <Board.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 420;

    bool DireWolves::s_registered = false;

    DireWolves::DireWolves() :
            LegionOfNagashBase("Dire Wolves", 10, g_wounds, 10, 5, false),
            m_fangsAndClaws(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 2, 4, 4, 0, 1),
            m_fangsAndClawsDoom(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE, DIRE_WOLVES};
        m_weapons = {&m_fangsAndClaws, &m_fangsAndClawsDoom};
        m_battleFieldRole = Role::Battleline;
    }

    bool DireWolves::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto doomWolf = new Model(g_basesize, wounds());
        doomWolf->addMeleeWeapon(&m_fangsAndClawsDoom);
        addModel(doomWolf);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangsAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DireWolves::Create(const ParameterList &parameters) {
        auto unit = new DireWolves();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DireWolves::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEADWALKERS}
            };
            s_registered = UnitFactory::Register("Dire Wolves", factoryMethod);
        }
    }

    int DireWolves::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toWoundModifier(weapon, target);

        // Slavering Charge
        if (m_charged) { modifier += 1; }

        return modifier;
    }

    int DireWolves::toSaveModifier(const Weapon *weapon, const Unit* attacker) const {
        int modifier = Unit::toSaveModifier(weapon, attacker);

        // Vigour Necris
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
        for (auto ip : units) {
            if (ip->hasKeyword(CORPSE_CARTS)) {
                modifier += 1;
                break;
            }
        }
        return modifier;
    }

    int DireWolves::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Death
