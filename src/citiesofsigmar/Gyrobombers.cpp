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
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 210;

    bool Gyrobombers::s_registered = false;

    Unit *Gyrobombers::Create(const ParameterList &parameters) {
        auto unit = new Gyrobombers();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Gyrobombers", factoryMethod);
        }
    }

    Gyrobombers::Gyrobombers() :
            CitizenOfSigmar("Gyrobombers", 12, WOUNDS, 6, 4, true),
            m_clattergun(Weapon::Type::Missile, "Clattergun", 20, 4, 4, 3, -1, 1),
            m_rotorBlades(Weapon::Type::Melee, "Rotor Blades", 1, RAND_D3, 5, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, GYROBOMBERS};
        m_weapons = {&m_clattergun, &m_rotorBlades};
    }

    bool Gyrobombers::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_clattergun);
            model->addMeleeWeapon(&m_rotorBlades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Gyrobombers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar