/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriestFurnace.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven
{
struct TableEntry
{
    int m_move;
    int m_censerDamage;
    int m_spikesToHit;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, PlaguePriestOnPlagueFurnace::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 4, 2},
        {6, 3, 3},
        {4, 2, 4},
        {4,  1, 4},
        {3,  0, 5}
    };


bool PlaguePriestOnPlagueFurnace::s_registered = false;

Unit *PlaguePriestOnPlagueFurnace::Create(const ParameterList &parameters)
{
    auto unit = new PlaguePriestOnPlagueFurnace();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlaguePriestOnPlagueFurnace::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Plague Priest on Plague Furnace", factoryMethod);
    }
}

PlaguePriestOnPlagueFurnace::PlaguePriestOnPlagueFurnace() :
    Skaventide("Plague Priest on Plague Furnace", 6, WOUNDS, 10, 4, false),
    m_censer(Weapon::Type::Melee, "Great Plague Censer", 3, 1, 2, 0, 0, 0),
    m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_blades(Weapon::Type::Melee, "Foetid Blades", 1, 6, 4, 4, 0, 1),
    m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, HERO, PRIEST,
    PLAGUE_FURNACE, PLAGUE_PRIEST};
    m_weapons = {&m_censer, &m_staff, &m_blades, &m_spikes};

    s_globalBraveryMod.connect(this, &PlaguePriestOnPlagueFurnace::altarOfTheHornedRat, &m_connection);
}

PlaguePriestOnPlagueFurnace::~PlaguePriestOnPlagueFurnace()
{
    m_connection.disconnect();
}

bool PlaguePriestOnPlagueFurnace::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_censer);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_blades);
    model->addMeleeWeapon(&m_spikes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void PlaguePriestOnPlagueFurnace::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_spikes.setToHit(g_damageTable[damageIndex].m_spikesToHit);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void PlaguePriestOnPlagueFurnace::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int PlaguePriestOnPlagueFurnace::getDamageTableIndex() const
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

Wounds PlaguePriestOnPlagueFurnace::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Great Plague Censor
    if ((weapon->name() == m_censer.name()) && (hitRoll >= 2))
    {
        return {0, Dice::rollD3() + g_damageTable[getDamageTableIndex()].m_censerDamage};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds PlaguePriestOnPlagueFurnace::onEndCombat(PlayerId player)
{
    Wounds wounds = Unit::onEndCombat(player);

    // Poisonous Fumes
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    for (auto unit : units)
    {
        if (!unit->hasKeyword(CLANS_PESTILENS))
        {
            int mortalWounds = 0;
            int roll = Dice::rollD6();
            if (roll == 6) mortalWounds = Dice::rollD3();
            else if (roll >= 4) mortalWounds = 1;

            unit->applyDamage({0, mortalWounds});
            wounds.mortal += mortalWounds;
        }
    }
    return wounds;
}

int PlaguePriestOnPlagueFurnace::altarOfTheHornedRat(const Unit *unit)
{
    // Altar of the Horned Rat
    if (unit->hasKeyword(SKAVENTIDE) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 13.0f))
    {
        // Make unit battleshock immune
        return 13;
    }

    return 0;
}

int PlaguePriestOnPlagueFurnace::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Unit::extraAttacks(attackingModel, weapon, target);

    // Pushed into Battle
    if ((weapon->name() == m_spikes.name()) && m_charged)
    {
        extra += Dice::rollD6();
    }
    return extra;
}

Wounds PlaguePriestOnPlagueFurnace::applyWoundSave(const Wounds &wounds)
{
    auto totalWounds = Skaventide::applyWoundSave(wounds);

    // Protection of the Horned Rat
    Dice::RollResult resultNormal, resultMortal;

    Dice::rollD6(wounds.normal, resultNormal);
    Dice::rollD6(wounds.mortal, resultMortal);

    Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
    totalWounds -= negatedWounds;
    return totalWounds.clamp();
}

void PlaguePriestOnPlagueFurnace::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if (player == owningPlayer())
    {
        // Noxious Prayers
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), CLANS_PESTILENS, 13.0f);

        if (!unit)
        {
            // No other units - pray for this unit.
            unit = this;
        }

        Dice::RollResult result;

        auto prayerRoll = Dice::rollD6();
        if (prayerRoll == 1)
        {
            // Failed - take one mortal wound.
            applyDamage({0, 1});
        }
        else if (prayerRoll >= 3)
        {
            // Success - select prayer (randomly)
            if (Dice::rollD6() >= 4)
            {
                // Filth-filth!
                unit->buffReroll(BuffableAttribute::ToWoundMelee, RerollFailed, {Phase::Hero, m_battleRound+1, owningPlayer()});
                unit->buffReroll(BuffableAttribute::ToWoundMissile, RerollFailed, {Phase::Hero, m_battleRound+1, owningPlayer()});
            }
            else
            {
                // Rabid-rabid!
                unit->buffModifier(BuffableAttribute::AttacksMelee, 1, {Phase::Hero, m_battleRound+1, owningPlayer()});
            }
        }
    }

}

} //namespace Skaven
