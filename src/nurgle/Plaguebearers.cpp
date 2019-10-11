/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Plaguebearers.h>
#include <UnitFactory.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    Plaguebearers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Plaguebearers::MIN_UNIT_SIZE, Plaguebearers::MIN_UNIT_SIZE,
            Plaguebearers::MAX_UNIT_SIZE, Plaguebearers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Piper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    NURGLE
};

bool Plaguebearers::s_registered = false;

Plaguebearers::Plaguebearers() :
    Unit("Plaguebearers", 4, WOUNDS, 10, 5, false),
    m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
    m_plagueswordPlagueRidden(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUEBEARERS};
}

bool Plaguebearers::configure(int numModels, bool iconBearer, bool pipers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_pipers = pipers;

    // Add the Plagueridden
    Model leader(BASESIZE, WOUNDS);
    leader.addMeleeWeapon(&m_plagueswordPlagueRidden);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_plaguesword);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Plaguebearers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_plaguesword);
    visitor(&m_plagueswordPlagueRidden);
}

Unit *Plaguebearers::Create(const ParameterList &parameters)
{
    auto unit = new Plaguebearers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool pipers = GetBoolParam("Pipers", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, pipers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Plaguebearers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Plaguebearers", factoryMethod);
    }
}

Wounds Plaguebearers::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Disgustingly Resilient
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

int Plaguebearers::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    // Cloud of Flies
    int modifier = Unit::targetHitModifier(weapon, attacker);
    if (remainingModels() >= 20)
    {
        modifier -= 2;
    }
    else
    {
        modifier -= 1;
    }
    return modifier;
}

} // namespace Nurgle

