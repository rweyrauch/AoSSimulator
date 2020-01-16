/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Vortemis.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    VortemisTheAllSeeing::Create,
    VortemisTheAllSeeing::ValueToString,
    VortemisTheAllSeeing::EnumStringToInt,
    VortemisTheAllSeeing::ComputePoints,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool VortemisTheAllSeeing::s_registered = false;

Unit *VortemisTheAllSeeing::Create(const ParameterList &parameters)
{
    auto unit = new VortemisTheAllSeeing();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string VortemisTheAllSeeing::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int VortemisTheAllSeeing::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void VortemisTheAllSeeing::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vortemis the All-seeing", factoryMethod);
    }
}

VortemisTheAllSeeing::VortemisTheAllSeeing() :
    Unit("Vortemis the All-seeing", 6, WOUNDS, 7, 5, false),
    m_staffMissile(Weapon::Type::Missile, "Tzeenchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
    m_staff(Weapon::Type::Melee, "Tzeenchian Runestaff", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, CULT_OF_THE_TRANSIENT_FORM, HERO, WIZARD, MAGISTER, VORTEMIS_THE_ALL_SEEING};
    m_weapons = {&m_staffMissile, &m_staff};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool VortemisTheAllSeeing::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_staffMissile);
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch