/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylRiders.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    RipperdactylRiders::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    RipperdactylRiders::ComputePoints,
    {
        {
            ParamType::Integer, "Models", RipperdactylRiders::MIN_UNIT_SIZE, RipperdactylRiders::MIN_UNIT_SIZE,
            RipperdactylRiders::MAX_UNIT_SIZE, RipperdactylRiders::MIN_UNIT_SIZE
        },
    },
    ORDER,
    { SERAPHON }
};

bool RipperdactylRiders::s_registered = false;

RipperdactylRiders::RipperdactylRiders() :
    SeraphonBase("Ripperdactyl Riders", 14, WOUNDS, 10, 5, true),
    m_spear(Weapon::Type::Melee, "Moonstone War-spear", 2, 1, 4, 4, 0, 1),
    m_spearAlpha(Weapon::Type::Melee, "Moonstone War-spear", 2, 2, 4, 4, 0, 1),
    m_claws(Weapon::Type::Melee, "Ripperdactyl's Slashing Claws", 1, 3, 3, 3, 0, 1),
    m_beak(Weapon::Type::Melee, "Ripperdactyl's Vicious Beak", 1, 1, 4, 3, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINK, RIPPERDACTYL_RIDERS};
    m_weapons = {&m_spear, &m_spearAlpha, &m_claws, &m_beak};
}

bool RipperdactylRiders::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_spearAlpha);
    alpha->addMeleeWeapon(&m_claws);
    alpha->addMeleeWeapon(&m_beak);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_beak);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *RipperdactylRiders::Create(const ParameterList &parameters)
{
    auto unit = new RipperdactylRiders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RipperdactylRiders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ripperdactyl Riders", factoryMethod);
    }
}

int RipperdactylRiders::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Seraphon
