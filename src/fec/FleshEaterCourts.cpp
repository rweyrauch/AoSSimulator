/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <fec/FleshEaterCourts.h>
#include <Board.h>
#include <Roster.h>

#include "fec/CryptHorrors.h"
#include "fec/CryptGhouls.h"
#include "fec/CryptFlayers.h"
#include "fec/CryptInfernalCourtier.h"
#include "fec/AbhorrantArchregent.h"
#include "fec/AbhorrantGhoulKing.h"
#include "fec/GhoulKingOnTerrorgheist.h"
#include "fec/GhoulKingOnZombieDragon.h"
#include "fec/VarghulfCourtier.h"
#include "fec/CryptGhastCourtier.h"
#include "fec/CryptHaunterCourtier.h"
#include "fec/RoyalZombieDragon.h"
#include "fec/RoyalTerrorgheist.h"
#include "fec/DukeCrakmarrow.h"
#include "fec/TheGrymwatch.h"

namespace FleshEaterCourt {

    Rerolls FleshEaterCourts::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_delusion == Delusion::The_Royal_Hunt) {
            if (target->hasKeyword(MONSTER)) {
                return Rerolls::Ones;
            }
        } else if (m_delusion == Delusion::A_Matter_Of_Honour) {
            if (target->hasKeyword(HERO)) {
                return Rerolls::Ones;
            }
        } else if (m_delusion == Delusion::The_Grand_Tournament) {
            if (!isGeneral()) {
                return Rerolls::Ones;
            }
        } else {
            // Morgaunt Court - Blood Loyalty
            if (hasKeyword(MORGAUNT) && hasKeyword(COURTIER)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
                for (auto ip : units) {
                    if (ip->hasKeyword(MORGAUNT) && ip->hasKeyword(SERFS)) {
                        return Rerolls::Ones;
                    }
                }
            }
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Savage_Chivalry)) {
            auto serfs = Board::Instance()->getUnitsWithKeyword(owningPlayer(), SERFS);
            for (auto serf : serfs) {
                if (distanceTo(serf) < 12.0) {
                    return Rerolls::Ones;
                }
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls FleshEaterCourts::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_delusion == Delusion::The_Royal_Hunt) {
            if (target->hasKeyword(MONSTER)) {
                return Rerolls::Ones;
            }
        } else if (m_delusion == Delusion::A_Matter_Of_Honour) {
            if (target->isGeneral()) {
                return Rerolls::Ones;
            }
        } else {
            // Hollowmourne - Shattering Charge
            if (hasKeyword(HOLLOWMOURNE) && (hasKeyword(COURTIER) || hasKeyword(KNIGHTS))) {
                if (charged()) {
                    return Rerolls::Ones;
                }
            }
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int FleshEaterCourts::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Gristlegore - Peerless Ferocity
        if (hasKeyword(GRISTLEGORE) && (hasKeyword(HERO) || hasKeyword(MONSTER))) {
            if (unmodifiedHitRoll == 6) {
                return 2;
            }
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int FleshEaterCourts::moveModifier() const {
        int modifier = Unit::moveModifier();
        // Blisterskin - Blistering Speed
        if (hasKeyword(BLISTERSKIN)) {
            modifier += 2;
        }
        return modifier;
    }

    int FleshEaterCourts::runModifier() const {
        int modifier = Unit::runModifier();
        if (m_delusion == Delusion::Crusading_Army) {
            modifier += 1;
        }
        return modifier;
    }

    int FleshEaterCourts::chargeModifier() const {
        int modifier = Unit::chargeModifier();
        if (m_delusion == Delusion::Crusading_Army) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds FleshEaterCourts::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Deathless Courtiers
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto ip : units) {
            if (ip->hasKeyword(FLESH_EATER_COURTS) && ip->hasKeyword(HERO)) {
                return ignoreWounds(wounds, 6);
            }
        }

        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    std::string FleshEaterCourts::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Grand Court") {
            auto courtName = magic_enum::enum_name((GrandCourt) parameter.intValue);
            return std::string(courtName);
        }
        if (std::string(parameter.name) == "Delusion") {
            auto delusionName = magic_enum::enum_name((Delusion) parameter.intValue);
            return std::string(delusionName);
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

        return ParameterValueToString(parameter);
    }

    int FleshEaterCourts::EnumStringToInt(const std::string &enumString) {
        auto court = magic_enum::enum_cast<GrandCourt>(enumString);
        if (court.has_value()) return (int) court.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int) mount.value();

        return 0;
    }

    void FleshEaterCourts::setGrandCourt(FleshEaterCourt::GrandCourt court) {
        removeKeyword(MORGAUNT);
        removeKeyword(HOLLOWMOURNE);
        removeKeyword(BLISTERSKIN);
        removeKeyword(GRISTLEGORE);

        m_grandCourt = court;
        switch (m_grandCourt) {
            case GrandCourt::Morgaunt:
                addKeyword(MORGAUNT);
                break;
            case GrandCourt::Hollowmourne:
                addKeyword(HOLLOWMOURNE);
                break;
            case GrandCourt::Blisterskin:
                addKeyword(BLISTERSKIN);
                break;
            case GrandCourt::Gristlegore:
                addKeyword(GRISTLEGORE);
                break;
            default:
                break;
        }
    }

    Rerolls FleshEaterCourts::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (m_delusion == Delusion::Defenders_Of_The_Realm) {
            // Defenders of the Realm
            int numModelsIn = 0;
            for (const auto &model : m_models) {
                if (Board::Instance()->isModelWithinDeploymentZone(model.get(), owningPlayer())) {
                    numModelsIn++;
                }
            }
            if (numModelsIn >= remainingModels() / 2)
                return Rerolls::Ones;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    void FleshEaterCourts::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Hellish_Orator)) {
            if (Dice::RollD6() >= 4) {
                getRoster()->addCommandPoints(1);
            }
        }
    }

    void Init() {
        CryptGhouls::Init();
        CryptHorrors::Init();
        CryptFlayers::Init();
        CryptInfernalCourtier::Init();
        AbhorrantArchregent::Init();
        AbhorrantGhoulKing::Init();
        AbhorrantGhoulKingOnTerrorgheist::Init();
        AbhorrantGhoulKingOnZombieDragon::Init();
        VarghulfCourtier::Init();
        RoyalTerrorgheist::Init();
        RoyalZombieDragon::Init();
        CryptGhastCourtier::Init();
        CryptHaunterCourtier::Init();
        DukeCrakmarrow::Init();
        TheGrymwatch::Init();
    }

} // namespace FleshEaterCourt
