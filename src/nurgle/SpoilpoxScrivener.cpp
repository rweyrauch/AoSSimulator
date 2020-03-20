/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/SpoilpoxScrivener.h"

namespace Nurgle {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    bool SpoilpoxScrivenerHeraldOfNurgle::s_registered = false;

    Unit *SpoilpoxScrivenerHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new SpoilpoxScrivenerHeraldOfNurgle();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SpoilpoxScrivenerHeraldOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpoilpoxScrivenerHeraldOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    SpoilpoxScrivenerHeraldOfNurgle::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Spoilpox Scrivener, Herald of Nurgle", factoryMethod);
        }
    }

    SpoilpoxScrivenerHeraldOfNurgle::SpoilpoxScrivenerHeraldOfNurgle() :
            NurgleBase("Spoilpox Scrivener, Herald of Nurgle", 4, WOUNDS, 10, 4, false),
            m_sneeze(Weapon::Type::Missile, "Disgusting Sneeze", 6, RAND_D6, 3, 4, 0, 1),
            m_maw(Weapon::Type::Melee, "Distended Maw", 2, 2, 3, 4, -1, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SPOILPOX_SCRIVENER, HERALD_OF_NURGLE};
        m_weapons = {&m_sneeze, &m_maw};

        s_globalChargeReroll.connect(this, &SpoilpoxScrivenerHeraldOfNurgle::keepCountingChargeRerolls,
                                     &m_keepCountingChargeSlot);
        s_globalToHitReroll.connect(this, &SpoilpoxScrivenerHeraldOfNurgle::keepCountingToHitRerolls,
                                    &m_keepCountingToHitSlot);
    }

    SpoilpoxScrivenerHeraldOfNurgle::~SpoilpoxScrivenerHeraldOfNurgle() {
        m_keepCountingChargeSlot.disconnect();
        m_keepCountingToHitSlot.disconnect();
    }

    bool SpoilpoxScrivenerHeraldOfNurgle::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_sneeze);
        model->addMeleeWeapon(&m_maw);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds SpoilpoxScrivenerHeraldOfNurgle::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    Rerolls SpoilpoxScrivenerHeraldOfNurgle::keepCountingChargeRerolls(const Unit *unit) {
        if ((unit->hasKeyword(PLAGUEBEARER) || unit->hasKeyword(PLAGUEBEARERS)) && (distanceTo(unit) <= 7.0f))
            return RerollOnes;

        return NoRerolls;
    }

    Rerolls SpoilpoxScrivenerHeraldOfNurgle::keepCountingToHitRerolls(const Unit *attacker, const Weapon *weapon,
                                                                      const Unit *target) {
        if ((attacker->hasKeyword(PLAGUEBEARER) || attacker->hasKeyword(PLAGUEBEARERS)) &&
            (distanceTo(attacker) <= 7.0f))
            return RerollOnes;

        return NoRerolls;
    }

    int SpoilpoxScrivenerHeraldOfNurgle::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
