/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "tzeentch/MagisterOnDisc.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool MagisterOnDiscOfTzeentch::s_registered = false;

    Unit *MagisterOnDiscOfTzeentch::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfFate[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_arcaniteCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaniteArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new MagisterOnDiscOfTzeentch(coven, lore, trait, artefact, general);
    }

    void MagisterOnDiscOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MagisterOnDiscOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    MagisterOnDiscOfTzeentch::ComputePoints,
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
            s_registered = UnitFactory::Register("Magister on Disc of Tzeentch", factoryMethod);
        }
    }

    MagisterOnDiscOfTzeentch::MagisterOnDiscOfTzeentch(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            TzeentchBase(coven, "Magister on Disc of Tzeentch", 16, g_wounds, 7, 5, true, g_pointsPerUnit),
            m_staff(Weapon::Type::Missile, "Tzeentchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
            m_sword(Weapon::Type::Melee, "Warpsteel Sword", 1, 1, 4, 4, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, MAGISTER};
        m_weapons = {&m_staff,
                     &m_sword,
                     &m_teethAndHorns};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_teethAndHorns.setMount(true);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateBoltOfChange(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int MagisterOnDiscOfTzeentch::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch