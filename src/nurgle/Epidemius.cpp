/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Epidemius.h"

namespace Nurgle {
    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    bool EpidemiusTallymanOfNurgle::s_registered = false;

    Unit *EpidemiusTallymanOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new EpidemiusTallymanOfNurgle();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void EpidemiusTallymanOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EpidemiusTallymanOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    EpidemiusTallymanOfNurgle::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Epidemius, Tallyman of Nurgle", factoryMethod);
        }
    }

    EpidemiusTallymanOfNurgle::EpidemiusTallymanOfNurgle() :
            NurgleBase("Epidemius, Tallyman of Nurgle", 4, WOUNDS, 10, 4, false),
            m_balesword(Weapon::Type::Melee, "Balesword", 1, 3, 3, 3, -1, RAND_D3),
            m_teeth(Weapon::Type::Melee, "Tiny Razor-sharp Teeth", 1, 5, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, EPIDEMIUS, TALLYMAN_OF_NURGLE};
        m_weapons = {&m_balesword, &m_teeth};
    }

    bool EpidemiusTallymanOfNurgle::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_balesword);
        model->addMeleeWeapon(&m_teeth);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds EpidemiusTallymanOfNurgle::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    int EpidemiusTallymanOfNurgle::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
