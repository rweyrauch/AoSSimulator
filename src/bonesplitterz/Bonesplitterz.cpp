/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <bonesplitterz/Bonesplitterz.h>
#include <Board.h>

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
            case Bonegrinz:
                addKeyword(BONEGRINZ);
                break;
            case Drakkfoot:
                addKeyword(DRAKKFOOT);
                break;
            case Icebone:
                addKeyword(ICEBONE);
                break;
            default:
                break;
        }
    }

    std::string Bonesplitterz::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Warclan") {
            if (parameter.intValue == Bonegrinz) { return "Bonegrinz"; }
            else if (parameter.intValue == Drakkfoot) { return "Drakkfoot"; }
            else if (parameter.intValue == Icebone) { return "Icebone"; }
        }
        return ParameterValueToString(parameter);
    }

    int Bonesplitterz::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Bonegrinz") { return Bonegrinz; }
        else if (enumString == "Drakkfoot") { return Drakkfoot; }
        else if (enumString == "Icebone") { return Icebone; }
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
        if ((m_warclan == Icebone) && (woundRoll == 6)) rend--;
        return rend;
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

