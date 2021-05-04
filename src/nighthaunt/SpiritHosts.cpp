/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/SpiritHosts.h>
#include <UnitFactory.h>
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 50;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 480;


    bool SpiritHosts::s_registered = false;

    SpiritHosts::SpiritHosts(Procession procession, int numModels, int points) :
            Nighthaunt(procession, "Spirit Hosts", 6, g_wounds, 10, 4, true, points),
            m_spectralClawsAndDaggars(Weapon::Type::Melee, "Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, SPIRIT_HOSTS};
        m_weapons = {&m_spectralClawsAndDaggars};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spectralClawsAndDaggars);
            addModel(model);
        }
    }

    Unit *SpiritHosts::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SpiritHosts(procession, numModels, ComputePoints(parameters));
    }

    void SpiritHosts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiritHosts::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    SpiritHosts::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Spirit Hosts", factoryMethod);
        }
    }

    Wounds SpiritHosts::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((woundRoll == 6) && (weapon->name() == m_spectralClawsAndDaggars.name())) {
            return {0, 1};
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SpiritHosts::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Nighthaunt
