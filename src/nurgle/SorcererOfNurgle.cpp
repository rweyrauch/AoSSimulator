/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/SorcererOfNurgle.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool SorcererOfNurgle::s_registered = false;

    Unit *SorcererOfNurgle::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalRotbringerCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalRotbringerArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new SorcererOfNurgle(legion, lore, trait, artefact, general);
    }

    void SorcererOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SorcererOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    SorcererOfNurgle::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_mortalRotbringerCommandTraits[0],
                                          g_mortalRotbringerCommandTraits),
                            EnumParameter("Artefact", g_mortalRotbringerArtefacts[0], g_mortalRotbringerArtefacts),
                            EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Sorcerer of Nurgle", factoryMethod);
        }
    }

    SorcererOfNurgle::SorcererOfNurgle(PlagueLegion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            NurgleBase(legion, "Sorcerer of Nurgle", 4, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Rotwood Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, WIZARD, SORCERER};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int SorcererOfNurgle::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Nurgle
