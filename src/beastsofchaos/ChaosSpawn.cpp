/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/ChaosSpawn.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    ChaosSpawn::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ChaosSpawn::MIN_UNIT_SIZE, ChaosSpawn::MIN_UNIT_SIZE,
            ChaosSpawn::MAX_UNIT_SIZE, ChaosSpawn::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool ChaosSpawn::s_registered = false;

ChaosSpawn::ChaosSpawn() :
    BeastsOfChaosBase("Chaos Spawn", RAND_2D6, WOUNDS, 10, 5, false),
    m_freakingMutations(Weapon::Type::Melee, "Freakish Mutations", 1, RAND_2D6, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_SPAWN};
    m_weapons = { &m_freakingMutations };
}

bool ChaosSpawn::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_freakingMutations);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *ChaosSpawn::Create(const ParameterList &parameters)
{
    auto unit = new ChaosSpawn();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosSpawn::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Spawn", factoryMethod);
    }
}

} // namespace BeastsOfChaos