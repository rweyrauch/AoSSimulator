/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "tzeentch/TheEyesOfNine.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool TheEyesOfTheNine::s_registered = false;

    Unit *TheEyesOfTheNine::Create(const ParameterList &parameters) {
        return new TheEyesOfTheNine();
    }

    int TheEyesOfTheNine::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void TheEyesOfTheNine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheEyesOfTheNine::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TheEyesOfTheNine::ComputePoints,
                    {
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("The Eyes of the Nine", factoryMethod);
        }
    }

    TheEyesOfTheNine::TheEyesOfTheNine() :
            TzeentchBase(ChangeCoven::Cult_Of_The_Transient_Form, "The Eyes of the Nine", 6, g_wounds, 6, 6, false, g_pointsPerUnit),
            m_flames(Weapon::Type::Missile, "Magical Flames", 12, 2, 5, 4, 0, 1),
            m_bolt(Weapon::Type::Missile, "Sorcerous Bolt", 12, 1, 5, 4, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
            m_blade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 4, 0, 1),
            m_beak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
            m_hands(Weapon::Type::Melee, "Taloned Hands", 1, 1, 5, 4, 0, 1) {
        m_keywords = {CHAOS, TZEENTCH, CULT_OF_THE_TRANSIENT_FORM, EYES_OF_THE_NINE};
        m_weapons = {&m_flames, &m_bolt, &m_greatblade, &m_blade, &m_beak, &m_hands};

        auto narvia = new Model(g_basesize, wounds());
        narvia->addMissileWeapon(&m_bolt);
        narvia->addMeleeWeapon(&m_blade);
        narvia->setName("Narvia");
        addModel(narvia);

        auto turosh = new Model(g_basesize, wounds());
        turosh->addMissileWeapon(&m_bolt);
        turosh->addMeleeWeapon(&m_blade);
        turosh->setName("Turosh");
        addModel(turosh);

        auto kcharik = new Model(g_basesize, wounds() + 1);
        kcharik->addMeleeWeapon(&m_greatblade);
        kcharik->addMeleeWeapon(&m_beak);
        kcharik->setName("K'charik");
        addModel(kcharik);

        auto horror = new Model(g_basesize, wounds());
        horror->addMissileWeapon(&m_flames);
        horror->addMeleeWeapon(&m_hands);
        horror->setName("Blue Horror");
        addModel(horror);
    }

    Wounds TheEyesOfTheNine::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = TzeentchBase::applyWoundSave(wounds, attackingUnit);

        // Arcanite Shield
        if (isNamedModelAlive("Narvia") || isNamedModelAlive("Turosh")) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
        }
        return totalWounds;
    }
} // namespace Tzeentch


