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
    static const int BASESIZE = 25;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 180;

    bool Gyrocopters::s_registered = false;

    Unit *Gyrocopters::Create(const ParameterList &parameters) {
        auto unit = new Gyrocopters();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, BrimstoneGun);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Gyrocopters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == BrimstoneGun) {
                return "Brimstone Gun";
            } else if (parameter.intValue == SteamGun) {
                return "Steam Gun";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Gyrocopters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Brimstone Gun") {
            return BrimstoneGun;
        } else if (enumString == "Steam Gun") {
            return SteamGun;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Gyrocopters::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {BrimstoneGun, SteamGun};
            static FactoryMethod factoryMethod = {
                    Gyrocopters::Create,
                    Gyrocopters::ValueToString,
                    Gyrocopters::EnumStringToInt,
                    Gyrocopters::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", BrimstoneGun, weapons),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Gyrocopters", factoryMethod);
        }
    }

    Gyrocopters::Gyrocopters() :
            CitizenOfSigmar("Gyrocopters", 16, WOUNDS, 6, 4, true),
            m_brimstoneGun(Weapon::Type::Missile, "Brimstone Gun", 16, 3, 3, 3, -1, 1),
            m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, 1, 3, 4, -1, 1),
            m_rotorBlades(Weapon::Type::Melee, "Rotor Blades", 1, RAND_D3, 5, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, GYROCOPTERS};
        m_weapons = {&m_brimstoneGun, &m_steamGun, &m_rotorBlades};
    }

    bool Gyrocopters::configure(int numModels, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == BrimstoneGun) {
                model->addMissileWeapon(&m_brimstoneGun);
            } else if (weapons == SteamGun) {
                model->addMissileWeapon(&m_steamGun);
            }
            model->addMeleeWeapon(&m_rotorBlades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Gyrocopters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar