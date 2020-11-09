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

    Aetherwings::Aetherwings() :
            StormcastEternal("Aetherwings", 12, g_wounds, 6, NoSave, true),
            m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, AETHERWINGS};
        m_weapons = {&m_beakAndClaws};

        // Swooping Hunters
        m_retreatAndCharge = true;
    }

    bool Aetherwings::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Aetherwings::Create(const ParameterList &parameters) {
        auto unit = new Aetherwings();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, ToInteger(Stormhost::None));
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    int Aetherwings::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
