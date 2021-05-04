/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Gyrocopters.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 180;

    bool Gyrocopters::s_registered = false;

    Unit *Gyrocopters::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Brimstone_Gun);
        return new Gyrocopters(city, numModels, weapons, ComputePoints(parameters));
    }

    std::string Gyrocopters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Brimstone_Gun) {
                return "Brimstone Gun";
            } else if (parameter.intValue == Steam_Gun) {
                return "Steam Gun";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Gyrocopters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Brimstone Gun") {
            return Brimstone_Gun;
        } else if (enumString == "Steam Gun") {
            return Steam_Gun;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Gyrocopters::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Brimstone_Gun, Steam_Gun};
            static FactoryMethod factoryMethod = {
                    Gyrocopters::Create,
                    Gyrocopters::ValueToString,
                    Gyrocopters::EnumStringToInt,
                    Gyrocopters::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Brimstone_Gun, weapons),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Gyrocopters", factoryMethod);
        }
    }

    Gyrocopters::Gyrocopters(City city, int numModels, WeaponOption weapons, int points) :
            CitizenOfSigmar(city, "Gyrocopters", 16, g_wounds, 6, 4, true, points),
            m_brimstoneGun(Weapon::Type::Missile, "Brimstone Gun", 16, 3, 3, 3, -1, 1),
            m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, 1, 3, 4, -1, 1),
            m_rotorBlades(Weapon::Type::Melee, "Rotor Blades", 1, RAND_D3, 5, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, GYROCOPTERS};
        m_weapons = {&m_brimstoneGun, &m_steamGun, &m_rotorBlades};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Brimstone_Gun) {
                model->addMissileWeapon(&m_brimstoneGun);
            } else if (weapons == Steam_Gun) {
                model->addMissileWeapon(&m_steamGun);
            }
            model->addMeleeWeapon(&m_rotorBlades);
            addModel(model);
        }
    }

    int Gyrocopters::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar