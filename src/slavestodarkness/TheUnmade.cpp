/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/TheUnmade.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 9;
    static const int g_maxUnitSize = 36;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool TheUnmade::s_registered = false;

    Unit *TheUnmade::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new TheUnmade(legion, numModels);
    }

    void TheUnmade::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("The Unmade", factoryMethod);
        }
    }

    TheUnmade::TheUnmade(DamnedLegion legion, int numModels) :
            SlavesToDarknessBase("The Unmade", 6, g_wounds, 5, 6, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, THE_UNMADE};
        m_weapons = {&m_maimingWeapons, &m_maimingWeaponsLeader, &m_nigthmareSickles};

        setDamnedLegion(legion);

        s_globalBraveryMod.connect(this, &TheUnmade::frozenInFear, &m_connection);

        auto joyousOne = new Model(g_basesize, wounds());
        joyousOne->addMeleeWeapon(&m_maimingWeaponsLeader);
        joyousOne->setName("Joyous One");
        addModel(joyousOne);

        auto blissfulOne = new Model(g_basesize, wounds());
        blissfulOne->addMeleeWeapon(&m_nigthmareSickles);
        blissfulOne->setName("Blissful One");
        addModel(blissfulOne);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_maimingWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    TheUnmade::~TheUnmade() {
        m_connection.disconnect();
    }

    int TheUnmade::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int TheUnmade::frozenInFear(const Unit *unit) {
        // Frozen in Fear
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0)) {
            return -1;
        }
        return 0;
    }

} //SlavesToDarkness