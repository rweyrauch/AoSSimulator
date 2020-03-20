/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/EngineOfTheGods.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 260;

struct TableEntry
{
    int m_move;
    int m_hornDamage;
    int m_stompAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 4, 5},
        {7, 3,  4},
        {6, 2,  3},
        {5, 2,  3},
        {4, 1,  1}
    };

bool EngineOfTheGods::s_registered = false;

EngineOfTheGods::EngineOfTheGods() :
    SeraphonBase("Engine of the Gods", 8, WOUNDS, 6, 4, false),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_horns(Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4),
    m_jaws(Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2),
    m_stomps(Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {ORDER, SERAPHON, STEGADON, SKINK, MONSTER, HERO, ENGINE_OF_THE_GODS};
    m_weapons = {&m_javelins, &m_horns, &m_jaws, &m_stomps};

    s_globalBattleshockReroll.connect(this, &EngineOfTheGods::steadfastMajestyBraveryReroll, &m_steadfastSlot);
}

EngineOfTheGods::~EngineOfTheGods()
{
    m_steadfastSlot.disconnect();
}

bool EngineOfTheGods::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_javelins);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_jaws);
    model->addMeleeWeapon(&m_stomps);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

void EngineOfTheGods::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

Unit *EngineOfTheGods::Create(const ParameterList &parameters)
{
    auto unit = new EngineOfTheGods();

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

void EngineOfTheGods::Init()
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

        s_registered = UnitFactory::Register("Engine of the Gods", factoryMethod);
    }
}

void EngineOfTheGods::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int EngineOfTheGods::getDamageTableIndex() const
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

void EngineOfTheGods::onCharged()
{
    SeraphonBase::onCharged();

    // Unstoppable Stampede
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
    for (auto unit : units)
    {
        if (Dice::rollD6() >= 3)
        {
            unit->applyDamage({0, Dice::rollD3()});
        }
    }
}

void EngineOfTheGods::onStartShooting(PlayerId player)
{
    SeraphonBase::onStartShooting(player);

    if (player != owningPlayer()) return;

    // Cosmic Engine
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SLANN, 12.0f);

    int roll = Dice::roll2D6();
    if (unit)
    {
        roll = Dice::roll3D6();
    }
    if (roll <= 3)
    {
        applyDamage({0, Dice::rollD3()});
    }
    else if (roll <= 8)
    {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
        for (auto u : units)
        {
            if (u->hasKeyword(SERAPHON))
            {
                u->heal(Dice::rollD3());
            }
        }
        heal(Dice::rollD3());
    }
    else if (roll <= 12)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0f);
        if (units.empty())
        {
            units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 24.0f);
            if (!units.empty())
            {
                units.front()->applyDamage({0, Dice::rollD3()});
            }
        }
        else
        {
            for (auto u : units)
            {
                u->applyDamage({0, Dice::rollD3()});
            }
        }
    }
    else if (roll <= 17)
    {
        // TODO: Setup a unit of 10 Saurus Warriors
    }
    else // roll == 18
    {
        // TODO: implement this.
    }
}

Rerolls EngineOfTheGods::steadfastMajestyBraveryReroll(const Unit *unit)
{
    if (isFriendly(unit) && unit->hasKeyword(SKINK) && (distanceTo(unit) <= 18.0f)) return RerollFailed;
    return NoRerolls;
}

int EngineOfTheGods::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Seraphon
