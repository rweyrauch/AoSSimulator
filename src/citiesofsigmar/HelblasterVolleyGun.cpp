/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/HelblasterVolleyGun.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    bool HelblasterVolleyGun::s_registered = false;

    Unit *HelblasterVolleyGun::Create(const ParameterList &parameters) {
        auto unit = new HelblasterVolleyGun();

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Helblaster Volley Gun", factoryMethod);
        }
    }

    HelblasterVolleyGun::HelblasterVolleyGun() :
            CitizenOfSigmar("Helblaster Volley Gun", 3, WOUNDS, 5, 4, false),
            m_volley1(Weapon::Type::Missile, "Volley of Shots (1 Deck)", 24, RAND_D6, 4, 3, -1, 1),
            m_volley2(Weapon::Type::Missile, "Volley of Shots (2 Decks)", 24, RAND_2D6, 4, 3, -1, 1),
            m_volley3(Weapon::Type::Missile, "Volley of Shots (3 Decks)", 24, RAND_3D6, 4, 3, -1, 1),
            m_crewsTools(Weapon::Type::Melee, "Crew's Tools", 1, 3, 5, 5, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, HELBLASTER_VOLLEY_GUN};
        m_weapons = {&m_volley1, &m_volley2, &m_volley3, &m_crewsTools};
    }

    bool HelblasterVolleyGun::configure() {
        auto model = new Model(BASESIZE, wounds());

        // TODO: allow selection of 1, 2 or 3 decks
        model->addMissileWeapon(&m_volley1);
        model->addMeleeWeapon(&m_crewsTools);

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int HelblasterVolleyGun::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Point Blank
        if (weapon->isMissile() && (distanceTo(target) <= 12.0f)) mod++;

        return mod;
    }

    int HelblasterVolleyGun::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace CitiesOfSigmar