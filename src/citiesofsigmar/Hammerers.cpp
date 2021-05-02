/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Hammerers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 360;

    bool Hammerers::s_registered = false;

    Hammerers::Hammerers(int points) :
            CitizenOfSigmar("Hammerers", 4, g_wounds, 7, 4, false, points),
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

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

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
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    Hammerers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Musician"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
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

    int Hammerers::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Musician)) mod++;
        return mod;
    }

    int Hammerers::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Musician)) mod++;
        return mod;
    }

    int Hammerers::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Hammerers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
