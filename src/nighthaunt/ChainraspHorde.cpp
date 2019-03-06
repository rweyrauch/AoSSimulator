/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/ChainraspHorde.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    ChainraspHorde::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = ChainraspHorde::MIN_UNIT_SIZE}, ChainraspHorde::MIN_UNIT_SIZE,
            ChainraspHorde::MAX_UNIT_SIZE, ChainraspHorde::MIN_UNIT_SIZE
        },
    },
    DEATH,
    NIGHTHAUNT
};

bool ChainraspHorde::s_registered = false;

ChainraspHorde::ChainraspHorde() :
    Nighthaunt("Chainrasp Horde", 6, WOUNDS, 10, 5, true), // todo: bravery 6 when no Dreadwarden
    m_malignantWeapon(Weapon::Type::Melee, "Malignant Weapon", 1, 2, 4, 4, 0, 1),
    m_malignantWeaponWarden(Weapon::Type::Melee, "Malignant Weapon (Warden)", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, CHAINRASP_HORDE};
}

bool ChainraspHorde::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model warden(BASESIZE, WOUNDS);
    warden.addMeleeWeapon(&m_malignantWeaponWarden);
    addModel(warden);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_malignantWeapon);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *ChainraspHorde::Create(const ParameterList &parameters)
{
    auto unit = new ChainraspHorde();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChainraspHorde::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chainrasp Horde", factoryMethod);
    }
}

Rerolls ChainraspHorde::toWoundRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Chilling Horde
    if (remainingModels() >= 10)
    {
        return RerollOnes;
    }

    return Nighthaunt::toHitRerolls(weapon, unit);
}

void ChainraspHorde::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_malignantWeapon);
    visitor(&m_malignantWeaponWarden);
}

} // namespace Nighthaunt
