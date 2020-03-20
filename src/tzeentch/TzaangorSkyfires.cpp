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

bool TzaangorSkyfires::s_registered = false;

TzaangorSkyfires::TzaangorSkyfires() :
    TzeentchBase("Tzaangor Skyfires", 16, WOUNDS, 6, 5, true),
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

    auto aviarch = new Model(BASESIZE, wounds());
    aviarch->addMissileWeapon(&m_arrowOfFateAviarch);
    aviarch->addMeleeWeapon(&m_bowStave);
    aviarch->addMeleeWeapon(&m_viciousBeak);
    aviarch->addMeleeWeapon(&m_teethAndHorns);
    addModel(aviarch);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_arrowOfFate);
        model->addMeleeWeapon(&m_bowStave);
        model->addMeleeWeapon(&m_viciousBeak);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *TzaangorSkyfires::Create(const ParameterList &parameters)
{
    auto *unit = new TzaangorSkyfires();
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

void TzaangorSkyfires::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            TzaangorSkyfires::Create,
            TzeentchBase::ValueToString,
            TzeentchBase::EnumStringToInt,
            TzaangorSkyfires::ComputePoints,
            {
                {ParamType::Integer, "Models", TzaangorSkyfires::MIN_UNIT_SIZE, TzaangorSkyfires::MIN_UNIT_SIZE,
                 TzaangorSkyfires::MAX_UNIT_SIZE, TzaangorSkyfires::MIN_UNIT_SIZE},
                {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
            },
            CHAOS,
            { TZEENTCH, BEASTS_OF_CHAOS }
        };
        s_registered = UnitFactory::Register("Tzaangor Skyfires", factoryMethod);
    }
}

Rerolls TzaangorSkyfires::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Future
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (!enemyHasFought) return RerollFailed;

    return TzeentchBase::toHitRerolls(weapon, target);
}

Rerolls TzaangorSkyfires::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Future
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (!enemyHasFought) return RerollFailed;

    return TzeentchBase::toWoundRerolls(weapon, target);
}

Wounds TzaangorSkyfires::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Judgement from Afar
    if ((hitRoll == 6) && (weapon->name() == m_arrowOfFate.name()))
    {
        return { 0, Dice::rollD3() };
    }
    return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int TzaangorSkyfires::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch