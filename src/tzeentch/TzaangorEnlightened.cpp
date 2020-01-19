/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorEnlightened.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    TzaangorEnlightened::Create,
    TzeentchBase::ValueToString,
    TzeentchBase::EnumStringToInt,
    TzaangorEnlightened::ComputePoints,
    {
        {ParamType::Integer, "Models", TzaangorEnlightened::MIN_UNIT_SIZE, TzaangorEnlightened::MIN_UNIT_SIZE,
         TzaangorEnlightened::MAX_UNIT_SIZE, TzaangorEnlightened::MIN_UNIT_SIZE},
        {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
    },
    CHAOS,
    { TZEENTCH, BEASTS_OF_CHAOS }
};

bool TzaangorEnlightened::s_registered = false;

TzaangorEnlightened::TzaangorEnlightened() :
    TzeentchBase("Tzaangor Enlightened", 6, WOUNDS, 6, 5, false),
    m_tzeentchianSpear(Weapon::Type::Melee, "Tzeentchian Spear", 2, 3, 4, 3, -1, 2),
    m_tzeentchianSpearAviarch(Weapon::Type::Melee, "Tzeentchian Spear", 2, 4, 4, 3, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_ENLIGHTENED};
    m_weapons = {&m_tzeentchianSpear, &m_tzeentchianSpearAviarch, &m_viciousBeak};
}

bool TzaangorEnlightened::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    auto aviarch = new Model(BASESIZE, WOUNDS);
    aviarch->addMeleeWeapon(&m_tzeentchianSpearAviarch);
    aviarch->addMeleeWeapon(&m_viciousBeak);
    addModel(aviarch);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_tzeentchianSpear);
        model->addMeleeWeapon(&m_viciousBeak);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *TzaangorEnlightened::Create(const ParameterList &parameters)
{
    auto *unit = new TzaangorEnlightened();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TzaangorEnlightened::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tzaangor Enlightened", factoryMethod);
    }
}

Rerolls TzaangorEnlightened::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Past
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (enemyHasFought) return RerollFailed;

    return Unit::toHitRerolls(weapon, target);
}

Rerolls TzaangorEnlightened::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Past
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (enemyHasFought) return RerollFailed;

    return Unit::toWoundRerolls(weapon, target);
}

int TzaangorEnlightened::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch