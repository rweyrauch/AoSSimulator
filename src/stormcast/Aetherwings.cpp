/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Aetherwings.h>
#include <UnitFactory.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 40;
    static const int g_pointsMaxUnitSize = 160;

    bool Aetherwings::s_registered = false;

    Aetherwings::Aetherwings(Stormhost stormhost, int numModels, int points) :
            StormcastEternal(stormhost,"Aetherwings", 12, g_wounds, 6, NoSave, true, points),
            m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, AETHERWINGS};
        m_weapons = {&m_beakAndClaws};

        // Swooping Hunters
        m_retreatAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }

        //m_points = computePoints();
    }

    Unit *Aetherwings::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, ToInteger(Stormhost::None));
        return new Aetherwings(stormhost, numModels, ComputePoints(parameters));
    }

    void Aetherwings::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Aetherwings", factoryMethod);
        }
    }

    int Aetherwings::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
