/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostSabres.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    FrostSabres::Create,
    MawtribesBase::ValueToString,
    MawtribesBase::EnumStringToInt,
    FrostSabres::ComputePoints,
    {
        {
            ParamType::Integer, "Models", FrostSabres::MIN_UNIT_SIZE, FrostSabres::MIN_UNIT_SIZE,
            FrostSabres::MAX_UNIT_SIZE, FrostSabres::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool FrostSabres::s_registered = false;

Unit *FrostSabres::Create(const ParameterList &parameters)
{
    auto unit = new FrostSabres();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FrostSabres::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Frost Sabres", factoryMethod);
    }
}

FrostSabres::FrostSabres() :
    MawtribesBase("Frost Sabres", 9, WOUNDS, 5, 6, false),
    m_fangs(Weapon::Type::Melee, "Elongated Fangs", 1, 3, 4, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, FROST_SABRES};
    m_weapons = {&m_fangs};
}

bool FrostSabres::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_fangs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int FrostSabres::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace OgorMawtribes
