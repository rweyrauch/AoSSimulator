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
    static const int g_pointsPerUnit = 140;

    bool SpoilpoxScrivenerHeraldOfNurgle::s_registered = false;

    Unit *SpoilpoxScrivenerHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new SpoilpoxScrivenerHeraldOfNurgle();

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
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Spoilpox Scrivener, Herald of Nurgle", factoryMethod);
        }
    }

    SpoilpoxScrivenerHeraldOfNurgle::SpoilpoxScrivenerHeraldOfNurgle() :
            NurgleBase("Spoilpox Scrivener, Herald of Nurgle", 4, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_sneeze(Weapon::Type::Missile, "Disgusting Sneeze", 6, RAND_D6, 2, 4, 0, 1),
            m_maw(Weapon::Type::Melee, "Distended Maw", 2, 2, 3, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SPOILPOX_SCRIVENER, HERALD_OF_NURGLE};
        m_weapons = {&m_sneeze, &m_maw};
        m_battleFieldRole = Role::Leader;
    }

    SpoilpoxScrivenerHeraldOfNurgle::~SpoilpoxScrivenerHeraldOfNurgle() {
    }

    void SpoilpoxScrivenerHeraldOfNurgle::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_sneeze);
        model->addMeleeWeapon(&m_maw);
        addModel(model);
    }

    Wounds SpoilpoxScrivenerHeraldOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int SpoilpoxScrivenerHeraldOfNurgle::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
