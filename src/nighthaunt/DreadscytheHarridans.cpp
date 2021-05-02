/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/DreadscytheHarridans.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 320;

    bool DreadscytheHarridans::s_registered = false;

    DreadscytheHarridans::DreadscytheHarridans(int numModels, int points) :
            Nighthaunt("Dreadscythe Harridans", 8, g_wounds, 10, 4, true, points) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, DREADSCYTHE_HARRIDANS};
        m_weapons = {&m_scythedLimbs, &m_scythedLimbsCrone};

        auto crone = new Model(g_basesize, wounds());
        crone->addMeleeWeapon(&m_scythedLimbsCrone);
        addModel(crone);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_scythedLimbs);
            addModel(model);
        }
    }

    Unit *DreadscytheHarridans::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new DreadscytheHarridans(numModels, ComputePoints(parameters));
    }

    void DreadscytheHarridans::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DreadscytheHarridans::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    DreadscytheHarridans::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Dreadscythe Harridans", factoryMethod);
        }
    }

    int DreadscytheHarridans::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Nighthaunt::targetHitModifier(weapon, attacker);

        // Harrowing Shriek
        if ((distanceTo(attacker) <= 3.0) && (attacker->bravery() < 7)) {
            modifier -= 1;
        }

        return modifier;
    }

    int DreadscytheHarridans::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int DreadscytheHarridans::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Murderous Bloodlust
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_scythedLimbs.name())) {
            return 2;
        }
        return Nighthaunt::generateHits(unmodifiedHitRoll, weapon, unit);
    }

} // namespace Nighthaunt
