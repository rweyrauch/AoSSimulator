/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <bonesplitterz/Bonesplitterz.h>
#include <Board.h>
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
            auto clanName = magic_enum::enum_name((Warclan)parameter.intValue);
            return std::string(clanName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(loreName);
        }

        return ParameterValueToString(parameter);
    }

    int Bonesplitterz::EnumStringToInt(const std::string &enumString) {
        auto clan = magic_enum::enum_cast<Warclan>(enumString);
        if (clan.has_value()) return (int)clan.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        return 0;
    }

    Wounds Bonesplitterz::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Unit::applyWoundSave(wounds);

        // Warpaint
        Dice::RollResult normalResult, mortalResult;
        Dice::rollD6(totalWounds.normal, normalResult);
        Dice::rollD6(totalWounds.mortal, mortalResult);

        totalWounds.normal -= normalResult.rollsGE(6);
        totalWounds.mortal -= mortalResult.rollsGE(6);

        return totalWounds;
    }

    void Bonesplitterz::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Monster Hunters
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), MONSTER, 3.0);
        if (unit) {
            // Select ability
            auto roll = Dice::rollD6();
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

    Wounds Bonesplitterz::onEndCombat(PlayerId player) {
        m_pileInMove = 3;
        m_stabStabStab = false;
        m_berserkStrength = false;

        return Unit::onEndCombat(player);
    }

    Wounds Bonesplitterz::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);

        if (m_berserkStrength && target->hasKeyword(MONSTER) && (woundRoll == 6)) {
            wounds.mortal++;
        }

        return wounds;
    }

    int Bonesplitterz::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        if (m_stabStabStab && target->hasKeyword(MONSTER)) mod++;

        return mod;
    }

    int Bonesplitterz::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
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

