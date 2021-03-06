/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/TheChangeling.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool TheChangeling::s_registered = false;

    Unit *TheChangeling::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new TheChangeling(coven, lore, trait, artefact, general);
    }

    void TheChangeling::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheChangeling::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TheChangeling::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("The Changeling", factoryMethod);
        }
    }

    TheChangeling::TheChangeling(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            TzeentchBase(coven, "The Changeling", 6, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "The Trickster's Staff", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, THE_CHANGELING};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int TheChangeling::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch