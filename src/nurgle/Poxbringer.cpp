/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Poxbringer.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool PoxbringerHeraldOfNurgle::s_registered = false;

    Unit *PoxbringerHeraldOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new PoxbringerHeraldOfNurgle();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        bool ok = unit->configure(lore);
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
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Poxbringer, Herald of Nurgle", factoryMethod);
        }
    }

    PoxbringerHeraldOfNurgle::PoxbringerHeraldOfNurgle() :
            NurgleBase("Poxbringer, Herald of Nurgle", 4, g_wounds, 10, 4, false),
            m_balesword(Weapon::Type::Melee, "Balesword", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, POXBRINGER, WIZARD, HERALD_OF_NURGLE};
        m_weapons = {&m_balesword};
        m_battleFieldRole = Leader;
    }

    bool PoxbringerHeraldOfNurgle::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_balesword);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds PoxbringerHeraldOfNurgle::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    int PoxbringerHeraldOfNurgle::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
