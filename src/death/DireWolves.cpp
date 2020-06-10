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

namespace Death {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    bool DireWolves::s_registered = false;

    DireWolves::DireWolves() :
            LegionOfNagashBase("Dire Wolves", 10, WOUNDS, 10, 5, false),
            m_fangsAndClaws(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 2, 4, 4, 0, 1),
            m_fangsAndClawsDoom(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE, DIRE_WOLVES};
        m_weapons = {&m_fangsAndClaws, &m_fangsAndClawsDoom};
    }

    bool DireWolves::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        auto doomWolf = new Model(BASESIZE, wounds());
        doomWolf->addMeleeWeapon(&m_fangsAndClawsDoom);
        addModel(doomWolf);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_fangsAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DireWolves::Create(const ParameterList &parameters) {
        auto unit = new DireWolves();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (Legion) GetEnumParam("Legion", parameters, GrandHostOfNagash);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Legion", g_legion[0], g_legion)
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

    int DireWolves::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);

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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace Death
