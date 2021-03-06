/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/MyrmournBanshees.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 4;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 210;

    bool MyrmournBanshees::s_registered = false;

    Unit *MyrmournBanshees::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new MyrmournBanshees(procession, numModels, ComputePoints(parameters));
    }

    int MyrmournBanshees::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void MyrmournBanshees::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MyrmournBanshees::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    MyrmournBanshees::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Myrmourn Banshees", factoryMethod);
        }
    }

    MyrmournBanshees::MyrmournBanshees(Procession procession, int numModels, int points) :
            Nighthaunt(procession, "Myrmourn Banshees", 8, g_wounds, 10, 4, true, points),
            m_dagger(Weapon::Type::Melee, "Chill Dagger", 1, 1, 4, 3, -2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, MYRMOURN_BANSHEES};
        m_weapons = {&m_dagger};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }
    }

} // namespace Nighthaunt

