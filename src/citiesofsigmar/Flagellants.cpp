/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/Flagellants.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool Flagellants::s_registered = false;

    Unit *Flagellants::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new Flagellants(city, numModels, ComputePoints(parameters));
    }

    std::string Flagellants::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Flagellants::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Flagellants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Flagellants::Create,
                    Flagellants::ValueToString,
                    Flagellants::EnumStringToInt,
                    Flagellants::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Flagellants", factoryMethod);
        }
    }

    Flagellants::Flagellants(City city, int numModels, int points) :
            CitizenOfSigmar(city, "Flagellants", 6, g_wounds, 8, NoSave, false, points),
            m_flailsAndClubs(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 2, 5, 4, 0, 1),
            m_flailsAndClubsProphet(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 3, 5, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, DEVOTED_OF_SIGMAR, FLAGELLANTS};
        m_weapons = {&m_flailsAndClubs, &m_flailsAndClubsProphet};

        // Add the Prophet
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_flailsAndClubsProphet);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_flailsAndClubs);
            addModel(model);
        }
    }

    int Flagellants::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extras = CitizenOfSigmar::extraAttacks(attackingModel, weapon, target);

        // Glorious Martyrs
        if (m_modelsSlain >= 5) {
            extras += 2;
        } else if (m_modelsSlain) {
            extras++;
        }
        return extras;
    }

    int Flagellants::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Fanatical Fury
        if (m_charged) mod++;

        return mod;
    }

    int Flagellants::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toWoundModifier(weapon, target);

        // Fanatical Fury
        if (m_charged) mod++;

        return mod;
    }

    void Flagellants::onFlee(int numFled) {
        // Reckless Abandon
        Dice::RollResult rolls;
        Dice::RollD6(numFled, rolls);
        int numMortalWounds = rolls.rollsGE(4);
        if (numMortalWounds) {
            auto board = Board::Instance();
            auto closestTarget = board->getNearestUnit(this, PlayerId::None);
            if (closestTarget && distanceTo(closestTarget) <= 6.0) {
                closestTarget->applyDamage({0, numMortalWounds}, this);
            }
        }
        CitizenOfSigmar::onFlee(numFled);
    }

    int Flagellants::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar