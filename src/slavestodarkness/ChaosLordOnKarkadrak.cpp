/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/ChaosLordOnKarkadrak.h"

namespace SlavesToDarkness
{

bool ChaosLordOnKarkadrak::s_registered = false;

Unit *ChaosLordOnKarkadrak::Create(const ParameterList &parameters)
{
    auto unit = new ChaosLordOnKarkadrak();

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosLordOnKarkadrak::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            ChaosLordOnKarkadrak::Create,
            SlavesToDarknessBase::ValueToString,
            SlavesToDarknessBase::EnumStringToInt,
            ChaosLordOnKarkadrak::ComputePoints,
            {
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
        };
        s_registered = UnitFactory::Register("Chaos Lord On Karkadrak", factoryMethod);
    }
}

ChaosLordOnKarkadrak::ChaosLordOnKarkadrak() :
    SlavesToDarknessBase("Chaos Lord On Karkadrak", 9, WOUNDS, 8, 3, false),
    m_battleAxe(Weapon::Type::Melee, "Hexed Battle-axe", 1, 5, 3, 3, 0, 2),
    m_blade(Weapon::Type::Melee, "Daemonbound Blade", 1, 3, 3, 3, -1, RAND_D3),
    m_hornsAndClaws(Weapon::Type::Melee, "Tearing Horn and Claws", 1, 3, 4, 3, -1, 2),
    m_tail(Weapon::Type::Melee, "Battering Tail", 1, 2, 4, 4, 0, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD };
    m_weapons = {&m_battleAxe, &m_blade, &m_hornsAndClaws, &m_tail};
}

bool ChaosLordOnKarkadrak::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_battleAxe);
    model->addMeleeWeapon(&m_blade);
    model->addMeleeWeapon(&m_hornsAndClaws);
    model->addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosLordOnKarkadrak::onCharged()
{
    Unit::onCharged();

    // Brutish Rampage
    if (m_charged)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto unit : units)
        {
            if (Dice::rollD6() >= 2)
            {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }
}

Wounds ChaosLordOnKarkadrak::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Daemonbound
    if ((hitRoll == 6) && (weapon->name() == m_blade.name()))
    {
        damage.mortal++;
    }
    return damage;
}

Wounds ChaosLordOnKarkadrak::applyWoundSave(const Wounds &wounds)
{
    auto savedWounds = Unit::applyWoundSave(wounds);
    Dice::RollResult result;
    // Rune-etched Plating
    Dice::rollD6(savedWounds.mortal, result);
    savedWounds.mortal -= result.rollsGE(5);
    return savedWounds;
}

} //namespace SlavesToDarkness
