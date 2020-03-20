/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/SpiritTorment.h"

namespace Nighthaunt
{
static const int BASESIZE = 40;
static const int WOUNDS = 5;
static const int POINTS_PER_UNIT = 120;

bool SpiritTorment::s_registered = false;

Unit *SpiritTorment::Create(const ParameterList &parameters)
{
    auto unit = new SpiritTorment();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}


void SpiritTorment::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            SpiritTorment::Create,
            nullptr,
            nullptr,
            SpiritTorment::ComputePoints,
            {
            },
            DEATH,
            { NIGHTHAUNT }
        };
        s_registered = UnitFactory::Register("Spirit Torment", factoryMethod);
    }
}

SpiritTorment::SpiritTorment() :
    Nighthaunt("Spirit Torment", 6, WOUNDS, 10, 4, true),
    m_chains(Weapon::Type::Melee, "Shacklegheist Chains", 2, 3, 4, 3, -2, RAND_D3)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, SPIRIT_TORMENT};
    m_weapons = {&m_chains};
}

bool SpiritTorment::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_chains);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int SpiritTorment::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace Nighthaunt
