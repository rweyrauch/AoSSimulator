/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/GauntSummoner.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    GauntSummonerOfTzeentch::Create,
    GauntSummonerOfTzeentch::ValueToString,
    GauntSummonerOfTzeentch::EnumStringToInt,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool GauntSummonerOfTzeentch::s_registered = false;

Unit *GauntSummonerOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new GauntSummonerOfTzeentch();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string GauntSummonerOfTzeentch::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int GauntSummonerOfTzeentch::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void GauntSummonerOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gaunt Summoner of Tzeentch", factoryMethod);
    }
}

GauntSummonerOfTzeentch::GauntSummonerOfTzeentch() :
    Unit("Gaunt Summoner of Tzeentch", 5, WOUNDS, 8, 6, false),
    m_staff(Weapon::Type::Missile, "Changestaff", 18, 1, 3, 4, 0, RAND_D3),
    m_blade(Weapon::Type::Melee, "Warptongue Blade", 1, 1, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, MORTAL, TZEENTCH, ARCANITE, SLAVES_TO_DARKNESS, EVERCHOSEN, HERO, WIZARD, GAUNT_SUMMONER};
    m_weapons = {&m_staff, &m_blade};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool GauntSummonerOfTzeentch::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_staff);
    model->addMeleeWeapon(&m_blade);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds GauntSummonerOfTzeentch::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Warptongue Blade
    if ((woundRoll == 6) && (weapon->name() == m_blade.name()))
    {
        Dice dice;
        return { 0, dice.rollD6() };
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // Tzeentch