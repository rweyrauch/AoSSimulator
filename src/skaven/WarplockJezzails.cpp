/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarplockJezzails.h>
#include <UnitFactory.h>

namespace Skaven
{
bool WarplockJezzails::s_registered = false;

Unit *WarplockJezzails::Create(const ParameterList &parameters)
{
    auto unit = new WarplockJezzails();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int WarplockJezzails::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void WarplockJezzails::Init()
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

        s_registered = UnitFactory::Register("Warplock Jezzails", *factoryMethod);
    }
}

WarplockJezzails::WarplockJezzails() :
    Skaventide("Warplock Jezzails", 6, WOUNDS, 4, 6, false),
    m_jezzail(Weapon::Type::Missile, "Warplock Jezzail", 30, 1, 4, 3, -2, 2),
    m_knives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WARPLOCK_JEZZAILS};
    m_weapons = {&m_jezzail, &m_knives};
}

bool WarplockJezzails::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_jezzail);
        model->addMeleeWeapon(&m_knives);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}

Wounds WarplockJezzails::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Warpstone Snipers
    if ((hitRoll == 6) && (weapon->name() == m_jezzail.name()))
    {
        return {0, 2};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls WarplockJezzails::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Pavise
    if (!m_moved && (weapon->name() == m_jezzail.name()))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

int WarplockJezzails::toSaveModifier(const Weapon *weapon) const
{
    auto mod = Unit::toSaveModifier(weapon);

    // Pavise
    if (weapon->isMissile())
    {
        mod += 2;
    }

    return mod;
}

} //namespace Skaven
