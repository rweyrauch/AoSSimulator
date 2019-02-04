/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptHorrors.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptHorrors::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = CryptHorrors::MIN_UNIT_SIZE}, CryptHorrors::MIN_UNIT_SIZE,
         CryptHorrors::MAX_UNIT_SIZE, CryptHorrors::MIN_UNIT_SIZE},
    }
};

bool CryptHorrors::s_registered = false;

CryptHorrors::CryptHorrors() :
    Unit("Crypt Horrors", 7, WOUNDS, 10, 5, false),
    m_clubsAndTalons(Weapon::Type::Melee, "Clubs and Septic Talons", 1, 3, 4, 3, 0, 2),
    m_clubsAndTalonsHaunter(Weapon::Type::Melee, "Clubs and Septic Talons (Crypt Haunter)", 1, 4, 4, 3, 0, 2)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATERS_COURT, CRYPT_HORRORS};
}

bool CryptHorrors::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model haunter(BASESIZE, WOUNDS);
    haunter.addMeleeWeapon(&m_clubsAndTalonsHaunter);
    addModel(haunter);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_clubsAndTalons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void CryptHorrors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_clubsAndTalons);
    visitor(&m_clubsAndTalonsHaunter);
}

Unit *CryptHorrors::Create(const ParameterList &parameters)
{
    auto unit = new CryptHorrors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptHorrors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Horrors", factoryMethod);
    }
}

Rerolls CryptHorrors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Chosen of the King
    auto unit = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, ABHORRANT_GHOUL_KING, 15.0f);
    if (unit != nullptr)
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

void CryptHorrors::hero(PlayerId player)
{
    Unit::hero(player);

    if (player == m_owningPlayer)
    {
        // Noble Blood
        int woundsHealed = heal(remainingModels());
        if (woundsHealed)
        {
            //std::cout << "Healed " << woundsHealed << " from Noble Blood." << std::endl;
        }
    }
}

int CryptHorrors::damageModifier(const Weapon *weapon, const Unit *target, const Dice::RollResult &woundRolls) const
{
    // Warrior Elite - one additional damage for each wound roll of 6.
    return woundRolls.numUnmodified6s();
}

} // namespace FleshEaterCourt
