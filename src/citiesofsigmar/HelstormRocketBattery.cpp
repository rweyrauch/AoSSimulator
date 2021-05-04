/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/HelstormRocketBattery.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 130;

    bool HelstormRocketBattery::s_registered = false;

    Unit *HelstormRocketBattery::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        return new HelstormRocketBattery(city);
    }

    std::string HelstormRocketBattery::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int HelstormRocketBattery::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void HelstormRocketBattery::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HelstormRocketBattery::Create,
                    HelstormRocketBattery::ValueToString,
                    HelstormRocketBattery::EnumStringToInt,
                    HelstormRocketBattery::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Helstorm Rocket Battery", factoryMethod);
        }
    }

    HelstormRocketBattery::HelstormRocketBattery(City city) :
            CitizenOfSigmar(city, "Helstorm Rocket Battery", 3, g_wounds, 5, 4, false, g_pointsPerUnit),
            m_rocketSalvo(Weapon::Type::Missile, "Helstorm Rocket Salvo", 36, 3, 5, 3, -2, RAND_D3),
            m_crewsTools(Weapon::Type::Melee, "Crew's Tools", 1, 3, 5, 5, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, HELSTORM_ROCKET_BATTERY};
        m_weapons = {&m_rocketSalvo, &m_crewsTools};
        m_battleFieldRole = Role::Artillery;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_rocketSalvo);
        model->addMeleeWeapon(&m_crewsTools);
        addModel(model);
    }

    int HelstormRocketBattery::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Rocket Salvo
        mod++; // Always targeting the same unit with each attack

        return mod;
    }

    Rerolls HelstormRocketBattery::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Calculated Trajectory
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto unit : units) {
            if (unit->hasKeyword(ENGINEER) && unit->hasKeyword(IRONWELD_ARSENAL)) return Rerolls::Ones;
        }
        return CitizenOfSigmar::toHitRerolls(weapon, target);
    }

    int HelstormRocketBattery::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar
