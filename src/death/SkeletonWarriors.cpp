/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/SkeletonWarriors.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Death
{
bool SkeletonWarriors::s_registered = false;

SkeletonWarriors::SkeletonWarriors() :
    LegionOfNagashBase("Skeleton Warriors", 4, WOUNDS, 10, 6, false),
    m_ancientBlade(Weapon::Type::Melee, "Ancient Blade", 1, 1, 4, 4, 0, 1),
    m_ancientBladeChampion(Weapon::Type::Melee, "Ancient Blade", 1, 2, 4, 4, 0, 1),
    m_ancientSpear(Weapon::Type::Melee, "Ancient Spear", 2, 1, 5, 4, 0, 1),
    m_ancientSpearChampion(Weapon::Type::Melee, "Ancient Spear", 2, 2, 5, 4, 0, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, SKELETON_WARRIORS};
    m_weapons = {&m_ancientBlade, &m_ancientBladeChampion, &m_ancientSpear, &m_ancientSpearChampion};
}

bool SkeletonWarriors::configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // TODO: standard bearers debuff enemy Bravery within 6".
    m_standardBearers = standardBearers;
    // TODO: minimum charge distance is 6" with hornblower present
    m_hornblowers = hornblowers;

    auto champion = new Model(BASESIZE, wounds());
    if (weapons == AncientBlade)
    {
        champion->addMeleeWeapon(&m_ancientBladeChampion);
    }
    else if (weapons == AncientSpear)
    {
        champion->addMeleeWeapon(&m_ancientSpearChampion);
    }
    addModel(champion);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        if (weapons == AncientBlade)
        {
            model->addMeleeWeapon(&m_ancientBlade);
        }
        else if (weapons == AncientSpear)
        {
            model->addMeleeWeapon(&m_ancientSpear);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *SkeletonWarriors::Create(const ParameterList &parameters)
{
    auto unit = new SkeletonWarriors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, AncientBlade);
    bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    bool ok = unit->configure(numModels, weapons, standardBearers, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SkeletonWarriors::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Weapons", AncientBlade, AncientBlade, AncientSpear, 1},
                {ParamType::Boolean, "Standard Bearers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Hornblowers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
            },
            DEATH,
            { DEATHRATTLE }
        };
        s_registered = UnitFactory::Register("Skeleton Warriors", factoryMethod);
    }
}

std::string SkeletonWarriors::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == AncientBlade) { return "Ancient Blade"; }
        else if (parameter.intValue == AncientSpear) { return "Ancient Spear"; }
    }
    return ParameterValueToString(parameter);
}

int SkeletonWarriors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ancient Blade") { return AncientBlade; }
    else if (enumString == "Ancient Spear") { return AncientSpear; }
    return 0;
}

int SkeletonWarriors::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Crypt Shields
    if (weapon->rend() == 0)
    {
        modifier += 1;
    }

    return modifier;
}

int SkeletonWarriors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = Unit::extraAttacks(nullptr, weapon, target);
    // Skeleton Legion
    if (remainingModels() >= 30)
    {
        attacks += 2;
    }
    else if (remainingModels() >= 20)
    {
        attacks += 1;
    }
    return attacks;
}

int SkeletonWarriors::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Serve in Death
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
    for (auto ip : units)
    {
        if (ip->hasKeyword(DEATH) && ip->hasKeyword(HERO))
        {
            modifier += 1;
            break;
        }
    }

    return modifier;
}

int SkeletonWarriors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Death
