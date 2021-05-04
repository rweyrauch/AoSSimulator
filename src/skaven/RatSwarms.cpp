/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatSwarms.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 8;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 240;

    bool RatSwarms::s_registered = false;

    Unit *RatSwarms::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new RatSwarms(numModels, ComputePoints(parameters));
    }

    int RatSwarms::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void RatSwarms::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Rat Swarms", factoryMethod);
        }
    }

    RatSwarms::RatSwarms(int numModels, int points) :
            Skaventide("Rat Swarms", 6, g_wounds, 10, NoSave, false, points),
            m_teeth(Weapon::Type::Melee, "Gnawing Teeth", 1, 5, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, PACK, RAT_SWARMS};
        m_weapons = {&m_teeth};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_teeth);
            addModel(model);
        }
    }
} //namespace Skaven
