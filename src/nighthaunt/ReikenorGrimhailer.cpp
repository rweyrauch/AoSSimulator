/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/ReikenorGrimhailer.h"

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    ReikenorTheGrimhailer::Create,
    nullptr,
    nullptr,
    ReikenorTheGrimhailer::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool ReikenorTheGrimhailer::s_registered = false;

Unit *ReikenorTheGrimhailer::Create(const ParameterList &parameters)
{
    auto unit = new ReikenorTheGrimhailer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ReikenorTheGrimhailer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Reikenor the Grimhailer", factoryMethod);
    }
}

ReikenorTheGrimhailer::ReikenorTheGrimhailer() :
    Nighthaunt("Reikenor the Grimhailer", 14, 7, 10, 4, true),
    m_fellreaper(Weapon::Type::Melee, "Fellreaper", 2, 4, 4, 3, -1, 2),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, REIKENOR_THE_GRIMHAILER};
    m_weapons = {&m_fellreaper, &m_hoovesAndTeeth};
}

bool ReikenorTheGrimhailer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_fellreaper);
    model->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

} // namespace Nighthaunt
