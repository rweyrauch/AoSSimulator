/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Aetherwings.h>
#include <UnitFactory.h>

namespace StormcastEternals
{
bool Aetherwings::s_registered = false;

Aetherwings::Aetherwings() :
    StormcastEternal("Aetherwings", 12, WOUNDS, 6, NoSave, true),
    m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, AETHERWINGS};
    m_weapons = {&m_beakAndClaws};

    // Swooping Hunters
    m_retreatAndCharge = true;
}

bool Aetherwings::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_beakAndClaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Aetherwings::Create(const ParameterList &parameters)
{
    auto unit = new Aetherwings();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Aetherwings::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Aetherwings", factoryMethod);
    }
}

int Aetherwings::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace StormcastEternals
