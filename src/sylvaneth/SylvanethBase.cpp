/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sylvaneth/SylvanethBase.h>
#include <Board.h>


#include "sylvaneth/Alarielle.h"
#include "sylvaneth/KurnothHunters.h"
#include "sylvaneth/SpiritOfDurthu.h"
#include "sylvaneth/Dryads.h"
#include "sylvaneth/TreeRevenants.h"
#include "sylvaneth/SpiteRevenants.h"
#include "sylvaneth/DrychaHamadreth.h"
#include "sylvaneth/Treelord.h"
#include "sylvaneth/TreelordAncient.h"
#include "sylvaneth/Branchwraith.h"
#include "sylvaneth/Branchwych.h"
#include "sylvaneth/ArchRevenant.h"
#include "sylvaneth/SkaethsWildHunt.h"
#include "sylvaneth/Ylthari.h"
#include "sylvaneth/YlthariGardians.h"


namespace Sylvaneth
{

void SylvanethBase::setGlade(Glade glade)
{
    removeKeyword(OAKENBROW);
    removeKeyword(GNARLROOT);
    removeKeyword(HEARTWOOD);
    removeKeyword(IRONBARK);
    removeKeyword(WINTERLEAF);
    removeKeyword(DREADWOOD);
    removeKeyword(HARVESTBOON);

    m_glade = glade;
    switch (m_glade)
    {
        case Oakenbrow:
            addKeyword(OAKENBROW);
            break;
        case Gnarlroot:
            addKeyword(GNARLROOT);
            break;
        case Heartwood:
            addKeyword(HEARTWOOD);
            break;
        case Ironbark:
            addKeyword(IRONBARK);
            break;
        case Winterleaf:
            addKeyword(WINTERLEAF);
            break;
        case Dreadwood:
            addKeyword(DREADWOOD);
            break;
        case Harvestboon:
            addKeyword(HARVESTBOON);
            break;
        default:
            break;
    }
}

Rerolls SylvanethBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    return Unit::toWoundRerolls(weapon, target);
}

Rerolls SylvanethBase::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Vibrant Surge
    if (hasKeyword(HARVESTBOON) && m_charged)
    {
        return RerollOnes;
    }
    // Shield the Arcane
    else if (hasKeyword(GNARLROOT))
    {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(GNARLROOT) && ip->hasKeyword(WIZARD))
            {
                return RerollOnes;
            }
        }
    }
    return Unit::toHitRerolls(weapon, unit);
}

std::string SylvanethBase::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Glade")
    {
        if (parameter.intValue == Oakenbrow) { return "Oakenbrow"; }
        else if (parameter.intValue == Gnarlroot) { return "Gnarlroot"; }
        else if (parameter.intValue == Heartwood) { return "Heartwood"; }
        else if (parameter.intValue == Ironbark) { return "Ironbark"; }
        else if (parameter.intValue == Winterleaf) { return "Winterleaf"; }
        else if (parameter.intValue == Dreadwood) { return "Dreadwood"; }
        else if (parameter.intValue == Harvestboon) { return "Harvestboon"; }
        else if (parameter.intValue == None) { return "None"; }
    }
    return ParameterValueToString(parameter);
}

int SylvanethBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Oakenbrow") { return Oakenbrow; }
    else if (enumString == "Gnarlroot") { return Gnarlroot; }
    else if (enumString == "Heartwood") { return Heartwood; }
    else if (enumString == "Ironbark") { return Ironbark; }
    else if (enumString == "Winterleaf") { return Winterleaf; }
    else if (enumString == "Dreadwood") { return Dreadwood; }
    else if (enumString == "Harvestboon") { return Harvestboon; }
    else if (enumString == "None") { return None; }
    return 0;
}

int SylvanethBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Winter's Bite
    if ((unmodifiedHitRoll == 6) && hasKeyword(WINTERLEAF))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

Rerolls SylvanethBase::battleshockRerolls() const
{
    // Stubborn and Taciturn
    if (hasKeyword(IRONBARK))
    {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(HERO) && ip->hasKeyword(IRONBARK))
            {
                return RerollFailed;
            }
        }
    }
    return Unit::battleshockRerolls();
}

int SylvanethBase::braveryModifier() const
{
    auto mod = Unit::braveryModifier();

    // Courage for Kurnoth
    if (hasKeyword(HEARTWOOD))
    {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(HERO) && ip->hasKeyword(HEARTWOOD))
            {
                mod++;
                break;
            }
        }
    }
    return mod;
}

void Init()
{
    Alarielle::Init();
    KurnothHunters::Init();
    SpiritOfDurthu::Init();
    Dryads::Init();
    TreeRevenants::Init();
    SpiteRevenants::Init();
    DrychaHamadreth::Init();
    Treelord::Init();
    TreelordAncient::Init();
    Branchwraith::Init();
    Branchwych::Init();
    ArchRevenant::Init();
    SkaethsWildHunt::Init();
    Ylthari::Init();
    YltharisGuardians::Init();
}

} //namespace Sylvaneth