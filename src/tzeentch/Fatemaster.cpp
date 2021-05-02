/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fatemaster.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 120;

    bool Fatemaster::s_registered = false;

    Unit *Fatemaster::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_arcaniteCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaniteArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Fatemaster(coven, trait, artefact, general);
    }

    void Fatemaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Fatemaster::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    Fatemaster::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_arcaniteCommandTraits[0], g_arcaniteCommandTraits),
                            EnumParameter("Artefact", g_arcaniteArtefacts[0], g_arcaniteArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fatemaster", factoryMethod);
        }
    }

    Fatemaster::Fatemaster(ChangeCoven coven, CommandTrait trait, Artefact artefact, bool isGeneral) :
            TzeentchBase(coven, "Fatemaster", 16, g_wounds, 8, 4, true, g_pointsPerUnit),
            m_glaive(Weapon::Type::Melee, "Fireglaive of Tzeentch", 2, 3, 3, 4, 0, RAND_D3),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, FATEMASTER};
        m_weapons = {&m_glaive, &m_teethAndHorns};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_teethAndHorns.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);
    }

    int Fatemaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds Fatemaster::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;

        // Soulbound Shield
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::RollD6() >= 4) {
                totalWounds.normal = 0;
                totalWounds.mortal = 0;
            }
        }
        return TzeentchBase::applyWoundSave(totalWounds, attackingUnit);
    }

    int Fatemaster::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = TzeentchBase::toSaveModifier(weapon, attacker);

        // Hovering Disc of Tzeentch
        if (weapon->isMelee() && !attacker->canFly() && !attacker->hasKeyword(MONSTER)) {
            mod += 2;
        }
        return mod;
    }

} // Tzeentch