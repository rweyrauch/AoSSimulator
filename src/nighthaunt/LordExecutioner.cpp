/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/LordExecutioner.h"

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    LordExecutioner::Create,
    nullptr,
    nullptr,
    LordExecutioner::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool LordExecutioner::s_registered = false;

Unit *LordExecutioner::Create(const ParameterList &parameters)
{
    auto unit = new LordExecutioner();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
    return nullptr;
}

void LordExecutioner::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord Executioner", factoryMethod);
    }
}

LordExecutioner::LordExecutioner() :
    Nighthaunt("Lord Executioner", 6, WOUNDS, 10, 4, true),
    m_greataxe(Weapon::Type::Melee, "Decapitating Greataxe", 1, 3, 3, 3, -2, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, LORD_EXECUTIONER};
    m_weapons = {&m_greataxe};
}

bool LordExecutioner::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_greataxe);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

} // namespace Nighthaunt
