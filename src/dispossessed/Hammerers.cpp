/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Hammerers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Dispossessed {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    bool Hammerers::s_registered = false;

    Hammerers::Hammerers() :
            Dispossessed("Hammerers", 4, WOUNDS, 7, 4, false),
            m_greatHammer(Weapon::Type::Melee, "Gromril Great Hammer", 1, 2, 3, 3, -1, 1),
            m_greatHammerKeeper(Weapon::Type::Melee, "Gromril Great Hammer", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HAMMERERS};
        m_weapons = {&m_greatHammer, &m_greatHammerKeeper};
    }

    bool Hammerers::configure(int numModels, bool standardBearer, bool musician) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_musician = musician;

        auto keeper = new Model(BASESIZE, wounds());
        keeper->addMeleeWeapon(&m_greatHammerKeeper);
        addModel(keeper);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_greatHammer);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Hammerers::Create(const ParameterList &parameters) {
        auto unit = new Hammerers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool musician = GetBoolParam("Musician", parameters, false);

        bool ok = unit->configure(numModels, standardBearer, musician);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hammerers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hammerers::Create,
                    Dispossessed::ValueToString,
                    Dispossessed::EnumStringToInt,
                    Hammerers::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Musician", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Grudge", Dispossessed::StuckUp, Dispossessed::StuckUp,
                             Dispossessed::SneakyAmbushers, 1}
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Hammerers", factoryMethod);
        }
    }

    bool Hammerers::battleshockRequired() const {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0f);
        for (auto ip : units) {
            // Kingsguard
            if (ip->hasKeyword(DISPOSSESSED) && ip->hasKeyword(HERO))
                return false;
        }
        return true;
    }

    int Hammerers::rollRunDistance() const {
        // Sound the Advance
        if (m_musician) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Hammerers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (m_standardBearer) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Hammerers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Dispossessed
