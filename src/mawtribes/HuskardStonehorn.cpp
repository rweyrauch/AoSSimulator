/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/HuskardStonehorn.h"

namespace OgorMawtribes
{
static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 320;

struct TableEntry
{
    int m_move;
    int m_hornsAttacks;
    int m_hoovesToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 6, 2},
        {10,  5, 3},
        {8,  4, 3},
        {6,  3, 4},
        {4,  2, 4}
    };

bool HuskardOnStonehorn::s_registered = false;

Unit *HuskardOnStonehorn::Create(const ParameterList &parameters)
{
    auto unit = new HuskardOnStonehorn();

    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, HarpoonLauncher);

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string HuskardOnStonehorn::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == HarpoonLauncher) return "Harpoon Launcher";
        else if (parameter.intValue == Chaintrap) return "Chaintrap";
        else if (parameter.intValue == BloodVulture) return "Blood Vulture";
    }
    return MawtribesBase::ValueToString(parameter);
}

int HuskardOnStonehorn::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Harpoon Launcher") return HarpoonLauncher;
    else if (enumString == "Chaintrap") return Chaintrap;
    else if (enumString == "Blood Vulture") return BloodVulture;

    return MawtribesBase::EnumStringToInt(enumString);
}

void HuskardOnStonehorn::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            HuskardOnStonehorn::Create,
            HuskardOnStonehorn::ValueToString,
            HuskardOnStonehorn::EnumStringToInt,
            HuskardOnStonehorn::ComputePoints,
            {
                {ParamType::Enum, "Weapon", HuskardOnStonehorn::HarpoonLauncher, HuskardOnStonehorn::HarpoonLauncher, HuskardOnStonehorn::BloodVulture, 1},
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            {OGOR_MAWTRIBES}
        };
        s_registered = UnitFactory::Register("Huskard on Stonehorn", factoryMethod);
    }
}

HuskardOnStonehorn::HuskardOnStonehorn() :
    MawtribesBase("Huskard on Stonehorn", 12, WOUNDS, 9, 3, false),
    m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
    m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
    m_vulture(Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0),
    m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 4, 0, 1),
    m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
    m_hooves(Weapon::Type::Melee, "Crushing Hooves", 1, RAND_D6, 3, 2, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, HUSKARD};
    m_weapons = {&m_harpoon, &m_chaintrap, &m_kicks, &m_horns, &m_hooves};
}

bool HuskardOnStonehorn::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, wounds());

    m_option = option;

    if (option == HarpoonLauncher)
        model->addMissileWeapon(&m_harpoon);
    else if (option == Chaintrap)
        model->addMissileWeapon(&m_chaintrap);
    else if (option == BloodVulture)
        model->addMissileWeapon(&m_vulture);
    model->addMeleeWeapon(&m_kicks);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_hooves);

    addModel(model);

    m_points = HuskardOnStonehorn::ComputePoints(1);

    return true;
}

void HuskardOnStonehorn::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

int HuskardOnStonehorn::getDamageTableIndex() const
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

void HuskardOnStonehorn::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
    m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

    MawtribesBase::onWounded();
}

Wounds HuskardOnStonehorn::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Earth-shattering Charge
    if (m_charged && (weapon->name() == m_horns.name() || weapon->name() == m_hooves.name()))
    {
        return { weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds HuskardOnStonehorn::applyWoundSave(const Wounds &wounds)
{
    // Stone Skeleton
    Dice::RollResult woundSaves, mortalSaves;
    Dice::rollD6(wounds.normal, woundSaves);
    Dice::rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

void HuskardOnStonehorn::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (player == owningPlayer())
    {
        if (m_meleeTarget)
        {
            if (m_option == BloodVulture)
            {
                if (Dice::rollD6() >= 2)
                {
                    m_meleeTarget->applyDamage({0, 1});
                }
            }
        }
    }
}

int HuskardOnStonehorn::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OgorMawtribes
