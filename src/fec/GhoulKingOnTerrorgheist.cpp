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
#include <spells/MysticShield.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantGhoulKingOnTerrorgheist::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    AbhorrantGhoulKingOnTerrorgheist::ComputePoints,
    {
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    { FLESH_EATER_COURTS }
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
    m_weapons = {&m_deathShriek, &m_goryTalonsAndFangs, &m_skeletalClaws, &m_fangedMaw};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool AbhorrantGhoulKingOnTerrorgheist::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_deathShriek);
    model->addMeleeWeapon(&m_goryTalonsAndFangs);
    model->addMeleeWeapon(&m_skeletalClaws);
    model->addMeleeWeapon(&m_fangedMaw);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AbhorrantGhoulKingOnTerrorgheist::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantGhoulKingOnTerrorgheist();

    auto court = (GrandCourt)GetEnumParam("Grand Court", parameters, NoCourt);
    auto delusion = (Delusion)GetEnumParam("Delusion", parameters, None);
    // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
    unit->setGrandCourt(court);
    unit->setCourtsOfDelusion(delusion);

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
    return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void AbhorrantGhoulKingOnTerrorgheist::onStartHero(PlayerId player)
{
    // Royal Blood
    if (player == owningPlayer())
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            int woundsHealed = Dice::rollD3();
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }
    }
}

int AbhorrantGhoulKingOnTerrorgheist::getDamageTableIndex() const
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

void AbhorrantGhoulKingOnTerrorgheist::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void AbhorrantGhoulKingOnTerrorgheist::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

void AbhorrantGhoulKingOnTerrorgheist::onSlain()
{
    FleshEaterCourts::onSlain();

    // Infested
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0f);
    for (auto ip : units)
    {
        Wounds wounds = {0, Dice::rollD3()};
        ip->applyDamage(wounds);
    }
}

} // namespace FleshEasterCourt
