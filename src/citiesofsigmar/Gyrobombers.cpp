/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Gyrobombers.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 210;

    bool Gyrobombers::s_registered = false;

    Unit *Gyrobombers::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new Gyrobombers(city, numModels, ComputePoints(parameters));
    }

    std::string Gyrobombers::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Gyrobombers::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Gyrobombers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gyrobombers::Create,
                    Gyrobombers::ValueToString,
                    Gyrobombers::EnumStringToInt,
                    Gyrobombers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Gyrobombers", factoryMethod);
        }
    }

    Gyrobombers::Gyrobombers(City city, int numModels, int points) :
            CitizenOfSigmar(city, "Gyrobombers", 12, g_wounds, 6, 4, true, points),
            m_clattergun(Weapon::Type::Missile, "Clattergun", 20, 4, 4, 3, -1, 1),
            m_rotorBlades(Weapon::Type::Melee, "Rotor Blades", 1, RAND_D3, 5, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, GYROBOMBERS};
        m_weapons = {&m_clattergun, &m_rotorBlades};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_clattergun);
            model->addMeleeWeapon(&m_rotorBlades);
            addModel(model);
        }
    }

    int Gyrobombers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar