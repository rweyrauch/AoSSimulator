/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptFlayers.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptFlayers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = CryptFlayers::MIN_UNIT_SIZE}, CryptFlayers::MIN_UNIT_SIZE,
            CryptFlayers::MAX_UNIT_SIZE, CryptFlayers::MIN_UNIT_SIZE
        },
    }
};

bool CryptFlayers::s_registered = false;

CryptFlayers::CryptFlayers() :
    Unit("Crypt Flayers", 12, WOUNDS, 10, 5, true),
    m_fearsomeBattleCry(Weapon::Type::Missile, "Fearsome Battle Cry", 10, 1, 0, 0, 0, 0),
    m_talonsAndClaws(Weapon::Type::Melee, "Piercing Talons and Claws", 1, 4, 4, 3, -1, 1),
    m_talonsAndClawsInfernal(Weapon::Type::Melee, "Piercing Talons and Claws (Crypt Infernal)", 1, 5, 4, 3, -1, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATERS_COURT, CRYPT_FLAYERS};
}

bool CryptFlayers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model infernal(BASESIZE, WOUNDS);
    infernal.addMissileWeapon(&m_fearsomeBattleCry);
    infernal.addMeleeWeapon(&m_talonsAndClawsInfernal);
    addModel(infernal);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_fearsomeBattleCry);
        model.addMeleeWeapon(&m_talonsAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void CryptFlayers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_fearsomeBattleCry);
    visitor(&m_talonsAndClaws);
    visitor(&m_talonsAndClawsInfernal);
}

Unit *CryptFlayers::Create(const ParameterList &parameters)
{
    auto unit = new CryptFlayers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptFlayers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Flayers", factoryMethod);
    }
}

Wounds CryptFlayers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Skewering Strike
    if ((hitRoll == 6) && weapon->name() == m_talonsAndClaws.name())
    {
        // One mortal wound.
        return {0, 1};
    }

    // Fearsome Battle Cry
    if (weapon->name() == m_fearsomeBattleCry.name())
    {
        Dice dice;
        int roll = dice.rollD6() + 1;
        if (roll > target->bravery())
        {
            return {0, roll - target->bravery()};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}


} // namespace FleshEaterCourt
