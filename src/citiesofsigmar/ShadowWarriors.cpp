/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/ShadowWarriors.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    bool ShadowWarriors::s_registered = false;

    Unit *ShadowWarriors::Create(const ParameterList &parameters) {
        auto unit = new ShadowWarriors();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ShadowWarriors::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int ShadowWarriors::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void ShadowWarriors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ShadowWarriors::Create,
                    ShadowWarriors::ValueToString,
                    ShadowWarriors::EnumStringToInt,
                    ShadowWarriors::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Shadow Warriors", factoryMethod);
        }
    }

    ShadowWarriors::ShadowWarriors() :
            CitizenOfSigmar("Shadow Warriors", 6, WOUNDS, 6, 5, false),
            m_bow(Weapon::Type::Missile, "Ranger Bow", 18, 1, 3, 4, -1, 1),
            m_blade(Weapon::Type::Melee, "Coldsteel Blade", 1, 2, 3, 4, 0, 1),
            m_bowWalker(Weapon::Type::Missile, "Ranger Bow", 18, 1, 2, 4, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, SHADOW_WARRIORS};
        m_weapons = {&m_bow, &m_blade, &m_bowWalker};
    }

    bool ShadowWarriors::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the Walker
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMissileWeapon(&m_bowWalker);
        bossModel->addMeleeWeapon(&m_blade);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_bow);
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int ShadowWarriors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar