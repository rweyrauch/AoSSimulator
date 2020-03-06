/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/TheWurmspat.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    TheWurmspat::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    TheWurmspat::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool TheWurmspat::s_registered = false;


Unit *TheWurmspat::Create(const ParameterList &parameters)
{
    auto unit = new TheWurmspat();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TheWurmspat::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("The Wurmspat", factoryMethod);
    }
}

TheWurmspat::TheWurmspat() :
    NurgleBase("The Wurmspat", 4, WOUNDS, 8, 4, false),
    m_blightedWeapons(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, BLESSED_SONS, PUTRID_BLIGHTKINGS, THE_WURMSPAT};
    m_weapons = {&m_blightedWeapons};
}

bool TheWurmspat::configure()
{
    auto sepsimus = new Model(BASESIZE, WOUNDS);
    sepsimus->addMeleeWeapon(&m_blightedWeapons);
    addModel(sepsimus);

    auto ghulgoch = new Model(BASESIZE, WOUNDS);
    ghulgoch->addMeleeWeapon(&m_blightedWeapons);
    addModel(ghulgoch);

    m_points = POINTS_PER_UNIT;

    return true;
}

int TheWurmspat::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Blighted Weapons
    if (unmodifiedHitRoll == 6)
    {
        Dice dice;
        // each 6 inflicts D6 hits
        return dice.rollD6();
    }

    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

} // namespace Nurgle