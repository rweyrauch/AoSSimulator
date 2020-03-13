/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/ThundertuskBeastriders.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    ThundertuskBeastriders::Create,
    ThundertuskBeastriders::ValueToString,
    ThundertuskBeastriders::EnumStringToInt,
    ThundertuskBeastriders::ComputePoints,
    {
        {ParamType::Enum, "Weapon", ThundertuskBeastriders::Chaintrap, ThundertuskBeastriders::Chaintrap, ThundertuskBeastriders::BloodVulture, 1},
        {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
    },
    DESTRUCTION,
    {OGOR_MAWTRIBES}
};

struct TableEntry
{
    int m_move;
    int m_ice;
    int m_tusksToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ThundertuskBeastriders::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 12, 2},
        {7,  10, 3},
        {6,  8, 3},
        {5,  6, 4},
        {4,  4, 4}
    };

bool ThundertuskBeastriders::s_registered = false;

Unit *ThundertuskBeastriders::Create(const ParameterList &parameters)
{
    auto unit = new ThundertuskBeastriders();

    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, Chaintrap);

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

std::string ThundertuskBeastriders::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == Chaintrap) return "Chaintrap";
        else if (parameter.intValue == BloodVulture) return "Blood Vulture";
    }
    return MawtribesBase::ValueToString(parameter);
}

int ThundertuskBeastriders::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Chaintrap") return Chaintrap;
    else if (enumString == "Blood Vulture") return BloodVulture;

    return MawtribesBase::EnumStringToInt(enumString);
}

void ThundertuskBeastriders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Thundertusk Beastriders", factoryMethod);
    }
}

ThundertuskBeastriders::ThundertuskBeastriders() :
    MawtribesBase("Thundertusk Beastriders", 8, WOUNDS, 7, 4, false),
    m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
    m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
    m_vulture(Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0),
    m_ice(Weapon::Type::Missile, "Frost-wreathed Ice", 18, 0, 0, 0, 0, 0),
    m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 6, 4, 4, 0, 1),
    m_tusks(Weapon::Type::Melee, "Colossal Tusks", 2, 4, 3, 2, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, OGOR, THUNDERTUSK, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, THUNDERTUSK_BEASTRIDERS};
    m_weapons = {&m_harpoon, &m_chaintrap, &m_ice, &m_kicks, &m_tusks};
}

bool ThundertuskBeastriders::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, wounds());

    m_option = option;

    model->addMissileWeapon(&m_harpoon);
    if (option == Chaintrap)
        model->addMissileWeapon(&m_chaintrap);
    else if (option == BloodVulture)
        model->addMissileWeapon(&m_vulture);
    model->addMissileWeapon(&m_ice);
    model->addMeleeWeapon(&m_kicks);
    model->addMeleeWeapon(&m_tusks);

    addModel(model);

    m_points = ThundertuskBeastriders::ComputePoints(1);

    return true;
}

void ThundertuskBeastriders::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

int ThundertuskBeastriders::getDamageTableIndex() const
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

void ThundertuskBeastriders::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_tusks.setToWound(g_damageTable[damageIndex].m_tusksToWound);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

    MawtribesBase::onWounded();
}

int ThundertuskBeastriders::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    auto mod = Unit::targetHitModifier(weapon, attacker);
    // Numbing Chill
    if (!weapon->isMissile()) mod--;

    return mod;
}

void ThundertuskBeastriders::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);


    if (player == owningPlayer())
    {
        if (m_meleeTarget)
        {
            Dice::RollResult result;
            Dice::rollD6(g_damageTable[getDamageTableIndex()].m_ice, result);
            int toWound = 6;
            if (m_meleeTarget->remainingModels() >= 20) toWound -= 2;
            else if (m_meleeTarget->remainingModels() >= 10) toWound -= 1;

            Wounds wounds = { 0, result.rollsGE(toWound) };
            m_meleeTarget->applyDamage(wounds);

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

} // namespace OgorMawtribes
