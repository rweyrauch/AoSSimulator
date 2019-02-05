/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/DreadscytheHarridans.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    DreadscytheHarridans::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = DreadscytheHarridans::MIN_UNIT_SIZE}, DreadscytheHarridans::MIN_UNIT_SIZE,
            DreadscytheHarridans::MAX_UNIT_SIZE, DreadscytheHarridans::MIN_UNIT_SIZE
        },
    }
};

bool DreadscytheHarridans::s_registered = false;

DreadscytheHarridans::DreadscytheHarridans() :
    Unit("Dreadscythe Harridans", 8, WOUNDS, 10, 4, true),
    m_scythedLimbs(Weapon::Type::Melee, "Scythed Limbs", 1, 3, 4, 3, -1, 1),
    m_scythedLimbsCrone(Weapon::Type::Melee, "Scythed Limbs (Crone)", 1, 4, 4, 3, -1, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, DREADSCYTHE_HARRIDANS};
}

bool DreadscytheHarridans::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model crone(BASESIZE, WOUNDS);
    crone.addMeleeWeapon(&m_scythedLimbsCrone);
    addModel(crone);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_scythedLimbs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *DreadscytheHarridans::Create(const ParameterList &parameters)
{
    auto unit = new DreadscytheHarridans();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DreadscytheHarridans::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadscythe Harridans", factoryMethod);
    }
}

int DreadscytheHarridans::toSaveModifier(const Weapon *weapon) const
{
    // Ethereal - no save modifiers allowed.
    int modifier = 0;

    // Ethereal - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

void DreadscytheHarridans::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_scythedLimbs);
    visitor(&m_scythedLimbsCrone);
}

int DreadscytheHarridans::damageModifier(const Weapon *weapon, const Unit *target, const Dice::RollResult &woundRolls) const
{
    int modifier = Unit::damageModifier(weapon, target, woundRolls);
    // Murderous Bloodlust
    if (weapon->name() == m_scythedLimbs.name())
    {
        modifier += woundRolls.numUnmodified6s();
    }
    return modifier;
}

int DreadscytheHarridans::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);

    // Harrowing Shriek
    if ((distanceTo(attacker) <= 3.0f) && (attacker->bravery() < 6))
    {
        modifier -= 1;
    }

    return modifier;
}

} // namespace Nighthaunt
