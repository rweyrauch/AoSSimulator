/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/ChameleonSkinks.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 360;

    bool ChameleonSkinks::s_registered = false;

    ChameleonSkinks::ChameleonSkinks(WayOfTheSeraphon way, Constellation constellation, int numModels, int points) :
            SeraphonBase("Chameleon Skinks", 8, g_wounds, 5, 6, false, points) {
        m_keywords = {ORDER, SERAPHON, SKINKS, CHAMELEON_SKINKS};
        m_weapons = {&m_dartpipe, &m_dagger};

        setWayOfTheSeraphon(way, constellation);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_dartpipe);
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }
    }

    Unit *ChameleonSkinks::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new ChameleonSkinks(way, constellation, numModels, ComputePoints(parameters));
    }

    void ChameleonSkinks::Init() {
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

            s_registered = UnitFactory::Register("Chameleon Skinks", factoryMethod);
        }
    }

    int ChameleonSkinks::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds ChameleonSkinks::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Star-venom
        if ((hitRoll == 6) && (weapon->name() == m_dartpipe.name())) {
            return {0, 1};
        }
        return SeraphonBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
