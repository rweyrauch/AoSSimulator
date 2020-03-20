/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Plaguebearers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool Plaguebearers::s_registered = false;

    Plaguebearers::Plaguebearers() :
            NurgleBase("Plaguebearers", 4, WOUNDS, 10, 5, false),
            m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
            m_plagueswordPlagueRidden(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUEBEARERS};
        m_weapons = {&m_plaguesword, &m_plagueswordPlagueRidden};
    }

    bool Plaguebearers::configure(int numModels, bool iconBearer, bool pipers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_pipers = pipers;

        // Add the Plagueridden
        auto leader = new Model(BASESIZE, wounds());
        leader->addMeleeWeapon(&m_plagueswordPlagueRidden);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_plaguesword);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Plaguebearers::Create(const ParameterList &parameters) {
        auto unit = new Plaguebearers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool pipers = GetBoolParam("Pipers", parameters, false);

        bool ok = unit->configure(numModels, iconBearer, pipers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Plaguebearers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Plaguebearers::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    Plaguebearers::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Piper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Plaguebearers", factoryMethod);
        }
    }

    Wounds Plaguebearers::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int Plaguebearers::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        // Cloud of Flies
        int modifier = Unit::targetHitModifier(weapon, attacker);
        if (remainingModels() >= 20) {
            modifier -= 2;
        } else {
            modifier -= 1;
        }
        return modifier;
    }

    int Plaguebearers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    Rerolls Plaguebearers::toSaveRerolls(const Weapon *weapon) const {
        // Locus of Fecundity
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0f);
        for (auto unit : units) {
            if (unit->hasKeyword(DAEMON) && unit->hasKeyword(NURGLE) && unit->hasKeyword(HERO)) return RerollOnes;
        }
        return Unit::toSaveRerolls(weapon);
    }

} // namespace Nurgle

