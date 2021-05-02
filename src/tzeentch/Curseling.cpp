/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Curseling.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 160;

    bool CurselingEyeOfTzeentch::s_registered = false;

    Unit *CurselingEyeOfTzeentch::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfFate[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_arcaniteCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaniteArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new CurselingEyeOfTzeentch(coven, lore, trait, artefact, general);
    }

    void CurselingEyeOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CurselingEyeOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    CurselingEyeOfTzeentch::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_arcaniteCommandTraits[0], g_arcaniteCommandTraits),
                            EnumParameter("Artefact", g_arcaniteArtefacts[0], g_arcaniteArtefacts),
                            EnumParameter("Lore", g_loreOfFate[0], g_loreOfFate),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };

            s_registered = UnitFactory::Register("Curseling Eye of Tzeentch", factoryMethod);
        }
    }

    CurselingEyeOfTzeentch::CurselingEyeOfTzeentch(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            TzeentchBase("Curseling Eye of Tzeentch", 5, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, CURSELING};
        m_weapons = {&m_sword, &m_flail, &m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setChangeCoven(coven);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_flail);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int CurselingEyeOfTzeentch::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch