/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Poxbringer.h"

namespace Nurgle {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool PoxbringerHeraldOfNurgle::s_registered = false;

    Unit *PoxbringerHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new PoxbringerHeraldOfNurgle();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void PoxbringerHeraldOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    PoxbringerHeraldOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    PoxbringerHeraldOfNurgle::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Poxbringer, Herald of Nurgle", factoryMethod);
        }
    }

    PoxbringerHeraldOfNurgle::PoxbringerHeraldOfNurgle() :
            NurgleBase("Poxbringer, Herald of Nurgle", 4, WOUNDS, 10, 4, false),
            m_balesword(Weapon::Type::Melee, "Balesword", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, POXBRINGER, WIZARD, HERALD_OF_NURGLE};
        m_weapons = {&m_balesword};
    }

    bool PoxbringerHeraldOfNurgle::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_balesword);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds PoxbringerHeraldOfNurgle::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    int PoxbringerHeraldOfNurgle::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
