/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/Praetors.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 155;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 2;

    bool Praetors::s_registered = false;

    Praetors::Praetors(Stormhost stormhost, int numModels, int points) :
            StormcastEternal(stormhost, "Annihilators", 4, g_wounds, 7, 2, false, points) {
        m_keywords = {ORDER, STORMCAST_ETERNAL, THUNDERSTRIKE, PRAETORS};
        m_weapons = {&m_halberd, &m_halberdChampion};

        // Add the Champion
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMeleeWeapon(&m_halberdChampion);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_halberd);
            addModel(model);
        }
    }

    Unit *Praetors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);

        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return nullptr;
        }
        return new Praetors(stormhost, numModels, ComputePoints(parameters));
    }

    void Praetors::Init() {
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

            s_registered = UnitFactory::Register("Praetors", factoryMethod);
        }
    }

    int Praetors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
