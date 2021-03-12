/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Roster.h>
#include <ironjawz/Ironjawz.h>
#include <magic_enum.hpp>
#include <Board.h>

#include "ironjawz/OrrukArdboys.h"
#include "ironjawz/OrrukBrutes.h"
#include "ironjawz/GoreGruntas.h"
#include "ironjawz/OrrukMegaboss.h"
#include "ironjawz/OrrukWarchanter.h"
#include "ironjawz/Gordrakk.h"
#include "ironjawz/MegabossOnMawKrusha.h"
#include "ironjawz/IronskullsBoyz.h"
#include "ironjawz/WeirdnobShaman.h"
#include "ironjawz/MorgoksKrushas.h"

namespace Ironjawz {

    int Ironjawz::chargeModifier() const {
        int mod = Unit::chargeModifier();

        // Eager for Battle
        mod++;

        // Hunt and Crush
        if (m_warclan == Warclan::Bloodtoofs) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::runModifier() const {
        int mod = Unit::runModifier();

        // Hunt and Crush
        if (m_warclan == Warclan::Bloodtoofs) {
            mod++;
        }
        return mod;
    }

    void Ironjawz::setWarclan(Warclan warclan) {
        removeKeyword(IRONSUNZ);
        removeKeyword(BLOODTOOFS);
        removeKeyword(CHOPPAS);

        m_warclan = warclan;
        switch (warclan) {
            case Warclan::Ironsunz:
                addKeyword(IRONSUNZ);
                break;
            case Warclan::Bloodtoofs:
                addKeyword(BLOODTOOFS);
                break;
            case Warclan::Da_Choppas:
                addKeyword(CHOPPAS);
                break;
            default:
                break;
        }
    }

    std::string Ironjawz::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Warclan") {
            auto clanName = magic_enum::enum_name((Warclan) parameter.intValue);
            return std::string(clanName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        }
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Warbeat") {
            auto beatName = magic_enum::enum_name((Warbeat) parameter.intValue);
            return std::string(beatName);
        }

        return ParameterValueToString(parameter);
    }

    int Ironjawz::EnumStringToInt(const std::string &enumString) {
        auto clan = magic_enum::enum_cast<Warclan>(enumString);
        if (clan.has_value()) return (int) clan.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int) mount.value();

        auto beat = magic_enum::enum_cast<Warbeat>(enumString);
        if (beat.has_value()) return (int) beat.value();

        return 0;
    }

    void Ironjawz::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if (isGeneral() && (battleRound == 1) && (m_commandTrait == CommandTrait::Right_Fist_Of_Dakkbad)) {
            m_roster->addCommandPoints(1);
        }
        if (isGeneral() && (battleRound == 1) && (m_commandTrait == CommandTrait::Dead_Kunnin)) {
            getRoster()->addCommandPoints(Dice::RollD3());
        }
    }

    void Ironjawz::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Ironjawz::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Ironjawz::onCharged() {
        Unit::onCharged();

        if (isGeneral() && (m_commandTrait == CommandTrait::Hulking_Muscle_Bound_Brute)) {
            if (meleeTarget() && distanceTo(meleeTarget()) < 1.0) {
                if (Dice::RollD6() >= 2) {
                    meleeTarget()->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                }
            }
        }
    }

    Rerolls Ironjawz::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (charged() && (m_commandTrait == CommandTrait::Live_To_Fight)) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int Ironjawz::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Ironclad)) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Master_Of_The_Weird)) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Master_Of_The_Weird)) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        if (hasKeyword(CHOPPAS)) {
            auto general = dynamic_cast<Ironjawz *>(getRoster()->getGeneral());
            if (general && (distanceTo(general) < 18.0) && (general->m_commandTrait == CommandTrait::Checked_Out)) {
                mod += 2;
            }
        }
        return mod;
    }

    int Ironjawz::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (m_mountTrait == MountTrait::Big_Un) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (m_mountTrait == MountTrait::Fast_Un) {
            mod += 2;
        }
        return mod;
    }

    void Ironjawz::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (!m_usedLoudUn) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (!units.empty()) {
                for (auto unit : units) {
                    unit->buffModifier(Attribute::To_Hit_Melee, -1, {Phase::Combat, m_battleRound, owningPlayer()});
                }
                m_usedLoudUn = true;
            }
        }
    }

    Wounds Ironjawz::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if ((m_mountTrait == MountTrait::Weird_Un) && (wounds.source == Wounds::Source::Spell)) {
            if (Dice::RollD6() >= 4) {
                totalWounds.mortal = 0;
                totalWounds.normal = 0;
            }
        }
        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    void Init() {
        OrrukArdboys::Init();
        OrrukBrutes::Init();
        OrrukGoreGruntas::Init();
        OrrukMegaboss::Init();
        OrrukWarchanter::Init();
        GordrakkTheFistOfGork::Init();
        MegabossOnMawKrusha::Init();
        IronskullsBoyz::Init();
        OrrukWeirdnobShaman::Init();
        MorgoksKrushas::Init();
    }

} // namespace Ironjawz

