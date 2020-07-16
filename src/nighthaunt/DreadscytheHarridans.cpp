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
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 260;

    bool DreadscytheHarridans::s_registered = false;

    DreadscytheHarridans::DreadscytheHarridans() :
            Nighthaunt("Dreadscythe Harridans", 8, WOUNDS, 10, 4, true),
            m_scythedLimbs(Weapon::Type::Melee, "Scythed Limbs", 1, 3, 4, 3, -1, 1),
            m_scythedLimbsCrone(Weapon::Type::Melee, "Scythed Limbs", 1, 4, 4, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, DREADSCYTHE_HARRIDANS};
        m_weapons = {&m_scythedLimbs, &m_scythedLimbsCrone};
    }

    bool DreadscytheHarridans::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto crone = new Model(BASESIZE, wounds());
        crone->addMeleeWeapon(&m_scythedLimbsCrone);
        addModel(crone);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_scythedLimbs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DreadscytheHarridans::Create(const ParameterList &parameters) {
        auto unit = new DreadscytheHarridans();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Dreadscythe Harridans", factoryMethod);
        }
    }

    Wounds
    DreadscytheHarridans::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Murderous Bloodlust
        if ((woundRoll == 6) && (weapon->name() == m_scythedLimbs.name())) {
            return {2, 0};
        }
        return Nighthaunt::weaponDamage(weapon, target, hitRoll, woundRoll);
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Nighthaunt
