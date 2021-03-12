/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildCrossbowmen.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool FreeguildCrossbowmen::s_registered = false;

    Unit *FreeguildCrossbowmen::Create(const ParameterList &parameters) {
        auto unit = new FreeguildCrossbowmen();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool piper = GetBoolParam("Piper", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, piper);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildCrossbowmen::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildCrossbowmen::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildCrossbowmen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FreeguildCrossbowmen::Create,
                    FreeguildCrossbowmen::ValueToString,
                    FreeguildCrossbowmen::EnumStringToInt,
                    FreeguildCrossbowmen::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Piper"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Crossbowmen", factoryMethod);
        }
    }

    FreeguildCrossbowmen::FreeguildCrossbowmen() :
            CitizenOfSigmar("Freeguild Crossbowmen", 5, g_wounds, 5, 6, false),
            m_crossbow(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 4, 3, 0, 1),
            m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
            m_crossbowMarksman(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 3, 3, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_CROSSBOWMEN};
        m_weapons = {&m_crossbow, &m_dagger, &m_crossbowMarksman};
        m_battleFieldRole = Role::Battleline;
    }

    bool FreeguildCrossbowmen::configure(int numModels, bool standardBearer, bool piper) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Marksman
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_crossbowMarksman);
        bossModel->addMeleeWeapon(&m_dagger);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_dagger);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (piper) {
                model->setName(Model::Piper);
                piper = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildCrossbowmen::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Piper)) mod++;
        return mod;
    }

    int FreeguildCrossbowmen::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Piper)) mod++;
        return mod;
    }

    int FreeguildCrossbowmen::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int
    FreeguildCrossbowmen::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extras = CitizenOfSigmar::extraAttacks(attackingModel, weapon, target);
        // Reload, Fire!
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        if (!m_moved && units.empty()) {
            extras++;
        }
        return extras;
    }

    int FreeguildCrossbowmen::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar