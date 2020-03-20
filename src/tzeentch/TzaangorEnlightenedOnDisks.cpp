/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorEnlightenedOnDisks.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static const int BASESIZE = 40;
static const int WOUNDS = 4;
static const int MIN_UNIT_SIZE = 3;
static const int MAX_UNIT_SIZE = 9;
static const int POINTS_PER_BLOCK = 180;
static const int POINTS_MAX_UNIT_SIZE = 540;


bool TzaangorEnlightenedOnDisks::s_registered = false;

TzaangorEnlightenedOnDisks::TzaangorEnlightenedOnDisks() :
    TzeentchBase("Tzaangor Enlightened on Disks", 16, WOUNDS, 6, 5, true),
    m_tzeentchianSpear(Weapon::Type::Melee, "Tzeentchian Spear", 2, 3, 4, 3, -1, 2),
    m_tzeentchianSpearAviarch(Weapon::Type::Melee, "Tzeentchian Spear", 2, 4, 4, 3, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_ENLIGHTENED};
    m_weapons = {&m_tzeentchianSpear, &m_tzeentchianSpearAviarch, &m_viciousBeak, &m_teethAndHorns};
}

bool TzaangorEnlightenedOnDisks::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    auto aviarch = new Model(BASESIZE, wounds());
    aviarch->addMeleeWeapon(&m_tzeentchianSpearAviarch);
    aviarch->addMeleeWeapon(&m_viciousBeak);
    aviarch->addMeleeWeapon(&m_teethAndHorns);
    addModel(aviarch);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_tzeentchianSpear);
        model->addMeleeWeapon(&m_viciousBeak);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *TzaangorEnlightenedOnDisks::Create(const ParameterList &parameters)
{
    auto *unit = new TzaangorEnlightenedOnDisks();
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

void TzaangorEnlightenedOnDisks::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            TzaangorEnlightenedOnDisks::Create,
            TzeentchBase::ValueToString,
            TzeentchBase::EnumStringToInt,
            TzaangorEnlightenedOnDisks::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
            },
            CHAOS,
            { TZEENTCH, BEASTS_OF_CHAOS }
        };
        s_registered = UnitFactory::Register("Tzaangor Enlightened on Disks", factoryMethod);
    }
}

Rerolls TzaangorEnlightenedOnDisks::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Past
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (enemyHasFought) return RerollFailed;

    return TzeentchBase::toHitRerolls(weapon, target);
}

Rerolls TzaangorEnlightenedOnDisks::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Guided by the Past
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    bool enemyHasFought = false;
    for (auto ip : units)
    {
        enemyHasFought |= ip->hasFought();
    }
    if (enemyHasFought) return RerollFailed;

    return TzeentchBase::toWoundRerolls(weapon, target);
}

int TzaangorEnlightenedOnDisks::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch