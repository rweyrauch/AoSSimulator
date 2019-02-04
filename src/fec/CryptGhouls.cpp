/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptGhouls.h>
#include <UnitFactory.h>
#include <Board.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptGhouls::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = CryptGhouls::MIN_UNIT_SIZE}, CryptGhouls::MIN_UNIT_SIZE,
         CryptGhouls::MAX_UNIT_SIZE, CryptGhouls::MIN_UNIT_SIZE},
    }
};

bool CryptGhouls::s_registered = false;

CryptGhouls::CryptGhouls() :
    Unit("Crypt Ghouls", 6, WOUNDS, 10, 6, false),
    m_teethAndClaws(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws", 1, 2, 4, 4, 0, 1),
    m_teethAndClawsGhast(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws (Crypt Ghast)", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATERS_COURT, CRYPT_GHOULS};
}

bool CryptGhouls::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model ghast(BASESIZE, WOUNDS);
    ghast.addMeleeWeapon(&m_teethAndClawsGhast);
    addModel(ghast);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_teethAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void CryptGhouls::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_teethAndClaws);
    visitor(&m_teethAndClawsGhast);
}

Unit *CryptGhouls::Create(const ParameterList &parameters)
{
    auto unit = new CryptGhouls();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptGhouls::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Ghouls", factoryMethod);
    }
}

int CryptGhouls::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);

    // Battalion Strength
    if (remainingModels() >= 20)
        attacks += 1;

    return attacks;
}

Rerolls CryptGhouls::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Royal Approval
    auto unit = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, ABHORRANT_GHOUL_KING, 15.0f);
    if (unit != nullptr)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace FleshEaterCourt
