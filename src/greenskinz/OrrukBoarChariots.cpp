/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukBoarChariots.h>
#include <UnitFactory.h>

namespace Greenskinz
{

bool OrrukBoarChariots::s_registered = false;

OrrukBoarChariots::OrrukBoarChariots() :
    Unit("Orruk Boar Chariots", 9, WOUNDS, 6, 4, false),
    m_pigstikkaSpears(Weapon::Type::Melee, "Crew's Pigstikka Spears", 2, 2, 4, 4, 0, 1),
    m_warBoarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUK_BOAR_CHARIOTS};
    m_weapons = {&m_pigstikkaSpears, &m_warBoarsTusks};
}

bool OrrukBoarChariots::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_pigstikkaSpears);
        model->addMeleeWeapon(&m_warBoarsTusks);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *OrrukBoarChariots::Create(const ParameterList &parameters)
{
    auto unit = new OrrukBoarChariots();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukBoarChariots::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            OrrukBoarChariots::Create,
            nullptr,
            nullptr,
            OrrukBoarChariots::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", OrrukBoarChariots::MIN_UNIT_SIZE, OrrukBoarChariots::MIN_UNIT_SIZE,
                    OrrukBoarChariots::MAX_UNIT_SIZE, OrrukBoarChariots::MIN_UNIT_SIZE
                },
            },
            DESTRUCTION,
            { GREENSKINZ }
        };
        s_registered = UnitFactory::Register("Orruk Boar Chariots", factoryMethod);
    }
}

int OrrukBoarChariots::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    // Tusker Charge
    if (m_charged && weapon->name() == m_warBoarsTusks.name())
    {
        return RerollFailed;
    }
    return Unit::toWoundModifier(weapon, target);
}

void OrrukBoarChariots::onCharged()
{
    // Scythed Wheels
    int roll = Dice::rollD6();
    if (roll >= 4)
    {
        if (m_meleeTarget)
        {
            m_meleeTarget->applyDamage({0, Dice::rollD3()});
        }
    }
    Unit::onCharged();
}

int OrrukBoarChariots::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Greenskinz
