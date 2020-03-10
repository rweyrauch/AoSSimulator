/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/FrostlordStonehorn.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    FrostlordOnStonehorn::Create,
    FrostlordOnStonehorn::ValueToString,
    FrostlordOnStonehorn::EnumStringToInt,
    FrostlordOnStonehorn::ComputePoints,
    {
        {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
    },
    DESTRUCTION,
    {OGOR_MAWTRIBES}
};

struct TableEntry
{
    int m_move;
    int m_hornsAttacks;
    int m_hoovesToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, FrostlordOnStonehorn::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 6, 2},
        {10,  5, 3},
        {8,  4, 3},
        {6,  3, 4},
        {4,  2, 4}
    };

bool FrostlordOnStonehorn::s_registered = false;

Unit *FrostlordOnStonehorn::Create(const ParameterList &parameters)
{
    auto unit = new FrostlordOnStonehorn();

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FrostlordOnStonehorn::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int FrostlordOnStonehorn::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

void FrostlordOnStonehorn::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Frostlord on Stonehorn", factoryMethod);
    }
}

FrostlordOnStonehorn::FrostlordOnStonehorn() :
    MawtribesBase("Frostlord on Stonehorn", 12, WOUNDS, 9, 3, false),
    m_spear(Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3),
    m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1),
    m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
    m_hooves(Weapon::Type::Melee, "Crushing Hooves", 2, RAND_D6, 3, 2, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, FROSTLORD};
    m_weapons = {&m_spear, &m_kicks, &m_horns, &m_hooves};
}

bool FrostlordOnStonehorn::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMeleeWeapon(&m_spear);
    model->addMeleeWeapon(&m_kicks);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_hooves);

    addModel(model);

    m_points = FrostlordOnStonehorn::ComputePoints(1);

    return true;
}

void FrostlordOnStonehorn::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

int FrostlordOnStonehorn::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

void FrostlordOnStonehorn::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
    m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

    MawtribesBase::onWounded();
}

Wounds FrostlordOnStonehorn::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Earth-shattering Charge
    if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name()))
    {
        return { weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds FrostlordOnStonehorn::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Stone Skeleton
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} // namespace OgorMawtribes
