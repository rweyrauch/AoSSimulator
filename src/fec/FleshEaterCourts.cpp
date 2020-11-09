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

namespace FleshEaterCourt {

    Rerolls FleshEaterCourts::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_delusion == Delusion::The_Royal_Hunt) {
            if (target->hasKeyword(MONSTER)) {
                return Reroll_Ones;
            }
        } else if (m_delusion == Delusion::A_Matter_Of_Honour) {
            if (target->hasKeyword(HERO)) {
                return Reroll_Ones;
            }
        } else if (m_delusion == Delusion::The_Grand_Tournament) {
            if (!isGeneral()) {
                return Reroll_Ones;
            }
        } else {
            // Morgaunt Court - Blood Loyalty
            if (hasKeyword(MORGAUNT) && hasKeyword(COURTIER)) {
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
                for (auto ip : units) {
                    if (ip->hasKeyword(MORGAUNT) && ip->hasKeyword(SERFS)) {
                        return Reroll_Ones;
                    }
                }
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls FleshEaterCourts::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_delusion == Delusion::The_Royal_Hunt) {
            if (target->hasKeyword(MONSTER)) {
                return Reroll_Ones;
            }
        } else if (m_delusion == Delusion::A_Matter_Of_Honour) {
            if (target->isGeneral()) {
                return Reroll_Ones;
            }
        } else {
            // Hollowmourne - Shattering Charge
            if (hasKeyword(HOLLOWMOURNE) && (hasKeyword(COURTIER) || hasKeyword(KNIGHTS))) {
                if (charged()) {
                    return Reroll_Ones;
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

    Wounds FleshEaterCourts::applyWoundSave(const Wounds &wounds) {
        // Deathless Courtiers
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto ip : units) {
            if (ip->hasKeyword(FLESH_EATER_COURTS) && ip->hasKeyword(HERO)) {
                Dice::RollResult woundSaves, mortalSaves;
                Dice::RollD6(wounds.normal, woundSaves);
                Dice::RollD6(wounds.mortal, mortalSaves);

                Wounds totalWounds = wounds;
                totalWounds.normal -= woundSaves.rollsGE(6);
                totalWounds.normal = std::max(totalWounds.normal, 0);
                totalWounds.mortal -= mortalSaves.rollsGE(6);
                totalWounds.mortal = std::max(totalWounds.mortal, 0);

                return totalWounds;
            }
        }

        return Unit::applyWoundSave(wounds);
    }

    std::string FleshEaterCourts::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Grand Court") {
            auto courtName = magic_enum::enum_name((GrandCourt)parameter.intValue);
            return std::string(courtName);
        }
        if (std::string(parameter.name) == "Delusion") {
            auto delusionName = magic_enum::enum_name((Delusion)parameter.intValue);
            return std::string(delusionName);
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
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }

        return ParameterValueToString(parameter);
    }

    int FleshEaterCourts::EnumStringToInt(const std::string &enumString) {
        auto court = magic_enum::enum_cast<GrandCourt>(enumString);
        if (court.has_value()) return (int)court.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int)mount.value();

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
    }

} // namespace FleshEaterCourt
