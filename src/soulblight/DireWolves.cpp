/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/DireWolves.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 135;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool DireWolves::s_registered = false;

    DireWolves::DireWolves(CursedBloodline bloodline, int numModels, int points) :
            SoulblightBase(bloodline, "Dire Wolves", 10, g_wounds, 10, 5, false, points),
            m_fangsAndClaws(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 2, 4, 4, 0, 1),
            m_fangsAndClawsDoom(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEADWALKERS, SUMMONABLE, DIRE_WOLVES};
        m_weapons = {&m_fangsAndClaws, &m_fangsAndClawsDoom};
        m_battleFieldRole = Role::Battleline;

        auto doomWolf = new Model(g_basesize, wounds());
        doomWolf->addMeleeWeapon(&m_fangsAndClawsDoom);
        addModel(doomWolf);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangsAndClaws);
            addModel(model);
        }
    }

    Unit *DireWolves::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new DireWolves(bloodline, numModels, ComputePoints(parameters));
    }

    void DireWolves::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Dire Wolves", factoryMethod);
        }
    }

    int DireWolves::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = SoulblightBase::toWoundModifier(weapon, target);

        // Slavering Charge
        if (m_charged) { modifier += 1; }

        return modifier;
    }

    int DireWolves::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = SoulblightBase::toSaveModifier(weapon, attacker);

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

    int DireWolves::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Soulblight
