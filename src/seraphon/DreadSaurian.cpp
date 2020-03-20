/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <seraphon/DreadSaurian.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static const int BASESIZE = 280; // x210 oval
static const int WOUNDS = 35;
static const int POINTS_PER_UNIT = 510;

bool DreadSaurian::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_jawsDamage;
    int m_clawsToHit;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {12, 18, 24, 30, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 6, 2},
        {9,  5,  3},
        {8,  4,  3},
        {7,  3,  4},
        {6,  2,  5}
    };

DreadSaurian::DreadSaurian() :
    SeraphonBase("Dread Saurian", 10, WOUNDS, 8, 4, false),
    m_gargantuanJaws(Weapon::Type::Melee, "Gargantuan Jaws", 2, 3, 4, 3, -2, 6),
    m_rakingClaws(Weapon::Type::Melee, "Raking Claws", 2, 6, 2, 3, -1, 2),
    m_armouredTail(Weapon::Type::Melee, "Armoured Tail", 2, 1, 4, 3, -1, RAND_D6)
{
    m_keywords = {ORDER, SERAPHON, MONSTER, TOTEM, DREAD_SAURIAN};
    m_weapons = {&m_gargantuanJaws, &m_rakingClaws, &m_armouredTail};

    s_globalBraveryMod.connect(this, &DreadSaurian::terror, &m_terrorSlot);
}

DreadSaurian::~DreadSaurian()
{
    m_terrorSlot.disconnect();
}

bool DreadSaurian::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_gargantuanJaws);
    model->addMeleeWeapon(&m_rakingClaws);
    model->addMeleeWeapon(&m_armouredTail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void DreadSaurian::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

Unit *DreadSaurian::Create(const ParameterList &parameters)
{
    auto unit = new DreadSaurian();

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DreadSaurian::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Dread Saurian", factoryMethod);
    }
}

void DreadSaurian::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_rakingClaws.setToHit(g_damageTable[damageIndex].m_clawsToHit);
    m_gargantuanJaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int DreadSaurian::getDamageTableIndex() const
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

int DreadSaurian::terror(const Unit *target)
{
    // Terror
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

void DreadSaurian::onCharged()
{
    SeraphonBase::onCharged();

    // Obliterating Charge
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
    for (auto unit : units)
    {
        if (Dice::rollD6() >= 2)
        {
            if (unit->hasKeyword(MONSTER))
                unit->applyDamage({0, Dice::rollD3()});
            else
                unit->applyDamage({0, Dice::rollD6()});
        }
    }
}

void DreadSaurian::onSlain()
{
    Unit::onSlain();

    // Death Throes
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    for (auto unit : units)
    {
        if (!unit->hasKeyword(MONSTER))
        {
            if (Dice::rollD6() >= 4)
                unit->applyDamage({0, Dice::rollD3()});
        }
    }
}

int DreadSaurian::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Seraphon