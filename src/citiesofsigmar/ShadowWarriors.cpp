/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/ShadowWarriors.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 330;

    bool ShadowWarriors::s_registered = false;

    Unit *ShadowWarriors::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new ShadowWarriors(city, numModels, ComputePoints(parameters));
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Shadow Warriors", factoryMethod);
        }
    }

    ShadowWarriors::ShadowWarriors(City city, int numModels, int points) :
            CitizenOfSigmar(city, "Shadow Warriors", 6, g_wounds, 6, 5, false, points),
            m_bow(Weapon::Type::Missile, "Ranger Bow", 18, 1, 3, 4, -1, 1),
            m_blade(Weapon::Type::Melee, "Coldsteel Blade", 1, 2, 3, 4, 0, 1),
            m_bowWalker(Weapon::Type::Missile, "Ranger Bow", 18, 1, 2, 4, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, SHADOW_WARRIORS};
        m_weapons = {&m_bow, &m_blade, &m_bowWalker};

        // Add the Walker
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_bowWalker);
        bossModel->addMeleeWeapon(&m_blade);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bow);
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }
    }

    int ShadowWarriors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar