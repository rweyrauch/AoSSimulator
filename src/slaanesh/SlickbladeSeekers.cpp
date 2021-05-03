/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SlickbladeSeekers.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 200;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool SlickbladeSeekers::s_registered = false;

    SlickbladeSeekers::SlickbladeSeekers(Host host, int numModels, int points) :
            SlaaneshBase(host, "Slickblade Seekers", 14, g_wounds, 6, 5, false, points),
            m_glaive(Weapon::Type::Melee, "Slickblade Glaive", 2, 3, 3, 3, -1, 1),
            m_glaiveHunter(Weapon::Type::Melee, "Slickblade Glaive", 2, 4, 3, 3, -1, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, SEEKERS, SLICKBLADE_SEEKERS};
        m_weapons = {&m_glaive, &m_glaiveHunter, &m_poisonedTongue};
        m_hasMount = true;
        m_poisonedTongue.setMount(true);

        // Add the High Tempter
        auto tempter = new Model(g_basesize, wounds());
        tempter->addMeleeWeapon(&m_poisonedTongue);
        tempter->addMeleeWeapon(&m_glaiveHunter);
        tempter->setName("Hunter-seeker");
        addModel(tempter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_poisonedTongue);
            model->addMeleeWeapon(&m_glaive);
            addModel(model);
        }
    }

    Unit *SlickbladeSeekers::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SlickbladeSeekers(host, numModels, ComputePoints(parameters));
    }

    void SlickbladeSeekers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Slickblade Seekers", factoryMethod);
        }
    }

    int SlickbladeSeekers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds SlickbladeSeekers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = SlaaneshBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        // Decapitating Strikes
        if ((woundRoll == 6) && (weapon->name() == m_glaive.name())) {
            wounds.mortal++;
        }
        return wounds;
    }

} // namespace Slaanesh