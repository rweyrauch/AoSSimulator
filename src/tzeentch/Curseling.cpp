/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Curseling.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    CurselingEyeOfTzeentch::Create,
    CurselingEyeOfTzeentch::ValueToString,
    CurselingEyeOfTzeentch::EnumStringToInt,
    CurselingEyeOfTzeentch::ComputePoints,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool CurselingEyeOfTzeentch::s_registered = false;

Unit *CurselingEyeOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new CurselingEyeOfTzeentch();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string CurselingEyeOfTzeentch::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int CurselingEyeOfTzeentch::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void CurselingEyeOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Curseling Eye of Tzeentch", factoryMethod);
    }
}

CurselingEyeOfTzeentch::CurselingEyeOfTzeentch() :
    Unit("Curseling Eye of Tzeentch", 5, WOUNDS, 7, 4, false),
    m_sword(Weapon::Type::Melee, "Blazing Sword", 1, 3, 3, 4, -1, 1),
    m_flail(Weapon::Type::Melee, "Threshing Flail", 1, 3, 4, 3, 0, 1),
    m_staff(Weapon::Type::Melee, "Staff of Tzeentch", 2, 1, 5, 4, 0, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, CURSELING};
    m_weapons = {&m_sword, &m_flail, &m_staff};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool CurselingEyeOfTzeentch::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_sword);
    model->addMeleeWeapon(&m_flail);
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch