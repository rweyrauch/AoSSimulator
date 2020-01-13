/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigBoss.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBigBoss::Create,
    Bonesplitterz::ValueToString,
    Bonesplitterz::EnumStringToInt,
    {
        {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
    },
    DESTRUCTION,
    { BONESPLITTERZ }
};

bool SavageBigBoss::s_registered = false;

Unit *SavageBigBoss::Create(const ParameterList &parameters)
{
    auto unit = new SavageBigBoss();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageBigBoss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Big Boss", factoryMethod);
    }
}

SavageBigBoss::SavageBigBoss() :
    Bonesplitterz("Savage Big Boss", 5, WOUNDS, 7, 6, false),
    m_chompasBoss(Weapon::Type::Melee, "Boss Chompa", 1, 6, 3, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, SAVAGE_BIG_BOSS};
    m_weapons = { &m_chompasBoss };
}

bool SavageBigBoss::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_chompasBoss);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Bonesplitterz
