/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GiantRats.h>
#include <UnitFactory.h>

namespace Skaven
{
bool GiantRats::s_registered = false;

Unit *GiantRats::Create(const ParameterList &parameters)
{
    auto unit = new GiantRats();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int GiantRats::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void GiantRats::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Giant Rats", *factoryMethod);
    }
}

GiantRats::GiantRats() :
    Skaventide("Giant Rats", 8, WOUNDS, 3, NoSave, false),
    m_teeth(Weapon::Type::Melee, "Vicious Teeth", 1, 1, 4, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, PACK, GIANT_RATS};
    m_weapons = {&m_teeth};
}

bool GiantRats::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }


    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_teeth);
        addModel(model);
    }

    setTeethRange();

    m_points = ComputePoints(numModels);

    return true;
}

void GiantRats::setTeethRange()
{
    if (remainingModels() >= 20)
    {
        m_teeth.setRange(3.0f);
    }
    else if (remainingModels()  >= 10)
    {
        m_teeth.setRange(2.0f);
    }
    else
    {
        m_teeth.setRange(1.0f);
    }
}

} //namespace Skaven
