/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/TerradonChief.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    TerradonChief::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    TerradonChief::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

bool TerradonChief::s_registered = false;

TerradonChief::TerradonChief() :
    SeraphonBase("Terradon Chief", 16, WOUNDS, 6, 6, true),
    m_skyblade(Weapon::Type::Melee, "Skyblade", 1, 3, 3, 4, -1, 1),
    m_jaws(Weapon::Type::Melee, "Razor-sharp Jaws", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, TERRADON, HERO, TERRADON_CHIEF};
    m_weapons = {&m_skyblade, &m_jaws};
}

bool TerradonChief::configure()
{
    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_skyblade);
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    m_points = ComputePoints(1);

    return true;
}

Unit *TerradonChief::Create(const ParameterList &parameters)
{
    auto unit = new TerradonChief();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TerradonChief::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Terradon Chief", factoryMethod);
    }
}

} //namespace Seraphon
