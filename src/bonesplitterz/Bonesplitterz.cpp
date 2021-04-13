/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <bonesplitterz/Bonesplitterz.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>

#include "bonesplitterz/BoarboyManiaks.h"
#include "bonesplitterz/ManiakWeirdnob.h"
#include "bonesplitterz/OrrukArrowBoys.h"
#include "bonesplitterz/OrrukMorboys.h"
#include "bonesplitterz/SavageBigStabbas.h"
#include "bonesplitterz/SavageBigBoss.h"
#include "bonesplitterz/SavageBoarboys.h"
#include "bonesplitterz/SavageOrruks.h"
#include "bonesplitterz/Wardokk.h"
#include "bonesplitterz/WurrgogProphet.h"

namespace Bonesplitterz {


    void Bonesplitterz::setWarclan(Warclan warclan) {
        removeKeyword(BONEGRINZ);
        removeKeyword(DRAKKFOOT);
        removeKeyword(ICEBONE);

        m_warclan = warclan;
        switch (warclan) {
            case Warclan::Bonegrinz:
                addKeyword(BONEGRINZ);
                break;
            case Warclan::Drakkfoot:
                addKeyword(DRAKKFOOT);
                break;
            case Warclan::Icebone:
                addKeyword(ICEBONE);
                break;
            default:
                break;
        }
    }

    std::string Bonesplitterz::ValueToString(const Parameter &parameter) {
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

        return ParameterValueToString(parameter);
    }

    int Bonesplitterz::EnumStringToInt(const std::string &enumString) {
        auto clan = magic_enum::enum_cast<Warclan>(enumString);
        if (clan.has_value()) return (int) clan.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    Wounds Bonesplitterz::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);

        // Warpaint
        return ignoreWounds(totalWounds, 6);
    }

    void Bonesplitterz::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Monster Hunters
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), MONSTER, 3.0);
        if (unit) {
            // Select ability
            auto roll = Dice::RollD6();
            if (roll == 1) {
                // Wild Abandon
                m_pileInMove = 6;
            } else if (roll <= 4) {
                // Stab! Stab! Stab
                m_stabStabStab = true;
            } else {
                // Berserk Strength
                m_berserkStrength = true;
            }
        }
    }

    void Bonesplitterz::onEndCombat(PlayerId player) {
        m_pileInMove = 3;
        m_stabStabStab = false;
        m_berserkStrength = false;

        Unit::onEndCombat(player);
    }

    Wounds Bonesplitterz::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);

        if (m_berserkStrength && target->hasKeyword(MONSTER) && (woundRoll == 6)) {
            wounds.mortal++;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Killa_Instinkt) && (woundRoll == 6) && weapon->isMelee()) {
            wounds.mortal++;
        }
        return wounds;
    }

    int Bonesplitterz::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        if (m_stabStabStab && target->hasKeyword(MONSTER)) mod++;

        if (isGeneral() && (weapon->name() == "Tusks and Hooves") &&
            (m_commandTrait == CommandTrait::Purebred_War_Boar)) {
            mod += 2;
        }
        return mod;
    }

    int Bonesplitterz::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        // Freezing Strike
        if ((m_warclan == Warclan::Icebone) && (woundRoll == 6)) rend--;
        return rend;
    }

    void Bonesplitterz::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Bonesplitterz::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Bonesplitterz::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Waaagh_Monger)) {
                if (Dice::RollD6() >= 4)
                    getRoster()->addCommandPoints(1);
            }
        }
    }

    int Bonesplitterz::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Power_Of_The_Beast)) {
            mod += 2;
        }
        return mod;
    }

    void Bonesplitterz::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if (isGeneral() && (battleRound == 1) && (m_commandTrait == CommandTrait::Dead_Kunnin)) {
            getRoster()->addCommandPoints(Dice::RollD3());
        }
    }

    int Bonesplitterz::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Master_Of_The_Weird)) {
            mod++;
        }
        return mod;
    }

    int Bonesplitterz::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Master_Of_The_Weird)) {
            mod++;
        }
        return mod;
    }

    int Bonesplitterz::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        if (hasKeyword(BONESPLITTERZ)) {
            auto general = dynamic_cast<Bonesplitterz *>(getRoster()->getGeneral());
            if (general && (distanceTo(general) < 18.0) &&
                (general->m_commandTrait == CommandTrait::Voice_Of_Da_Gods)) {
                mod += 2;
            }
        }
        return mod;
    }

    int Bonesplitterz::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Purebred_War_Boar)) {
            mod += 2;
        }
        return mod;
    }

    int Bonesplitterz::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if (isGeneral() && (weapon->name() == "Tusks and Hooves") &&
            (m_commandTrait == CommandTrait::Purebred_War_Boar)) {
            mod += 2;
        }
        return mod;
    }

    void Init() {
        SavageBoarboyManiaks::Init();
        ManiakWeirdnob::Init();
        SavageOrrukArrowboys::Init();
        SavageOrrukMorboys::Init();
        SavageBigBoss::Init();
        SavageBigStabbas::Init();
        SavageBoarboys::Init();
        SavageOrruks::Init();
        Wardokk::Init();
        WurrgogProphet::Init();
    }

} // namespace Bonesplitterz

