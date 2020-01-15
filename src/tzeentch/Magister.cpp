/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Magister.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    Magister::Create,
    Magister::ValueToString,
    Magister::EnumStringToInt,
    Magister::ComputePoints,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool Magister::s_registered = false;

Unit *Magister::Create(const ParameterList &parameters)
{
    auto unit = new Magister();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Magister::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int Magister::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Magister::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Magister", factoryMethod);
    }
}

Magister::Magister() :
    Unit("Magister", 6, WOUNDS, 7, 5, false),
    m_staff(Weapon::Type::Missile, "Tzeentchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
    m_sword(Weapon::Type::Melee, "Warpsteel Sword", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, MAGISTER};
    m_weapons = {&m_staff,
                 &m_sword};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool Magister::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_staff);
    model->addMeleeWeapon(&m_sword);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch