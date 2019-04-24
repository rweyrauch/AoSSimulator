/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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

namespace FleshEaterCourt
{

Rerolls FleshEaterCourts::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (m_delusion == TheRoyalHunt)
    {
        if (target->hasKeyword(MONSTER))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == AMatterOfHonour)
    {
        if (target->hasKeyword(HERO))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == TheGrandTournament)
    {
        if (!isGeneral())
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == None)
    {
        // Morgaunt Court - Blood Loyalty
        if (hasKeyword(MORGAUNT) && hasKeyword(COURTIER))
        {
            auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
            for (auto ip : units)
            {
                if (ip->hasKeyword(MORGAUNT) && ip->hasKeyword(SERFS))
                {
                    return RerollOnes;
                }
            }
        }
    }
    return Unit::toHitRerolls(weapon, target);
}

Rerolls FleshEaterCourts::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    if (m_delusion == TheRoyalHunt)
    {
        if (target->hasKeyword(MONSTER))
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == AMatterOfHonour)
    {
        if (target->isGeneral())
        {
            return RerollOnes;
        }
    }
    else if (m_delusion == None)
    {
        // Hollowmourne - Shattering Charge
        if (hasKeyword(HOLLOWMOURNE) && (hasKeyword(COURTIER) || hasKeyword(KNIGHTS)))
        {
            if (charged())
            {
                return RerollOnes;
            }
        }
    }
    return Unit::toWoundRerolls(weapon, target);
}

int FleshEaterCourts::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Gristlegore - Peerless Ferocity
    if (hasKeyword(GRISTLEGORE) && (hasKeyword(HERO) || hasKeyword(MONSTER)))
    {
        if (unmodifiedHitRoll == 6)
        {
            return 2;
        }
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int FleshEaterCourts::moveModifier() const
{
    int modifier = Unit::moveModifier();
    // Blisterskin - Blistering Speed
    if (hasKeyword(BLISTERSKIN))
    {
        modifier += 2;
    }
    return modifier;
}

int FleshEaterCourts::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_delusion == CrusadingArmy)
    {
        modifier += 1;
    }
    return modifier;
}

int FleshEaterCourts::chargeModifier() const
{
    int modifier = Unit::chargeModifier();
    if (m_delusion == CrusadingArmy)
    {
        modifier += 1;
    }
    return modifier;
}

Wounds FleshEaterCourts::applyWoundSave(const Wounds &wounds)
{
    Dice dice;
    // Deathless Courtiers
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(FLESH_EATER_COURTS) && ip->hasKeyword(HERO))
        {
            Dice::RollResult woundSaves, mortalSaves;
            dice.rollD6(wounds.normal, woundSaves);
            dice.rollD6(wounds.mortal, mortalSaves);

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

std::string FleshEaterCourts::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Grand Court")
    {
        if (parameter.m_intValue == Morgaunt) { return "Morgaunt"; }
        else if (parameter.m_intValue == Hollowmourne) { return "Hollowmourne"; }
        else if (parameter.m_intValue == Blisterskin) { return "Blisterskin"; }
        else if (parameter.m_intValue == Gristlegore) { return "Gristlegore"; }
        else if (parameter.m_intValue == NoCourt) { return "No Court"; }
    }
    else if (parameter.m_name == "Delusion")
    {
        if (parameter.m_intValue == CrusadingArmy) { return "Crusading Army"; }
        else if (parameter.m_intValue == TheRoyalHunt) { return "The Royal Hunt"; }
        else if (parameter.m_intValue == TheFeastDay) { return "The Feast Day"; }
        else if (parameter.m_intValue == AMatterOfHonour) { return "A Matter of Honour"; }
        else if (parameter.m_intValue == TheGrandTournament) { return "The Grand Tournament"; }
        else if (parameter.m_intValue == DefendersOfTheRealm) { return "Defenders of the Realm"; }
        else if (parameter.m_intValue == None) { return "None"; }
    }
    return ParameterValueToString(parameter);
}

int FleshEaterCourts::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Morgaunt") { return Morgaunt; }
    else if (enumString == "Hollowmourne") { return Hollowmourne; }
    else if (enumString == "Blisterskin") { return Blisterskin; }
    else if (enumString == "Gristlegore") { return Gristlegore; }
    else if (enumString == "No Court") { return NoCourt; }
    else if (enumString == "Crusading Army") { return CrusadingArmy; }
    else if (enumString == "The Royal Hunt") { return TheRoyalHunt; }
    else if (enumString == "The Feast Day") { return TheFeastDay; }
    else if (enumString == "A Matter of Honour") { return AMatterOfHonour; }
    else if (enumString == "The Grand Tournament") { return TheGrandTournament; }
    else if (enumString == "Defenders of the Realm") { return DefendersOfTheRealm; }
    else if (enumString == "None") { return None; }

    return 0;
}

void FleshEaterCourts::setGrandCourt(FleshEaterCourts::GrandCourt court)
{
    m_grandCourt = court;
    switch (m_grandCourt)
    {
        case Morgaunt:
            addKeyword(MORGAUNT);
            break;
        case Hollowmourne:
            addKeyword(HOLLOWMOURNE);
            break;
        case Blisterskin:
            addKeyword(BLISTERSKIN);
            break;
        case Gristlegore:
            addKeyword(GRISTLEGORE);
            break;
        default:
            break;
    }
}

void Init()
{
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
