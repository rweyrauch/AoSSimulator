/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/SorceressOnBlackDragon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SorceressOnBlackDragon::Create,
    SorceressOnBlackDragon::ValueToString,
    SorceressOnBlackDragon::EnumStringToInt,
    SorceressOnBlackDragon::ComputePoints,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
};

struct TableEntry
{
    int m_move;
    int m_jawsToWound;
    int m_clawAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, SorceressOnBlackDragon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 1, 6},
        {12, 2, 5},
        {10, 3, 4},
        {8,  4, 3},
        {6,  5, 2}
    };


bool SorceressOnBlackDragon::s_registered = false;

Unit *SorceressOnBlackDragon::Create(const ParameterList &parameters)
{
    auto unit = new SorceressOnBlackDragon();

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SorceressOnBlackDragon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SorceressOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SorceressOnBlackDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sorceress on Black Dragon", factoryMethod);
    }
}

SorceressOnBlackDragon::SorceressOnBlackDragon() :
    CitizenOfSigmar("Sorceress on Black Dragon", 14, WOUNDS, 7, 5, true),
    m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, 0, 0),
    m_rod(Weapon::Type::Melee, "Witch Rod", 1, 1, 4, 3, -1, RAND_D3),
    m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 3, 4, 4, 0, 1),
    m_lash(Weapon::Type::Melee, "Witch Lash", 2, 1, 3, 4, 0, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 1, -2, RAND_D6),
    m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, MONSTER, HERO, WIZARD, SORCERESS};
    m_weapons = {&m_noxiousBreath, &m_rod, &m_sword, &m_lash, &m_jaws, &m_claws};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool SorceressOnBlackDragon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_noxiousBreath);

    // TODO: weapon options
    model->addMeleeWeapon(&m_rod);

    model->addMeleeWeapon(&m_jaws);
    model->addMeleeWeapon(&m_claws);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void SorceressOnBlackDragon::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

void SorceressOnBlackDragon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
    m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

    Unit::onWounded();
}

int SorceressOnBlackDragon::getDamageTableIndex() const
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

} // namespace CitiesOfSigmar