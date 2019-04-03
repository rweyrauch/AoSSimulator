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
            ParamType::Integer, "Models", CryptFlayers::MIN_UNIT_SIZE, CryptFlayers::MIN_UNIT_SIZE,
            CryptFlayers::MAX_UNIT_SIZE, CryptFlayers::MIN_UNIT_SIZE
        },
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool CryptFlayers::s_registered = false;

CryptFlayers::CryptFlayers() :
    FleshEaterCourts("Crypt Flayers", 12, WOUNDS, 10, 5, true),
    m_deathScream(Weapon::Type::Missile, "Death Scream", 10, 1, 0, 0, 0, 0),
    m_piercingTalons(Weapon::Type::Melee, "Piercing Talons", 1, 4, 4, 3, -1, 1),
    m_piercingTalonsInfernal(Weapon::Type::Melee, "Piercing Talons (Crypt Infernal)", 1, 5, 4, 3, -1, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, KNIGHTS, CRYPT_FLAYERS};
}

bool CryptFlayers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model infernal(BASESIZE, WOUNDS);
    infernal.addMissileWeapon(&m_deathScream);
    infernal.addMeleeWeapon(&m_piercingTalonsInfernal);
    addModel(infernal);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_deathScream);
        model.addMeleeWeapon(&m_piercingTalons);
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
    visitor(&m_deathScream);
    visitor(&m_piercingTalons);
    visitor(&m_piercingTalonsInfernal);
}

Unit *CryptFlayers::Create(const ParameterList &parameters)
{
    auto unit = new CryptFlayers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
    if ((hitRoll == 6) && weapon->name() == m_piercingTalons.name())
    {
        // One mortal wound.
        return {0, 1};
    }

    // Death Scream
    if (weapon->name() == m_deathScream.name())
    {
        Dice dice;
        int roll = dice.roll2D6();
        if (distanceTo(target) > 3.0f)
            roll -= 2;
        if (roll > target->bravery())
        {
            return {0, roll - target->bravery()};
        }
    }
    return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
}


} // namespace FleshEaterCourt
