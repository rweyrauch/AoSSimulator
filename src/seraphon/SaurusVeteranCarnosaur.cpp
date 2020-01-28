/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusVeteranCarnosaur.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusScarVeteranOnCarnosaur::Create,
    SaurusScarVeteranOnCarnosaur::ValueToString,
    SaurusScarVeteranOnCarnosaur::EnumStringToInt,
    SaurusScarVeteranOnCarnosaur::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_move;
    int m_forelimbToHit;
    int m_jawsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SaurusScarVeteranOnCarnosaur::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 3, 5},
        {10,  4,  4},
        {8,  4,  3},
        {8,  5,  2},
        {6,  5,  1}
    };

bool SaurusScarVeteranOnCarnosaur::s_registered = false;

SaurusScarVeteranOnCarnosaur::SaurusScarVeteranOnCarnosaur() :
    SeraphonBase("Saurus Scar-Veteran on Carnosaur", 10, WOUNDS, 10, 4, false),
    m_warblade(Weapon::Type::Melee, "Celestite Warblade", 1, 6, 3, 3, 0, 1),
    m_warspear(Weapon::Type::Melee, "Celestite War-spear", 2, 6, 4, 3, -1, 1),
    m_greatblade(Weapon::Type::Melee, "Celestite Greatblade", 1, 3, 4, 3, -1, 2),
    m_jawsAndShield(Weapon::Type::Melee, "Fearsome Jaws and Stardrake Shield", 1, 1, 4, 3, 0, 1),
    m_forelimbs(Weapon::Type::Melee, "Carnosaur's Clawed Forelimbs", 2, 2,3, 3, 0, 2),
    m_jaws(Weapon::Type::Melee, "Carnosaur's Massive Jaws", 2, 5, 4, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, CARNOSAUR, SAURUS, MONSTER, HERO, SAURUS_OLDBLOOD};
    m_weapons = {&m_warblade, &m_warspear, &m_greatblade, &m_jawsAndShield, &m_forelimbs, &m_jaws};
}

bool SaurusScarVeteranOnCarnosaur::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (option == CelestiteWarblade) model->addMeleeWeapon(&m_warblade);
    else if (option == CelestiteWarspear) model->addMeleeWeapon(&m_warspear);
    else if (option == CelestiteGreatblade) model->addMeleeWeapon(&m_greatblade);
    model->addMeleeWeapon(&m_forelimbs);
    model->addMeleeWeapon(&m_jawsAndShield);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int SaurusScarVeteranOnCarnosaur::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *SaurusScarVeteranOnCarnosaur::Create(const ParameterList &parameters)
{
    auto unit = new SaurusScarVeteranOnCarnosaur();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, CelestiteWarblade);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SaurusScarVeteranOnCarnosaur::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == CelestiteWarblade) return "Celestite Warblade";
        else if (parameter.intValue == CelestiteWarspear) return "Celestite War-spear";
        else if (parameter.intValue == CelestiteGreatblade) return "Celestite Greatblade";
    }
    return SeraphonBase::ValueToString(parameter);
}

int SaurusScarVeteranOnCarnosaur::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Celestite Warblade") return CelestiteWarblade;
    else if (enumString == "Celestite War-spear") return CelestiteWarspear;
    else if (enumString == "Celestite Greatblade") return CelestiteGreatblade;
    return SeraphonBase::EnumStringToInt(enumString);
}

void SaurusScarVeteranOnCarnosaur::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Scar-Veteran on Carnosaur", factoryMethod);
    }
}

void SaurusScarVeteranOnCarnosaur::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_jaws.setAttacks(g_damageTable[damageIndex].m_jawsAttacks);
    m_forelimbs.setToHit(g_damageTable[damageIndex].m_forelimbToHit);
}

int SaurusScarVeteranOnCarnosaur::getDamageTableIndex() const
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

} //namespace Seraphon