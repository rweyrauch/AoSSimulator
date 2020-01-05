/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/ChaosWarhounds.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    ChaosWarhounds::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ChaosWarhounds::MIN_UNIT_SIZE, ChaosWarhounds::MIN_UNIT_SIZE,
            ChaosWarhounds::MAX_UNIT_SIZE, ChaosWarhounds::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool ChaosWarhounds::s_registered = false;

ChaosWarhounds::ChaosWarhounds() :
    BeastsOfChaosBase("Chaos Warhounds", 10, WOUNDS, 4, 6, false),
    m_slaveringJaws(Weapon::Type::Melee, "Slavering Jaws", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, CHAOS_WARHOUNDS};
    m_weapons = { &m_slaveringJaws };
}

bool ChaosWarhounds::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_slaveringJaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *ChaosWarhounds::Create(const ParameterList &parameters)
{
    auto unit = new ChaosWarhounds();
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

void ChaosWarhounds::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Warhounds", factoryMethod);
    }
}

} // namespace BeastsOfChaos