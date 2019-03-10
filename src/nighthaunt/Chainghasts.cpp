/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Chainghasts.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nighthaunt
{
static FactoryMethod factoryMethod = {
    Chainghasts::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Chainghasts::MIN_UNIT_SIZE}, Chainghasts::MIN_UNIT_SIZE,
            Chainghasts::MAX_UNIT_SIZE, Chainghasts::MIN_UNIT_SIZE
        },
    },
    DEATH,
    NIGHTHAUNT
};

bool Chainghasts::s_registered = false;

Chainghasts::Chainghasts() :
    Nighthaunt("Chainghasts", 6, WOUNDS, 10, 4, true),
    m_ghastflailsMissile(Weapon::Type::Missile, "Ghastflails (Missile)", 15, RAND_D3, 4, 3, -2, 1),
    m_ghastflails(Weapon::Type::Melee, "Ghastflails", 2, 0, 4, 3, -1, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, SPIRIT_HOSTS};
}

bool Chainghasts::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_ghastflailsMissile);
        model.addMeleeWeapon(&m_ghastflails);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Chainghasts::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ghastflailsMissile);
    visitor(&m_ghastflails);
}

Unit *Chainghasts::Create(const ParameterList &parameters)
{
    auto unit = new Chainghasts();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Chainghasts::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chainghasts", factoryMethod);
    }
}

int Chainghasts::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);
    // Sweeping Blows
    if (weapon->name() == m_ghastflails.name())
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 2.0f);
        attacks += (int)units.size();
    }
    return attacks;
}

} //namespace Nighthaunt
