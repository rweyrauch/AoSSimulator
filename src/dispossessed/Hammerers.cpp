/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Hammerers.h>
#include "DispossessedPrivate.h"
#include <UnitFactory.h>
#include <Board.h>

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 420;

    bool Hammerers::s_registered = false;

    Hammerers::Hammerers(int points) :
            Dispossessed("Hammerers", 4, g_wounds, 7, 4, false, points),
            m_greatHammer(Weapon::Type::Melee, "Gromril Great Hammer", 1, 2, 3, 3, -1, 1),
            m_greatHammerKeeper(Weapon::Type::Melee, "Gromril Great Hammer", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HAMMERERS};
        m_weapons = {&m_greatHammer, &m_greatHammerKeeper};
    }

    bool Hammerers::configure(int numModels, bool standardBearer, bool musician) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto keeper = new Model(g_basesize, wounds());
        keeper->addMeleeWeapon(&m_greatHammerKeeper);
        addModel(keeper);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_greatHammer);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (musician) {
                model->setName(Model::Musician);
                musician = false;
            }
            addModel(model);
        }

        return true;
    }

    Unit *Hammerers::Create(const ParameterList &parameters) {
        auto unit = new Hammerers(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Musician"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Hammerers", factoryMethod);
        }
    }

    bool Hammerers::battleshockRequired() const {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0);
        for (auto ip : units) {
            // Kingsguard
            if (ip->hasKeyword(DISPOSSESSED) && ip->hasKeyword(HERO))
                return false;
        }
        return true;
    }

    int Hammerers::rollRunDistance() {
        // Sound the Advance
        if (isNamedModelAlive(Model::Musician)) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Hammerers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (isNamedModelAlive(Model::StandardBearer)) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Hammerers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Dispossessed
