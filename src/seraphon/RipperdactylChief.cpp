/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylChief.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    RipperdactylChief::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    RipperdactylChief::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

bool RipperdactylChief::s_registered = false;

RipperdactylChief::RipperdactylChief() :
    SeraphonBase("Ripperdactyl Chief", 12, WOUNDS, 6, 5, true),
    m_skyblade(Weapon::Type::Melee, "Skyblade", 1, 4, 3, 4, -1, 1),
    m_jaws(Weapon::Type::Melee, "Tearing Jaws", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, HERO, RIPPERDACTYL_CHIEF};
    m_weapons = {&m_skyblade, &m_jaws};
}

bool RipperdactylChief::configure()
{
    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_skyblade);
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    m_points = ComputePoints(1);

    return true;
}

Unit *RipperdactylChief::Create(const ParameterList &parameters)
{
    auto unit = new RipperdactylChief();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RipperdactylChief::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ripperdactyl Chief", factoryMethod);
    }
}

} //namespace Seraphon
