/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include <magic_enum.hpp>
#include <Roster.h>
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
            auto tribeName = magic_enum::enum_name((Mawtribe) parameter.intValue);
            return std::string(tribeName);
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
        if (std::string(parameter.name) == "Prayer") {
            auto prayerName = magic_enum::enum_name((Prayer) parameter.intValue);
            return std::string(prayerName);
        }
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }

        return ParameterValueToString(parameter);
    }

    int MawtribesBase::EnumStringToInt(const std::string &enumString) {
        auto tribe = magic_enum::enum_cast<Mawtribe>(enumString);
        if (tribe.has_value()) return (int) tribe.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto prayer = magic_enum::enum_cast<Prayer>(enumString);
        if (prayer.has_value()) return (int) prayer.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int) mount.value();

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
                if ((remainingModels() >= 8) || hasKeyword(MONSTER) || (m_commandTrait == CommandTrait::Crushing_Bulk)) threshold -= 2;

                Dice::RollD6(numDice, result);

                unit->applyDamage({0, result.rollsGE(threshold)}, this);
            }
        }
    }

    void MawtribesBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Grasp of the Everwinter
        if ((player == owningPlayer()) && hasKeyword(BEASTCLAW_RAIDERS)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                int roll = Dice::RollD6();
                if (roll < m_battleRound) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }

        if (m_commandTrait == CommandTrait::Food_For_Thought) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            const bool isEating = !units.empty();
            if (isEating) {
                getRoster()->addCommandPoints(1);
            }
        }
    }

    int MawtribesBase::rollChargeDistance() {
        m_unmodifiedChargeRoll = Dice::Roll2D6();
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
            case Mawtribe::Meatfist:
                addKeyword(MEATFIST);
                break;
            case Mawtribe::Bloodgullet:
                addKeyword(BLOODGULLET);
                break;
            case Mawtribe::Underguts:
                addKeyword(UNDERGUTS);
                break;
            case Mawtribe::Boulderhead:
                addKeyword(BOULDERHEAD);
                break;
            case Mawtribe::Thunderbellies:
                addKeyword(THUNDERBELLIES);
                break;
            case Mawtribe::Winterbite:
                addKeyword(WINTERBITE);
                break;
            default:
                break;
        }
    }

    void MawtribesBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void MawtribesBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    int MawtribesBase::questionableHygiene(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (!isFriendly(attacker) && distanceTo(attacker) < 6.0) {
            return -1;
        }
        return 0;
    }

    int MawtribesBase::heraldOfGulpingGod(const Unit *unit) {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isFeeding = !units.empty();
        if (isFriendly(unit) && isFeeding && (distanceTo(unit) < 15.0)) {
            return Autopass_Battleshock;
        }
        return 0;
    }

    int MawtribesBase::growlingStomach(const Unit *unit) {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        const bool isHungry = units.empty();
        if (!isFriendly(unit) && isHungry && (distanceTo(unit) < 12.0)) {
            return -2;
        }
        return 0;
    }

    int MawtribesBase::masterOfMournfangs(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(MOURNFANG_PACK) && (distanceTo(unit) < 18.0)) {
            return Autopass_Battleshock;
        }
        return 0;
    }

    int MawtribesBase::raisedByYhetees(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                       const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(ICEFALL_YHETESS) && (distanceTo(attacker) < 12.0)) {
            return 1;
        }
        return 0;
    }

    Rerolls MawtribesBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        const auto inEnemyTerritory = Board::Instance()->isUnitWithinDeploymentZone(this, GetEnemyId(owningPlayer()));
        if ((m_commandTrait == CommandTrait::Nomadic_Raider) && weapon->isMelee() && inEnemyTerritory) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
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
