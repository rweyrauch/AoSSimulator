/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/DaughterOfKhaine.h>

#include "dok/WitchAelves.h"
#include "dok/BloodSisters.h"
#include "dok/KhineraiLifetakers.h"
#include "dok/SistersOfSlaughter.h"
#include "dok/KhineraiHeartrenders.h"
#include "dok/DoomfireWarlocks.h"
#include "dok/BloodStalkers.h"
#include "dok/HagQueen.h"
#include "dok/SlaughterQueen.h"
#include "dok/AvatarOfKhaine.h"
#include "dok/BloodwrackMedusa.h"

namespace DaughtersOfKhaine
{


int DaughterOfKhaine::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);

    // Bladed Killers
    if (hasKeyword(DRAICHI_GANETH) && m_charged)
    {
        modifier += 1;
    }
    return modifier;
}


Rerolls DaughterOfKhaine::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Blood Rites - Zealot's Rage
    if (m_battleRound == 3)
    {
        // Daughters of the First Temple
        if (hasKeyword(HAGG_NAR))
        {
            return RerollFailed;
        }
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

Rerolls DaughterOfKhaine::toWoundRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Blood Rites - Slaughterer's Strength
    if (m_battleRound == 4)
    {
        return RerollOnes;
    }

    return Unit::toWoundRerolls(weapon, unit);
}

Rerolls DaughterOfKhaine::toSaveRerolls(const Weapon *weapon) const
{
    // Blood Rites - Unquenchable Fervour
    if (m_battleRound == 5)
    {
        return RerollOnes;
    }

    return Unit::toSaveRerolls(weapon);
}

Rerolls DaughterOfKhaine::runRerolls() const
{
    // Blood Rites - Quickening Bloodlust
    if (m_battleRound == 1)
    {
        return RerollOnes;
    }
    return Unit::runRerolls();
}

Rerolls DaughterOfKhaine::chargeRerolls() const
{
    // Blood Rites - Headlong Fury
    if (m_battleRound == 2)
    {
        return RerollOnes;
    }
    return Unit::chargeRerolls();
}

void Init()
{
    AvatarOfKhaine::Init();
    BloodSisters::Init();
    BloodStalkers::Init();
    BloodwrackMedusa::Init();
    DoomfireWarlocks::Init();
    HagQueen::Init();
    KhineraiHeartrenders::Init();
    KhineraiLifetakers::Init();
    SistersOfSlaughter::Init();
    SlaughterQueen::Init();
    WitchAelves::Init();
}

} // namespace DaughtersOfKhaine
