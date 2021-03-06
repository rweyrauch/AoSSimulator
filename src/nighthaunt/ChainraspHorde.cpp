/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/ChainraspHorde.h>
#include <UnitFactory.h>
#include <iostream>
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool ChainraspHorde::s_registered = false;

    ChainraspHorde::ChainraspHorde(Procession procession, int numModels, int points) :
            Nighthaunt(procession, "Chainrasp Horde", 6, g_wounds, 10, 5, true, points), // todo: bravery 6 when no Dreadwarden
            m_malignantWeapon(Weapon::Type::Melee, "Malignant Weapon", 1, 2, 4, 4, 0, 1),
            m_malignantWeaponWarden(Weapon::Type::Melee, "Malignant Weapon", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, CHAINRASP_HORDE};
        m_weapons = {&m_malignantWeapon, &m_malignantWeaponWarden};
        m_battleFieldRole = Role::Battleline;

        auto warden = new Model(g_basesize, wounds());
        warden->addMeleeWeapon(&m_malignantWeaponWarden);
        addModel(warden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_malignantWeapon);
            addModel(model);
        }
    }

    Unit *ChainraspHorde::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new ChainraspHorde(procession, numModels, ComputePoints(parameters));
    }

    void ChainraspHorde::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChainraspHorde::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ChainraspHorde::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };

            s_registered = UnitFactory::Register("Chainrasp Horde", factoryMethod);
        }
    }

    Rerolls ChainraspHorde::toWoundRerolls(const Weapon *weapon, const Unit *unit) const {
        // Chilling Horde
        if (remainingModels() >= 10) {
            return Rerolls::Ones;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int ChainraspHorde::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nighthaunt
