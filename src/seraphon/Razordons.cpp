/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Razordons.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Razordons::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    Razordons::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Razordons::MIN_UNIT_SIZE, Razordons::MIN_UNIT_SIZE,
            Razordons::MAX_UNIT_SIZE, Razordons::MIN_UNIT_SIZE
        },
    },
    ORDER,
    { SERAPHON }
};

bool Razordons::s_registered = false;

Razordons::Razordons() :
    SeraphonBase("Razordons", 8, WOUNDS, 5, 4, false),
    m_spikes(Weapon::Type::Missile, "Volley of Spikes", 18, RAND_2D6, 3, 4, 0, 1),
    m_tail(Weapon::Type::Melee, "Spiked Tail", 1, 3, 3, 3, -2, 2),
    m_goad(Weapon::Type::Melee, "Celestite Goad", 1, 1,4, 5, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, RAZORDON, HUNTING_PACK};
    m_weapons = {&m_spikes, &m_tail, &m_goad};
}

bool Razordons::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto razordon = new Model(BASESIZE_RAZORDON, WOUNDS_RAZORDON);
    razordon->addMissileWeapon(&m_spikes);
    razordon->addMeleeWeapon(&m_tail);
    addModel(razordon);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_goad);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Razordons::Create(const ParameterList &parameters)
{
    auto unit = new Razordons();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Razordons::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Razordons", factoryMethod);
    }
}

int Razordons::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int Razordons::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Piercing Barbs
    if ((weapon->name() == m_spikes.name()) && (distanceTo(m_shootingTarget) <= 6.0f))
    {
        return -1;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

} //namespace Seraphon
