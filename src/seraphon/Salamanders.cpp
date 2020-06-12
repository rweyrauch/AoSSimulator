/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Salamanders.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 25;
    static const int BASESIZE_SALAMANDER = 60; // x35 oval
    static const int WOUNDS = 1;
    static const int WOUNDS_SALAMANDER = 3;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    bool Salamanders::s_registered = false;

    Salamanders::Salamanders() :
            SeraphonBase("Salamanders", 8, WOUNDS, 5, 4, false),
            m_streamOfFire(Weapon::Type::Missile, "Stream of Fire", 12, 4, 3, 3, -2, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Burning Jaws", 1, 3, 3, 3, -2, RAND_D3),
            m_goad(Weapon::Type::Melee, "Celestite Goad", 1, 1, 4, 5, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, SALAMANDER, HUNTING_PACK};
        m_weapons = {&m_streamOfFire, &m_jaws, &m_goad};
    }

    bool Salamanders::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto salamander = new Model(BASESIZE_SALAMANDER, WOUNDS_SALAMANDER);
        salamander->addMissileWeapon(&m_streamOfFire);
        salamander->addMeleeWeapon(&m_jaws);
        addModel(salamander);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_goad);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Salamanders::Create(const ParameterList &parameters) {
        auto unit = new Salamanders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace Seraphon
