/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeerScreamingBell.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>

namespace Skaven
{
struct TableEntry
{
    int m_move;
    int m_spikesToHit;
    int m_pealRange;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, GreySeerOnScreamingBell::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 2, 26},
        {6, 3, 22},
        {4, 4, 18},
        {4,  4, 14},
        {3,  5, 10}
    };


bool GreySeerOnScreamingBell::s_registered = false;

Unit *GreySeerOnScreamingBell::Create(const ParameterList &parameters)
{
    auto unit = new GreySeerOnScreamingBell();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GreySeerOnScreamingBell::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Grey Seer on Screaming Bell", *factoryMethod);
    }
}

GreySeerOnScreamingBell::GreySeerOnScreamingBell() :
    Skaventide("Grey Seer on Screaming Bell", 6, WOUNDS, 6, 4, false),
    m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 3, 4, 4, -1, 1),
    m_clawsAndFangs(Weapon::Type::Melee, "Tearing Claws and Fangs", 1, 4, 4, 3, -1, 2),
    m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, WAR_MACHINE, HERO, WIZARD, SCREAMING_BELL,
        GREY_SEER};
    m_weapons = {&m_staff, &m_clawsAndFangs, &m_spikes};

    s_globalBraveryMod.connect(this, &GreySeerOnScreamingBell::altarOfTheHornedRat, &m_connection);
    s_globalToHitMod.connect(this, &GreySeerOnScreamingBell::wallOfUnholySound, &m_unholySoundConnection);

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

GreySeerOnScreamingBell::~GreySeerOnScreamingBell()
{
    m_connection.disconnect();
    m_unholySoundConnection.disconnect();
}

bool GreySeerOnScreamingBell::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_clawsAndFangs);
    model->addMeleeWeapon(&m_spikes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds GreySeerOnScreamingBell::applyWoundSave(const Wounds &wounds)
{
    auto totalWounds = Skaventide::applyWoundSave(wounds);

    // Protection of the Horned Rat
    Dice dice;
    Dice::RollResult resultNormal, resultMortal;

    dice.rollD6(wounds.normal, resultNormal);
    dice.rollD6(wounds.mortal, resultMortal);

    Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
    totalWounds -= negatedWounds;
    return totalWounds.clamp();
}

void GreySeerOnScreamingBell::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_spikes.setToHit(g_damageTable[damageIndex].m_spikesToHit);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void GreySeerOnScreamingBell::onRestore()
{
    Unit::onRestore();

    m_unholySoundActive = false;

    // Restore table-driven attributes
    onWounded();
}

int GreySeerOnScreamingBell::getDamageTableIndex() const
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

int GreySeerOnScreamingBell::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Unit::extraAttacks(attackingModel, weapon, target);

    // Pushed into Battle
    if ((weapon->name() == m_spikes.name()) && m_charged)
    {
        Dice dice;
        extra += dice.rollD6();
    }
    return extra;
}

int GreySeerOnScreamingBell::altarOfTheHornedRat(const Unit *unit)
{
    if (unit->hasKeyword(SKAVENTIDE) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 13.0f))
    {
        // Make unit battleshock immune
        return 13;
    }
    return 0;
}

void GreySeerOnScreamingBell::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    m_unholySoundActive = false;

    // Peal of Doom
    if (owningPlayer() == player)
    {
        Dice dice;
        int roll = dice.roll2D6();
        if (roll == 2)
        {
            // Magical Backlash
            auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);
            for (auto unit : units)
            {
                unit->applyDamage({0, dice.rollD3()});
            }
        }
        else if (roll <= 4)
        {
            // Unholy Clamour
            buffModifier(BuffableAttribute::MoveDistance, dice.rollD6(), {Phase::Hero, m_battleRound+1, owningPlayer()});
        }
        else if (roll <= 6)
        {
            // Deafening Peals
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), g_damageTable[getDamageTableIndex()].m_pealRange);
            for (auto unit : units)
            {
                if (dice.rollD6() >= 4)
                {
                    unit->applyDamage({0,1});
                }
            }
        }
        else if (roll == 7)
        {
            // Avalanche of Energy
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), g_damageTable[getDamageTableIndex()].m_pealRange);
            for (auto unit : units)
            {
                unit->buffModifier(BuffableAttribute::CastingRoll, 1, {Phase::Hero, m_battleRound+1, owningPlayer()});
            }
        }
        else if (roll <= 9)
        {
            // Apocalyptic Doom
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), g_damageTable[getDamageTableIndex()].m_pealRange);
            for (auto unit : units)
            {
                if (dice.rollD6() >= 4)
                {
                    unit->applyDamage({0,dice.rollD3()});
                }
            }
        }
        else if (roll <= 11)
        {
            // Wall of Unholy Sound
            m_unholySoundActive = true;
        }
        else // roll == 12
        {
            // A Stirring Beyond
            // Summon any unit with the VERMINLORD keyword
            static const std::string VerminlordUnitNames[6] = {
                "Lord Skreech Verminking",
                "Verminlord Corruptor",
                "Verminlord Deceiver",
                "Verminlord Warbringer",
                "Verminlord Warpseer",
                "Verminlord Corruptor",
            };

            const int which = dice.rollD6()-1;

            auto factory = UnitFactory::LookupUnit(VerminlordUnitNames[which]);
            if (factory)
            {
                if (m_roster)
                {
                    auto unit = UnitFactory::Create(VerminlordUnitNames[which], factory->m_parameters);
                    unit->setPosition(position(), m_orientation);
                    m_roster->addUnit(unit);
                }
            }
        }
    }
}

int GreySeerOnScreamingBell::wallOfUnholySound(const Weapon *weapon, const Unit *unit)
{
    // Wall of Unholy Sound
    if (m_unholySoundActive && (unit->owningPlayer() != owningPlayer()) &&
        (distanceTo(unit) <= g_damageTable[getDamageTableIndex()].m_pealRange))
    {
        return -1;
    }
    return 0;
}

} //namespace Skaven
