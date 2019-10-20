/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/KhorneBase.h>
#include <Board.h>

#include "khorne/Bloodreavers.h"
#include "khorne/BloodWarriors.h"
#include "khorne/Bloodletters.h"
#include "khorne/Skullreapers.h"
#include "khorne/Wrathmongers.h"
#include "khorne/Khorgoraths.h"
#include "khorne/BloodthirsterOfUnfetteredFury.h"
#include "khorne/BloodthirsterOfInsensateRage.h"
#include "khorne/WrathOfKhorneBloodthirster.h"
#include "khorne/Skarbrand.h"
#include "khorne/Karanak.h"
#include "khorne/FleshHounds.h"
#include "khorne/Skulltaker.h"
#include "khorne/Bloodmaster.h"
#include "khorne/Bloodcrushers.h"
#include "khorne/MightySkullcrushers.h"
#include "khorne/Skullmaster.h"
#include "khorne/VorgarothAndSkalok.h"
#include "khorne/MightyLordOfKhorne.h"
#include "khorne/Slaughterpriest.h"
#include "khorne/KorghosKhul.h"
#include "khorne/ExaltedDeathbringer.h"
#include "khorne/Bloodsecrator.h"
#include "khorne/Bloodstoker.h"
#include "khorne/BloodThrone.h"
#include "khorne/SkullCannons.h"
#include "khorne/ValkiaTheBloody.h"
#include "khorne/AspiringDeathbringer.h"
#include "khorne/Skullgrinder.h"
#include "khorne/ScylaAnfingrimm.h"
#include "khorne/LordOfKhorneOnJuggernaut.h"
#include "khorne/SkarrBloodwrath.h"
#include "khorne/Riptooth.h"

namespace Khorne
{

void KhorneBase::setSlaughterHost(SlaughterHost host)
{
    m_slaughterHost = host;
    switch (m_slaughterHost)
    {
        case ReapersOfVengeance:
            addKeyword(REAPERS_OF_VENGEANCE);
            break;
        case Bloodlords:
            addKeyword(BLOODLORDS);
            break;
        case Goretide:
            addKeyword(GORETIDE);
            break;
        case SkullfiendTribe:
            addKeyword(SKULLFIEND_TRIBE);
            break;
        default:
            break;
    }
}

Rerolls KhorneBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Slay the Mighty
    if (m_slaughterHost == Bloodlords)
    {
        if (hasKeyword(DAEMON) && (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)))
        {
            return RerollOnes;
        }
    }
    // Tireless Conquerors
    else if (m_slaughterHost == Goretide)
    {
        if (hasKeyword(MORTAL))
        {
            auto numObjMarkers = Board::Instance()->getNumObjectives();
            for (auto i = 0; i < numObjMarkers; i++)
            {
                const auto obj = Board::Instance()->getObjective(i);
                if (obj)
                {
                    if (position().distance(Math::Point3(obj->m_x, obj->m_y, 0.0f)) <= 12.0f)
                    {
                        return RerollOnes;
                    }
                }
            }
        }
    }
    return Unit::toWoundRerolls(weapon, target);
}

Rerolls KhorneBase::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Skull Hunters
    if (m_slaughterHost == SkullfiendTribe)
    {
        if (hasKeyword(MORTAL))
        {
            auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(m_owningPlayer), HERO, 12.0f);
            if (unit)
            {
                return RerollOnes;
            }
        }
    }

    // Locus of Fury
    if (hasKeyword(DAEMON))
    {
        auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 16.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(GREATER_DAEMON))
            {
                return RerollOnes;
            }
            else if (ip->hasKeyword(DAEMON) && distanceTo(ip) <= 12.0f)
            {
                return RerollOnes;
            }
        }
    }
    return Unit::toHitRerolls(weapon, unit);
}

std::string KhorneBase::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Slaughter Host")
    {
        if (parameter.m_intValue == ReapersOfVengeance) { return "Reapers of Vengeance"; }
        else if (parameter.m_intValue == Bloodlords) { return "Bloodlords"; }
        else if (parameter.m_intValue == Goretide) { return "Goretide"; }
        else if (parameter.m_intValue == SkullfiendTribe) { return "Skullfiend Tribe"; }
        else if (parameter.m_intValue == None) { return "None"; }
    }
    return ParameterValueToString(parameter);
}

int KhorneBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Reapers of Vengeance") { return ReapersOfVengeance; }
    else if (enumString == "Bloodlords") { return Bloodlords; }
    else if (enumString == "Goretide") { return Goretide; }
    else if (enumString == "Skullfiend Tribe") { return SkullfiendTribe; }
    else if (enumString == "None") { return None; }
    return 0;
}

void Init()
{
    AspiringDeathbringer::Init();
    Bloodcrushers::Init();
    Bloodletters::Init();
    Bloodmaster::Init();
    Bloodreavers::Init();
    Bloodsecrator::Init();
    Bloodstoker::Init();
    BloodthirsterOfInsensateRage::Init();
    BloodthirsterOfUnfetteredFury::Init();
    BloodWarriors::Init();
    ExaltedDeathbringer::Init();
    FleshHounds::Init();
    HeraldOfKhorneOnBloodThrone::Init();
    Karanak::Init();
    Khorgoraths::Init();
    KorghosKhul::Init();
    LordOfKhorneOnJuggernaut::Init();
    MightyLordOfKhorne::Init();
    MightySkullcrushers::Init();
    Riptooth::Init();
    ScylaAnfingrimm::Init();
    Skarbrand::Init();
    SkarrBloodwrath::Init();
    SkullCannons::Init();
    Skullgrinder::Init();
    Skullmaster::Init();
    Skullreapers::Init();
    Skulltaker::Init();
    Slaughterpriest::Init();
    ValkiaTheBloody::Init();
    VorgarothAndSkalok::Init();
    Wrathmongers::Init();
    WrathOfKhorneBloodthirster::Init();
}

} //namespace Khorne