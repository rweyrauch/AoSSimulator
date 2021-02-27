/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/SpoilpoxScrivener.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool SpoilpoxScrivenerHeraldOfNurgle::s_registered = false;

    Unit *SpoilpoxScrivenerHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new SpoilpoxScrivenerHeraldOfNurgle();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

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
                        EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                        EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                        BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Spoilpox Scrivener, Herald of Nurgle", factoryMethod);
        }
    }

    SpoilpoxScrivenerHeraldOfNurgle::SpoilpoxScrivenerHeraldOfNurgle() :
            NurgleBase("Spoilpox Scrivener, Herald of Nurgle", 4, g_wounds, 10, 4, false),
            m_sneeze(Weapon::Type::Missile, "Disgusting Sneeze", 6, RAND_D6, 3, 4, 0, 1),
            m_maw(Weapon::Type::Melee, "Distended Maw", 2, 2, 3, 4, -1, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SPOILPOX_SCRIVENER, HERALD_OF_NURGLE};
        m_weapons = {&m_sneeze, &m_maw};
        m_battleFieldRole = Leader;

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
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_sneeze);
        model->addMeleeWeapon(&m_maw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds SpoilpoxScrivenerHeraldOfNurgle::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    Rerolls SpoilpoxScrivenerHeraldOfNurgle::keepCountingChargeRerolls(const Unit *unit) {
        if ((unit->hasKeyword(PLAGUEBEARER) || unit->hasKeyword(PLAGUEBEARERS)) && (distanceTo(unit) <= 7.0))
            return Reroll_Ones;

        return No_Rerolls;
    }

    Rerolls SpoilpoxScrivenerHeraldOfNurgle::keepCountingToHitRerolls(const Unit *attacker, const Weapon * /*weapon*/,
                                                                      const Unit * /*target*/) {
        if ((attacker->hasKeyword(PLAGUEBEARER) || attacker->hasKeyword(PLAGUEBEARERS)) &&
            (distanceTo(attacker) <= 7.0))
            return Reroll_Ones;

        return No_Rerolls;
    }

    int SpoilpoxScrivenerHeraldOfNurgle::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
