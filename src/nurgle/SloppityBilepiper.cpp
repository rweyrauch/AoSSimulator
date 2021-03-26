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
    static const int g_pointsPerUnit = 90;

    bool SloppityBilepiperHeraldOfNurgle::s_registered = false;

    Unit *SloppityBilepiperHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new SloppityBilepiperHeraldOfNurgle();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        unit->configure();
        return unit;
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

    SloppityBilepiperHeraldOfNurgle::SloppityBilepiperHeraldOfNurgle() :
            NurgleBase("Sloppity Bilepiper, Herald of Nurgle", 4, g_wounds, 10, 4, false),
            m_marotter(Weapon::Type::Melee, "Marotter", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SLOPPITY_BILEPIPER, HERALD_OF_NURGLE};
        m_weapons = {&m_marotter};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod,
                                   &m_diseaseOfMirthSlot);
        s_globalChargeReroll.connect(this, &SloppityBilepiperHeraldOfNurgle::jollyGutpipesChargeReroll,
                                     &m_jollyGutpipesSlot);
    }

    SloppityBilepiperHeraldOfNurgle::~SloppityBilepiperHeraldOfNurgle() {
        m_diseaseOfMirthSlot.disconnect();
        m_jollyGutpipesSlot.disconnect();
    }

    bool SloppityBilepiperHeraldOfNurgle::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_marotter);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds SloppityBilepiperHeraldOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod(const Unit *unit) {
        if (isFriendly(unit)) {
            if ((unit->hasKeyword(NURGLE) && unit->hasKeyword(DAEMON)) && (distanceTo(unit) <= 7.0)) return 1;
        } else if (distanceTo(unit) <= 7.0) {
            return -1;
        }

        return 0;
    }

    Rerolls SloppityBilepiperHeraldOfNurgle::jollyGutpipesChargeReroll(const Unit *unit) {
        if ((unit->hasKeyword(NURGLINGS) || unit->hasKeyword(GREAT_UNCLEAN_ONE)) && (distanceTo(unit) <= 7.0))
            return Rerolls::Ones;

        return Rerolls::None;
    }

    int SloppityBilepiperHeraldOfNurgle::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
