/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/GothizzarHarvester.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    GothizzarHarvester::Create,
    GothizzarHarvester::ValueToString,
    GothizzarHarvester::EnumStringToInt,
    {
        {ParamType::Enum, "Weapon", GothizzarHarvester::Sickles, GothizzarHarvester::Sickles, GothizzarHarvester::Bludgeons, 1},
    },
    DEATH,
    OSSIARCH_BONEREAPERS
};

struct TableEntry
{
    int m_range;
    int m_hoovesToWound;
    int m_attacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, GothizzarHarvester::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 2, 6},
        {14, 3, 5},
        {12, 4, 4},
        {8,  5, 3},
        {4,  6, 2}
    };

bool GothizzarHarvester::s_registered = false;

Unit *GothizzarHarvester::Create(const ParameterList &parameters)
{
    auto unit = new GothizzarHarvester();

    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, GothizzarHarvester::Sickles);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string GothizzarHarvester::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == Sickles) return "Soulcleaver Sickles";
        else if (parameter.m_intValue == Bludgeons) return "Soulcrusher Bludgeons";
    }
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int GothizzarHarvester::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Soulcleaver Sickles") return Sickles;
    else if (enumString == "Soulcrusher Bludgeons") return Bludgeons;
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void GothizzarHarvester::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gothizzar Harvester", factoryMethod);
    }
}

GothizzarHarvester::GothizzarHarvester() :
    OssiarchBonereaperBase("Gothizzar Harvester", 6, WOUNDS, 10, 4, false),
    m_deathsHeadMaw(Weapon::Type::Missile, "Death's Head Maw", 16, 4, 3, 3, -1, 1),
    m_sickles(Weapon::Type::Melee, "Soulcleaver Sickles", 1, 6, 3, 3, -2, 2),
    m_bludgeons(Weapon::Type::Melee, "Soulcrusher Bludgeons", 1, 6, 3, 3, -2, 2),
    m_hoovesAndTail(Weapon::Type::Melee, "Ossified Hooves and Tail", 2, 4, 3, 2, -1, 2)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MONSTER, GOTHIZZAR_HARVESTER};
    m_weapons = {&m_deathsHeadMaw, &m_sickles, &m_bludgeons, &m_hoovesAndTail};
}

bool GothizzarHarvester::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_deathsHeadMaw);
    if (option == Sickles)
        model->addMeleeWeapon(&m_sickles);
    else if (option == Bludgeons)
        model->addMeleeWeapon(&m_bludgeons);
    model->addMeleeWeapon(&m_hoovesAndTail);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void GothizzarHarvester::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_deathsHeadMaw.setRange(g_damageTable[damageIndex].m_range);
    m_hoovesAndTail.setToWound(g_damageTable[damageIndex].m_hoovesToWound);
    m_sickles.setAttacks(g_damageTable[damageIndex].m_attacks);
    m_bludgeons.setAttacks(g_damageTable[damageIndex].m_attacks);

    Unit::onWounded();
}

int GothizzarHarvester::getDamageTableIndex() const
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

int GothizzarHarvester::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Soulcleaver Sickles
    if ((weapon->name() == m_sickles.name()) && (target->remainingModels() >= 5))
    {
        mod++;
    }
    return mod;
}

Wounds GothizzarHarvester::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Soulcrusher Bludgeons
    if ((hitRoll == 6) && (weapon->name() == m_bludgeons.name()))
    {
        return {0, 2};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace OssiarchBonereapers
