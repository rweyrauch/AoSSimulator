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

void Init()
{
    Khorne::Bloodcrushers::Init();
    Khorne::Bloodletters::Init();
    Khorne::Bloodmaster::Init();
    Khorne::Bloodreavers::Init();
    Khorne::BloodthirsterOfInsensateRage::Init();
    Khorne::BloodthirsterOfUnfetteredFury::Init();
    Khorne::BloodWarriors::Init();
    Khorne::FleshHounds::Init();
    Khorne::Karanak::Init();
    Khorne::Khorgoraths::Init();
    Khorne::KorghosKhul::Init();
    Khorne::MightyLordOfKhorne::Init();
    Khorne::MightySkullcrushers::Init();
    Khorne::Skarbrand::Init();
    Khorne::Skullmaster::Init();
    Khorne::Skullreapers::Init();
    Khorne::Skulltaker::Init();
    Khorne::Slaughterpriest::Init();
    Khorne::VorgarothAndSkalok::Init();
    Khorne::Wrathmongers::Init();
    Khorne::WrathOfKhorneBloodthirster::Init();
}

} //namespace Khorne