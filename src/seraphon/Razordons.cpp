/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Razordons.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 26;
    static const int BASESIZE_RAZORDON = 60; // x35 oval
    static const int g_wounds = 1;
    static const int WOUNDS_RAZORDON = 3;
    static const int g_minUnitSize = 4;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 240;

    bool Razordons::s_registered = false;

    Razordons::Razordons(WayOfTheSeraphon way, Constellation constellation, int numModels, int points) :
            SeraphonBase("Razordons", 8, g_wounds, 5, 4, false, points) {
        m_keywords = {ORDER, SERAPHON, SKINK, RAZORDON, HUNTING_PACK};
        m_weapons = {&m_spikes, &m_tail, &m_goad};

        setWayOfTheSeraphon(way, constellation);

        auto razordon = new Model(BASESIZE_RAZORDON, WOUNDS_RAZORDON);
        razordon->addMissileWeapon(&m_spikes);
        razordon->addMeleeWeapon(&m_tail);
        addModel(razordon);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_goad);
            addModel(model);
        }
    }

    Unit *Razordons::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new Razordons(way, constellation, numModels, ComputePoints(parameters));
    }

    void Razordons::Init() {
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

            s_registered = UnitFactory::Register("Razordons", factoryMethod);
        }
    }

    int Razordons::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Razordons::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Piercing Barbs
        if ((weapon->name() == m_spikes.name()) && (distanceTo(m_shootingTarget) <= 6.0)) {
            return -1;
        }
        return SeraphonBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
