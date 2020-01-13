/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorSkyfires.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    TzaangorSkyfires::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", TzaangorSkyfires::MIN_UNIT_SIZE, TzaangorSkyfires::MIN_UNIT_SIZE,
         TzaangorSkyfires::MAX_UNIT_SIZE, TzaangorSkyfires::MIN_UNIT_SIZE},
    },
    CHAOS,
    { TZEENTCH, BEASTS_OF_CHAOS }
};

bool TzaangorSkyfires::s_registered = false;

TzaangorSkyfires::TzaangorSkyfires() :
    Unit("Tzaangor Skyfires", 16, WOUNDS, 6, 5, true),
    m_arrowOfFate(Weapon::Type::Missile, "Arrow of Fate", 24, 1, 4, 3, -1, RAND_D3),
    m_arrowOfFateAviarch(Weapon::Type::Missile, "Arrow of Fate", 24, 1, 3, 3, -1, RAND_D3),
    m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 2, 5, 5, 0, 1),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_SKYFIRES};
    m_weapons = {&m_arrowOfFate, &m_arrowOfFateAviarch, &m_bowStave, &m_viciousBeak, &m_teethAndHorns};
}

bool TzaangorSkyfires::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    auto aviarch = new Model(BASESIZE, WOUNDS);
    aviarch->addMissileWeapon(&m_arrowOfFateAviarch);
    aviarch->addMeleeWeapon(&m_bowStave);
    aviarch->addMeleeWeapon(&m_viciousBeak);
    aviarch->addMeleeWeapon(&m_teethAndHorns);
    addModel(aviarch);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_arrowOfFate);
        model->addMeleeWeapon(&m_bowStave);
        model->addMeleeWeapon(&m_viciousBeak);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *TzaangorSkyfires::Create(const ParameterList &parameters)
{
    auto *unit = new TzaangorSkyfires();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TzaangorSkyfires::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tzaangor Skyfires", factoryMethod);
    }
}

Rerolls TzaangorSkyfires::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Future
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (!enemyHasFought) return RerollFailed;

    return Unit::toHitRerolls(weapon, target);
}

Rerolls TzaangorSkyfires::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Future
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (!enemyHasFought) return RerollFailed;

    return Unit::toWoundRerolls(weapon, target);
}

Wounds TzaangorSkyfires::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Judgement from Afar
    if ((hitRoll == 6) && (weapon->name() == m_arrowOfFate.name()))
    {
        Dice dice;
        return { 0, dice.rollD3() };
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Tzeentch