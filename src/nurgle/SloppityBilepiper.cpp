/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/SloppityBilepiper.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 150;

    bool SloppityBilepiperHeraldOfNurgle::s_registered = false;

    Unit *SloppityBilepiperHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new SloppityBilepiperHeraldOfNurgle(legion, trait, artefact, general);
    }

    void SloppityBilepiperHeraldOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SloppityBilepiperHeraldOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    SloppityBilepiperHeraldOfNurgle::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Sloppity Bilepiper, Herald of Nurgle", factoryMethod);
        }
    }

    SloppityBilepiperHeraldOfNurgle::SloppityBilepiperHeraldOfNurgle(PlagueLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            NurgleBase(legion, "Sloppity Bilepiper, Herald of Nurgle", 4, g_wounds, 10, 5, false, g_pointsPerUnit),
            m_marotter(Weapon::Type::Melee, "Marotter", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SLOPPITY_BILEPIPER, HERALD_OF_NURGLE};
        m_weapons = {&m_marotter};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod, &m_diseaseOfMirthSlot);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_marotter);
        addModel(model);
    }

    SloppityBilepiperHeraldOfNurgle::~SloppityBilepiperHeraldOfNurgle() {
        m_diseaseOfMirthSlot.disconnect();
    }

    Wounds SloppityBilepiperHeraldOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        int numSixes = 0;
        return ignoreWounds(wounds, 5, numSixes);
    }

    int SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod(const Unit *unit) {
        if (isFriendly(unit)) {
            if ((unit->hasKeyword(NURGLE) && unit->hasKeyword(DAEMON)) && (distanceTo(unit) <= 7.0)) return 1;
        } else if (distanceTo(unit) <= 14.0) {
            return -1;
        }

        return 0;
    }

    int SloppityBilepiperHeraldOfNurgle::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
