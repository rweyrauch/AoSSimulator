/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include "mawtribes/MawtribesBase.h"

#include "mawtribes/Tyrant.h"
#include "mawtribes/Ironblaster.h"
#include "mawtribes/Leadbelchers.h"
#include "mawtribes/OgorGluttons.h"
#include "mawtribes/Butcher.h"
#include "mawtribes/Slaughtermaster.h"
#include "mawtribes/Gorgers.h"
#include "mawtribes/Ironguts.h"
#include "mawtribes/Scraplauncher.h"
#include "mawtribes/Gnoblars.h"
#include "mawtribes/Firebelly.h"
#include "mawtribes/Maneaters.h"
#include "mawtribes/FrostlordStonehorn.h"
#include "mawtribes/FrostlordThundertusk.h"
#include "mawtribes/HuskardStonehorn.h"
#include "mawtribes/HuskardThundertusk.h"
#include "mawtribes/StonehornBeastriders.h"
#include "mawtribes/ThundertuskBeastriders.h"
#include "mawtribes/FrostSabres.h"
#include "mawtribes/IcebrowHunter.h"
#include "mawtribes/IcefallYhetees.h"
#include "mawtribes/MournfangPack.h"
#include "mawtribes/Hrothgorn.h"
#include "mawtribes/HrothgornsMantrappers.h"

namespace OgorMawtribes {

    std::string MawtribesBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Mawtribe") {
            if (parameter.intValue == NoMawtribe) return "No Mawtribe";
            else if (parameter.intValue == Meatfist) return "Meatfist";
            else if (parameter.intValue == Bloodgullet) return "Bloodgullet";
            else if (parameter.intValue == Underguts) return "Underguts";
            else if (parameter.intValue == Boulderhead) return "Boulderhead";
            else if (parameter.intValue == Thunderbellies) return "Thunderbellies";
            else if (parameter.intValue == Winterbite) return "Winterbite";
        }
        return ParameterValueToString(parameter);
    }

    int MawtribesBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "No Mawtribe") return NoMawtribe;
        else if (enumString == "Meatfist") return Meatfist;
        else if (enumString == "Bloodgullet") return Bloodgullet;
        else if (enumString == "Underguts") return Underguts;
        else if (enumString == "Boulderhead") return Boulderhead;
        else if (enumString == "Thunderbellies") return Thunderbellies;
        else if (enumString == "Winterbite") return Winterbite;
        return 0;
    }

    void MawtribesBase::onCharged() {
        Unit::onCharged();

        // Trampling Charge
        if (hasKeyword(OGOR) || hasKeyword(RHINOX)) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 1.0)) {
                Dice::RollResult result;
                int numDice = m_unmodifiedChargeRoll;

                // Fleshy Stampede
                if (hasKeyword(MEATFIST)) numDice++;

                int threshold = 6;
                if ((remainingModels() >= 8) || hasKeyword(MONSTER)) threshold -= 2;

                Dice::rollD6(numDice, result);

                unit->applyDamage({0, result.rollsGE(threshold)});
            }
        }
    }

    void MawtribesBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Grasp of the Everwinter
        if ((player == owningPlayer()) && hasKeyword(BEASTCLAW_RAIDERS)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                int roll = Dice::rollD6();
                if (roll < m_battleRound) {
                    unit->applyDamage({0, Dice::rollD3()});
                }
            }
        }
    }

    int MawtribesBase::rollChargeDistance() const {
        m_unmodifiedChargeRoll = Dice::roll2D6();
        return m_unmodifiedChargeRoll + chargeModifier();
    }

    int MawtribesBase::moveModifier() const {
        auto mod = Unit::moveModifier();

        // Ravenous Brutes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isHungery = units.empty();
        if (isHungery) mod += 2;

        return mod;
    }

    int MawtribesBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Ravenous Brutes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isEating = !units.empty();
        if (isEating) mod += 2;

        return mod;
    }

    void MawtribesBase::setMawtribe(Mawtribe tribe) {
        removeKeyword(MEATFIST);
        removeKeyword(BLOODGULLET);
        removeKeyword(UNDERGUTS);
        removeKeyword(BOULDERHEAD);
        removeKeyword(THUNDERBELLIES);
        removeKeyword(WINTERBITE);

        m_tribe = tribe;

        switch (m_tribe) {
            case Meatfist:
                addKeyword(MEATFIST);
                break;
            case Bloodgullet:
                addKeyword(BLOODGULLET);
                break;
            case Underguts:
                addKeyword(UNDERGUTS);
                break;
            case Boulderhead:
                addKeyword(BOULDERHEAD);
                break;
            case Thunderbellies:
                addKeyword(THUNDERBELLIES);
                break;
            case Winterbite:
                addKeyword(WINTERBITE);
                break;
            default:
                break;
        }
    }

    void Init() {
        Butcher::Init();
        Firebelly::Init();
        FrostSabres::Init();
        Gnoblars::Init();
        Gorgers::Init();
        Hrothgorn::Init();
        HrothgornsMantrappers::Init();
        IcebrowHunter::Init();
        IcefallYhetees::Init();
        Ironblaster::Init();
        Ironguts::Init();
        Leadbelchers::Init();
        Maneaters::Init();
        MournfangPack::Init();
        OgorGluttons::Init();
        GnoblarScraplauncher::Init();
        Slaughtermaster::Init();
        Tyrant::Init();
        FrostlordOnStonehorn::Init();
        FrostlordOnThundertusk::Init();
        HuskardOnStonehorn::Init();
        HuskardOnThundertusk::Init();
        StonehornBeastriders::Init();
        ThundertuskBeastriders::Init();
    }

} // namespace OgorMawtribes
