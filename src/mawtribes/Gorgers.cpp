/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Gorgers.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 240;

    bool Gorgers::s_registered = false;

    bool Gorgers::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Gorgers::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new Gorgers(tribe, numModels, ComputePoints(parameters));
        }
        return nullptr;
    }

    void Gorgers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gorgers::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Gorgers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Gorgers", factoryMethod);
        }
    }

    Gorgers::Gorgers(Mawtribe tribe, int numModels, int points) :
            MawtribesBase(tribe, "Gorgers", 6, g_wounds, 8, 6, false, points) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, GORGERS};
        m_weapons = {&m_claws, &m_jaw};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_claws);
            model->addMeleeWeapon(&m_jaw);
            addModel(model);
        }
    }

    int Gorgers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OgorMawtribes
