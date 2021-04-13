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
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 260;

    bool DreadscytheHarridans::s_registered = false;

    DreadscytheHarridans::DreadscytheHarridans() :
            Nighthaunt("Dreadscythe Harridans", 8, g_wounds, 10, 4, true),
            m_scythedLimbs(Weapon::Type::Melee, "Scythed Limbs", 1, 3, 4, 3, -1, 1),
            m_scythedLimbsCrone(Weapon::Type::Melee, "Scythed Limbs", 1, 4, 4, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, DREADSCYTHE_HARRIDANS};
        m_weapons = {&m_scythedLimbs, &m_scythedLimbsCrone};
    }

    bool DreadscytheHarridans::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto crone = new Model(g_basesize, wounds());
        crone->addMeleeWeapon(&m_scythedLimbsCrone);
        addModel(crone);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_scythedLimbs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DreadscytheHarridans::Create(const ParameterList &parameters) {
        auto unit = new DreadscytheHarridans();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Wounds
    DreadscytheHarridans::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Murderous Bloodlust
        if ((woundRoll == 6) && (weapon->name() == m_scythedLimbs.name())) {
            return {2, 0};
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int DreadscytheHarridans::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Nighthaunt::targetHitModifier(weapon, attacker);

        // Harrowing Shriek
        if ((distanceTo(attacker) <= 3.0) && (attacker->bravery() < 6)) {
            modifier -= 1;
        }

        return modifier;
    }

    int DreadscytheHarridans::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nighthaunt
