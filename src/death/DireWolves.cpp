/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/DireWolves.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    DireWolves::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", DireWolves::MIN_UNIT_SIZE, DireWolves::MIN_UNIT_SIZE, DireWolves::MAX_UNIT_SIZE,
            DireWolves::MIN_UNIT_SIZE
        },
    },
    DEATH,
    DEADWALKERS
};

bool DireWolves::s_registered = false;

DireWolves::DireWolves() :
    Unit("Dire Wolves", 10, WOUNDS, 10, 5, false),
    m_fangsAndClaws(Weapon::Type::Melee, "Rotting Fangs and Claws", 1, 2, 4, 4, 0, 1),
    m_fangsAndClawsDoom(Weapon::Type::Melee, "Rotting Fangs and Claws (Doom Wolf)", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE, DIRE_WOLVES};
}

bool DireWolves::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    Model doomWolf(BASESIZE, WOUNDS);
    doomWolf.addMeleeWeapon(&m_fangsAndClawsDoom);
    addModel(doomWolf);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_fangsAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void DireWolves::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_fangsAndClaws);
    visitor(&m_fangsAndClawsDoom);
}

Unit *DireWolves::Create(const ParameterList &parameters)
{
    auto unit = new DireWolves();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DireWolves::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dire Wolves", factoryMethod);
    }
}

int DireWolves::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toWoundModifier(weapon, target);

    // Slavering Charge
    if (m_charged)
    { modifier += 1; }

    return modifier;
}

int DireWolves::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Vigour Necris
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
    for (auto ip : units)
    {
        if (ip->hasKeyword(CORPSE_CARTS))
        {
            modifier += 1;
            break;
        }
    }
    return modifier;
}

} //namespace Death
