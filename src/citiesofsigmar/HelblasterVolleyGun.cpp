/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/HelblasterVolleyGun.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool HelblasterVolleyGun::s_registered = false;

    Unit *HelblasterVolleyGun::Create(const ParameterList &parameters) {
        auto unit = new HelblasterVolleyGun();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        unit->configure();
        return unit;
    }

    std::string HelblasterVolleyGun::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int HelblasterVolleyGun::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void HelblasterVolleyGun::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HelblasterVolleyGun::Create,
                    HelblasterVolleyGun::ValueToString,
                    HelblasterVolleyGun::EnumStringToInt,
                    HelblasterVolleyGun::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Helblaster Volley Gun", factoryMethod);
        }
    }

    HelblasterVolleyGun::HelblasterVolleyGun() :
            CitizenOfSigmar("Helblaster Volley Gun", 3, g_wounds, 5, 4, false, g_pointsPerUnit),
            m_volley1(Weapon::Type::Missile, "Volley of Shots (1 Deck)", 24, RAND_D6, 4, 3, -1, 1),
            m_volley2(Weapon::Type::Missile, "Volley of Shots (2 Decks)", 24, RAND_2D6, 4, 3, -1, 1),
            m_volley3(Weapon::Type::Missile, "Volley of Shots (3 Decks)", 24, RAND_3D6, 4, 3, -1, 1),
            m_crewsTools(Weapon::Type::Melee, "Crew's Tools", 1, 3, 5, 5, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, HELBLASTER_VOLLEY_GUN};
        m_weapons = {&m_volley1, &m_volley2, &m_volley3, &m_crewsTools};
        m_battleFieldRole = Role::Artillery;
    }

    bool HelblasterVolleyGun::configure() {
        auto model = new Model(g_basesize, wounds());

        // TODO: allow selection of 1, 2 or 3 decks
        model->addMissileWeapon(&m_volley1);
        model->addMeleeWeapon(&m_crewsTools);

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int HelblasterVolleyGun::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Point Blank
        if (weapon->isMissile() && (distanceTo(target) <= 12.0)) mod++;

        return mod;
    }

    int HelblasterVolleyGun::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar