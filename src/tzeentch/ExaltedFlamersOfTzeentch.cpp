/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/ExaltedFlamersOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static const int BASESIZE = 75; // x42
static const int WOUNDS = 4;
static const int MIN_UNIT_SIZE = 1;
static const int MAX_UNIT_SIZE = 6;
static const int POINTS_PER_BLOCK = 100;
static const int POINTS_MAX_UNIT_SIZE = 600;

bool ExaltedFlamersOfTzeentch::s_registered = false;

ExaltedFlamersOfTzeentch::ExaltedFlamersOfTzeentch() :
    TzeentchBase("Exalted Flamers of Tzeentch", 9, WOUNDS, 10, 5, true),
    m_warpflame(Weapon::Type::Missile, "Billowing Warpflame", 18, 6, 4, 3, -1, RAND_D3),
    m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 2, 4, 5, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, FLAMER, EXALTED_FLAMERS};
    m_weapons = {&m_warpflame, &m_flamingMaw};
}

bool ExaltedFlamersOfTzeentch::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_warpflame);
        model->addMeleeWeapon(&m_flamingMaw);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *ExaltedFlamersOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new ExaltedFlamersOfTzeentch();
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

void ExaltedFlamersOfTzeentch::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            ExaltedFlamersOfTzeentch::Create,
            TzeentchBase::ValueToString,
            TzeentchBase::EnumStringToInt,
            ExaltedFlamersOfTzeentch::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
            },
            CHAOS,
            { TZEENTCH }
        };
        s_registered = UnitFactory::Register("Exalted Flamers of Tzeentch", factoryMethod);
    }
}

int ExaltedFlamersOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = TzeentchBase::toHitModifier(weapon, target);

    // Capricious Warpflame
    if (target->remainingModels() >= 20) mod += 2;
    else if (target->remainingModels() >= 10) mod++;

    return mod;
}

Wounds ExaltedFlamersOfTzeentch::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto wounds = TzeentchBase::computeReturnedDamage(weapon, saveRoll);

    // Touched by Fire
    Dice::RollResult mortalSaves;
    Dice::rollD6(wounds.mortal, mortalSaves);
    wounds.mortal += mortalSaves.rollsGE(5);

    return wounds;
}

int ExaltedFlamersOfTzeentch::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch