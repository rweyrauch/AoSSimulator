/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/MagisterOnDisc.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    MagisterOnDiscOfTzeentch::Create,
    MagisterOnDiscOfTzeentch::ValueToString,
    MagisterOnDiscOfTzeentch::EnumStringToInt,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool MagisterOnDiscOfTzeentch::s_registered = false;

Unit *MagisterOnDiscOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new MagisterOnDiscOfTzeentch();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MagisterOnDiscOfTzeentch::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int MagisterOnDiscOfTzeentch::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void MagisterOnDiscOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Magister on Disc of Tzeentch", factoryMethod);
    }
}

MagisterOnDiscOfTzeentch::MagisterOnDiscOfTzeentch() :
    Unit("Magister on Disc of Tzeentch", 16, WOUNDS, 7, 5, true),
    m_staff(Weapon::Type::Missile, "Tzeentchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
    m_sword(Weapon::Type::Melee, "Warpsteel Sword", 1, 1, 4, 4, 0, 1),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, MAGISTER};
    m_weapons = {&m_staff,
                 &m_sword,
                 &m_teethAndHorns};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool MagisterOnDiscOfTzeentch::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_staff);
    model->addMeleeWeapon(&m_sword);
    model->addMeleeWeapon(&m_teethAndHorns);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch