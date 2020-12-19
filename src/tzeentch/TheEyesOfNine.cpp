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
        auto unit = new TheEyesOfTheNine();

        unit->setChangeCoven(ChangeCoven::Cult_Of_The_Transient_Form);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int TheEyesOfTheNine::ComputePoints(int /*numModels*/) {
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
            TzeentchBase("The Eyes of the Nine", 6, g_wounds, 6, 6, false),
            m_flames(Weapon::Type::Missile, "Magical Flames", 12, 2, 5, 4, 0, 1),
            m_bolt(Weapon::Type::Missile, "Sorcerous Bolt", 12, 1, 5, 4, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
            m_blade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 4, 0, 1),
            m_beak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
            m_hands(Weapon::Type::Melee, "Taloned Hands", 1, 1, 5, 4, 0, 1) {
        m_keywords = {CHAOS, TZEENTCH, CULT_OF_THE_TRANSIENT_FORM, EYES_OF_THE_NINE};
        m_weapons = {&m_flames, &m_bolt, &m_greatblade, &m_blade, &m_beak, &m_hands};
    }

    bool TheEyesOfTheNine::configure() {
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

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds TheEyesOfTheNine::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);

        // Arcanite Shield
        for (auto &model : m_models) {
            if (((model->getName() == "Narvia") || (model->getName() == "Turosh")) &&
                !model->slain() && !model->fled()) {
                Dice::RollResult normalSaves, mortalSaves;
                Dice::RollD6(totalWounds.normal, normalSaves);
                Dice::RollD6(totalWounds.mortal, mortalSaves);
                totalWounds.normal -= normalSaves.rollsGE(6);
                totalWounds.mortal -= mortalSaves.rollsGE(6);
                break;
            }
        }
        return totalWounds;
    }
} // namespace Tzeentch


