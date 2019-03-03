/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/GhoulKingOnTerrorgheist.h>
#include <UnitFactory.h>
#include <Board.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantGhoulKingOnTerrorgheist::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool AbhorrantGhoulKingOnTerrorgheist::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_deathShriek;
    int m_clawAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 7, 10, 13, AbhorrantGhoulKingOnTerrorgheist::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 6, 4},
        {12, 5, 4},
        {10, 4, 3},
        {8,  3, 3},
        {6,  2, 2}
    };

AbhorrantGhoulKingOnTerrorgheist::AbhorrantGhoulKingOnTerrorgheist() :
    FleshEaterCourts("Abhorrant Ghoul King on Terrorgheist", 14, WOUNDS, 10, 4, true),
    m_deathShriek(Weapon::Type::Missile, "Death Shriek", 10, 1, 0, 0, 0, 0),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 5, 3, 3, -1, 1),
    m_skeletalClaws(Weapon::Type::Melee, "Skeletal Claws", 2, 4, 4, 3, -1, RAND_D3),
    m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 3, 3, 4, 3, -2, RAND_D6)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, MONSTER, HERO,
        WIZARD, ABHORRANT_GHOUL_KING};
}

bool AbhorrantGhoulKingOnTerrorgheist::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_deathShriek);
    model.addMeleeWeapon(&m_goryTalonsAndFangs);
    model.addMeleeWeapon(&m_skeletalClaws);
    model.addMeleeWeapon(&m_fangedMaw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AbhorrantGhoulKingOnTerrorgheist::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_deathShriek);
    visitor(&m_goryTalonsAndFangs);
    visitor(&m_skeletalClaws);
    visitor(&m_fangedMaw);
}

Unit *AbhorrantGhoulKingOnTerrorgheist::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantGhoulKingOnTerrorgheist();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AbhorrantGhoulKingOnTerrorgheist::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Abhorrant Ghoul King on Terrorgheist", factoryMethod);
    }
}

Wounds AbhorrantGhoulKingOnTerrorgheist::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Gaping Maw
    if (weapon->name() == m_fangedMaw.name() && hitRoll == 6)
    {
        return {0, 6};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void AbhorrantGhoulKingOnTerrorgheist::hero(PlayerId player)
{
    Unit::hero(player);

    // Royal Blood
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m.woundsRemaining() += woundsHealed;
                if (m.woundsRemaining() > WOUNDS)
                {
                    m.woundsRemaining() = WOUNDS;
                }
            }
        }
    }
}

int AbhorrantGhoulKingOnTerrorgheist::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

void AbhorrantGhoulKingOnTerrorgheist::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
}

int AbhorrantGhoulKingOnTerrorgheist::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void AbhorrantGhoulKingOnTerrorgheist::onSlain()
{
    Unit::onSlain();

    Dice dice;
    // Infested
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 3.0f);
    for (auto ip : units)
    {
        Wounds wounds = {0, dice.rollD3()};
        ip->applyDamage(wounds);
    }
}

} // namespace FleshEasterCourt
