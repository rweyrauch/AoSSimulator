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

static FactoryMethod factoryMethod = {
    SkeletonWarriors::Create,
    SkeletonWarriors::ValueToString,
    SkeletonWarriors::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = SkeletonWarriors::MIN_UNIT_SIZE}, SkeletonWarriors::MIN_UNIT_SIZE, SkeletonWarriors::MAX_UNIT_SIZE,
            SkeletonWarriors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "standardBearers", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false, false},
    },
    DEATH,
    DEATHRATTLE
};

bool SkeletonWarriors::s_registered = false;

SkeletonWarriors::SkeletonWarriors() :
    Unit("Skeleton Warriors", 4, WOUNDS, 10, 6, false),
    m_ancientBlade(Weapon::Type::Melee, "Ancient Blade", 1, 1, 4, 4, 0, 1),
    m_ancientBladeChampion(Weapon::Type::Melee, "Ancient Blade (Champion)", 1, 2, 4, 4, 0, 1),
    m_ancientSpear(Weapon::Type::Melee, "Ancient Spear", 2, 1, 5, 4, 0, 1),
    m_ancientSpearChampion(Weapon::Type::Melee, "Ancient Spear (Champion)", 2, 2, 5, 4, 0, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, SKELETON_WARRIORS};
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

    Model champion(BASESIZE, WOUNDS);
    if (weapons == AncientBlade)
    {
        champion.addMeleeWeapon(&m_ancientBladeChampion);
    }
    else if (weapons == AncientSpear)
    {
        champion.addMeleeWeapon(&m_ancientSpearChampion);
    }
    addModel(champion);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == AncientBlade)
        {
            model.addMeleeWeapon(&m_ancientBlade);
        }
        else if (weapons == AncientSpear)
        {
            model.addMeleeWeapon(&m_ancientSpear);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SkeletonWarriors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ancientBlade);
    visitor(&m_ancientBladeChampion);
    visitor(&m_ancientSpear);
    visitor(&m_ancientSpearChampion);
}

Unit *SkeletonWarriors::Create(const ParameterList &parameters)
{
    auto unit = new SkeletonWarriors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetEnumParam("weapons", parameters, AncientBlade);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

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
        s_registered = UnitFactory::Register("Skeleton Warriors", factoryMethod);
    }
}

std::string SkeletonWarriors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == AncientBlade)
        { return "AncientBlade"; }
        else if (parameter.m_intValue == AncientSpear)
        { return "AncientSpear"; }
    }
    return ParameterValueToString(parameter);
}

int SkeletonWarriors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "AncientBlade")
    { return AncientBlade; }
    else if (enumString == "AncientSpear")
    { return AncientSpear; }
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

int SkeletonWarriors::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);
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

} //namespace Death
