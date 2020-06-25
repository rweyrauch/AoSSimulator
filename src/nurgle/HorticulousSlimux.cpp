/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/HorticulousSlimux.h"

namespace Nurgle {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 220;

    bool HorticulousSlimux::s_registered = false;

    Unit *HorticulousSlimux::Create(const ParameterList &parameters) {
        auto unit = new HorticulousSlimux();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HorticulousSlimux::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HorticulousSlimux::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    HorticulousSlimux::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Horticulous Slimux", factoryMethod);
        }
    }

    HorticulousSlimux::HorticulousSlimux() :
            NurgleBase("Horticulous Slimux", 5, WOUNDS, 10, 3, false),
            m_shears(Weapon::Type::Melee, "Lopping Shears", 1, 3, 3, 3, -1, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Mulch's Slime-encrusted Jaws", 1, RAND_D3, 3, 3, -2, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, HORTICULOUS_SLIMUX};
        m_weapons = {&m_shears, &m_jaws};

        s_globalChargeReroll.connect(this, &HorticulousSlimux::beastHandlerChargeReroll, &m_beastHandlerChargeSlot);
        s_globalToHitReroll.connect(this, &HorticulousSlimux::beastHandlerToHitRerolls, &m_beastHandlerToHitSlot);
    }

    HorticulousSlimux::~HorticulousSlimux() {
        m_beastHandlerChargeSlot.disconnect();
        m_beastHandlerToHitSlot.disconnect();
    }

    bool HorticulousSlimux::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_shears);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds HorticulousSlimux::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    Rerolls HorticulousSlimux::beastHandlerChargeReroll(const Unit *unit) {
        if (isFriendly(unit) && (distanceTo(unit) <= 7.0)) {
            if (unit->hasKeyword(BEASTS_OF_NURGLE)) return RerollOnes;
        }
        return NoRerolls;
    }

    Rerolls
    HorticulousSlimux::beastHandlerToHitRerolls(const Unit *attacker, const Weapon * /*weapon*/,
                                                const Unit * /*target*/) {
        if (isFriendly(attacker) && (distanceTo(attacker) <= 7.0)) {
            if (attacker->hasKeyword(BEASTS_OF_NURGLE)) return RerollOnes;
        }
        return NoRerolls;
    }

    int HorticulousSlimux::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
