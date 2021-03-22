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

    Salamanders::Salamanders() :
            SeraphonBase("Salamanders", 8, g_wounds, 5, 4, false),
            m_streamOfFire(Weapon::Type::Missile, "Stream of Fire", 12, 4, 3, 3, -2, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Burning Jaws", 1, 3, 3, 3, -2, RAND_D3),
            m_goad(Weapon::Type::Melee, "Celestite Goad", 1, 1, 4, 5, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, SALAMANDER, HUNTING_PACK};
        m_weapons = {&m_streamOfFire, &m_jaws, &m_goad};
    }

    bool Salamanders::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto salamander = new Model(BASESIZE_SALAMANDER, WOUNDS_SALAMANDER);
        salamander->addMissileWeapon(&m_streamOfFire);
        salamander->addMeleeWeapon(&m_jaws);
        addModel(salamander);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_goad);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Salamanders::Create(const ParameterList &parameters) {
        auto unit = new Salamanders();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            BoolParameter("Stardrake Icon"),
                            BoolParameter("Wardrum"),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Salamanders", factoryMethod);
        }
    }

    int Salamanders::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Seraphon
