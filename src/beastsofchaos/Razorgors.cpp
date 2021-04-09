/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Razorgors.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 50;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 6;

    bool Razorgors::s_registered = false;

    Razorgors::Razorgors(Greatfray fray, int numModels) :
            BeastsOfChaosBase("Razorgors", 10, g_wounds, 6, 5, false),
            m_tusksAndHooves(Weapon::Type::Melee, "Large Tusks and Hooves", 1, 4, 4, 3, -1, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, RAZORGORS};
        m_weapons = {&m_tusksAndHooves};

        setGreatfray(fray);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Unit *Razorgors::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        return new Razorgors(fray, numModels);
    }

    void Razorgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Razorgors", factoryMethod);
        }
    }

    Wounds Razorgors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        Wounds damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Uncontrollable Stampede
        if (m_charged && (hitRoll == 6)) {
            damage.mortal += 1;
        }
        return damage;
    }

    int Razorgors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos