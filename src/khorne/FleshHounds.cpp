/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/FleshHounds.h>
#include <UnitFactory.h>
#include <iostream>

namespace Khorne
{

bool FleshHounds::s_registered = false;

FleshHounds::FleshHounds() :
    KhorneBase("Flesh Hounds", 8, WOUNDS, 10, 5, false),
    m_burningRoar(Weapon::Type::Missile, "Burning Roar", 8, 1, 2, 4, 0, 1),
    m_blooddarkClaws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, KHORNE, FLESH_HOUNDS};
    m_weapons = {&m_burningRoar, &m_blooddarkClaws};
}

bool FleshHounds::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    int numGoreHounds = numModels / 5;
    // Add the Gore Hounds
    for (auto i = 0; i < numGoreHounds; i++)
    {
        auto goreHound = new Model(BASESIZE, WOUNDS);
        goreHound->addMissileWeapon(&m_burningRoar);
        goreHound->addMeleeWeapon(&m_blooddarkClaws);
        addModel(goreHound);
    }

    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_blooddarkClaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *FleshHounds::Create(const ParameterList &parameters)
{
    auto unit = new FleshHounds();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FleshHounds::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            FleshHounds::Create,
            KhorneBase::ValueToString,
            KhorneBase::EnumStringToInt,
            FleshHounds::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", FleshHounds::MIN_UNIT_SIZE, FleshHounds::MIN_UNIT_SIZE,
                    FleshHounds::MAX_UNIT_SIZE, FleshHounds::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
        s_registered = UnitFactory::Register("Flesh Hounds", factoryMethod);
    }
}

int FleshHounds::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Khorne
