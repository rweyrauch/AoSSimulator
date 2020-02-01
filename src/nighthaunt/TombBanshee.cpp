/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/TombBanshee.h"

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    TombBanshee::Create,
    nullptr,
    nullptr,
    TombBanshee::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool TombBanshee::s_registered = false;

Unit *TombBanshee::Create(const ParameterList &parameters)
{
    auto unit = new TombBanshee();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
    return nullptr;
}

void TombBanshee::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tomb Banshee", factoryMethod);
    }
}

TombBanshee::TombBanshee() :
    Nighthaunt("Tomb Banshee", 6, WOUNDS, 10, 4, true),
    m_dagger(Weapon::Type::Melee, "Chill Dagger", 1, 1, 4, 3, -2, RAND_D3)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, TOMB_BANSHEE};
    m_weapons = {&m_dagger};
}

bool TombBanshee::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_dagger);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

} // namespace Nighthaunt
