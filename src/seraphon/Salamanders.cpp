/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Salamanders.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 25;
    static const int BASESIZE_SALAMANDER = 60; // x35 oval
    static const int g_wounds = 1;
    static const int WOUNDS_SALAMANDER = 3;
    static const int g_minUnitSize = 4;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 440;

    bool Salamanders::s_registered = false;

    Salamanders::Salamanders(WayOfTheSeraphon way, Constellation constellation, int numModels, int points) :
            SeraphonBase("Salamanders", 8, g_wounds, 5, 4, false, points) {
        m_keywords = {ORDER, SERAPHON, SKINK, SALAMANDER, HUNTING_PACK};
        m_weapons = {&m_streamOfFire, &m_jaws, &m_goad};

        setWayOfTheSeraphon(way, constellation);
        auto salamander = new Model(BASESIZE_SALAMANDER, WOUNDS_SALAMANDER);
        salamander->addMissileWeapon(&m_streamOfFire);
        salamander->addMeleeWeapon(&m_jaws);
        addModel(salamander);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_goad);
            addModel(model);
        }
    }

    Unit *Salamanders::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new Salamanders(way, constellation, numModels, ComputePoints(parameters));
    }

    void Salamanders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Salamanders", factoryMethod);
        }
    }

    int Salamanders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Seraphon
