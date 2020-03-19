/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/BloodwrackShrine.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace DaughtersOfKhaine
{
struct TableEntry
{
    int m_move;
    int m_staveAttacks;
    int m_auraOfAgony;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 5, 8, 10, BloodwrackShrine::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 6, 2},
        {5, 5, 2},
        {4, 4, 3},
        {3,  3, 4},
        {2,  2, 5}
    };

bool BloodwrackShrine::s_registered = false;

BloodwrackShrine::BloodwrackShrine() :
    DaughterOfKhaine("Bloodwrack Shrine", 6, WOUNDS, 8, 5, false),
    m_bloodwrackStare(Weapon::Type::Missile, "Bloodwrack Stare", 10, 1, 0, 0, -7, 0),
    m_whisperclaw(Weapon::Type::Melee, "Whisperclaw", 1, 4, 4, 3, 0, 1),
    m_tailOfSerpents(Weapon::Type::Melee, "Tail of Serpents", 2, RAND_D6, 4, 4, 0, 1),
    m_bloodwrackSpear(Weapon::Type::Melee, "Bloodwrack Spear", 2, 2, 3, 3, -1, RAND_D3),
    m_goadstaves(Weapon::Type::Melee, "Shrinekeeper's Goadstaves", 2, 6, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, MELUSAI, HERO, WIZARD, BLOODWRACK_MEDUSA, BLOODWRACK_SHRINE};
    m_weapons = {&m_bloodwrackStare, &m_whisperclaw, &m_tailOfSerpents, &m_bloodwrackSpear, &m_goadstaves};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool BloodwrackShrine::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_bloodwrackStare);
    model->addMeleeWeapon(&m_whisperclaw);
    model->addMeleeWeapon(&m_tailOfSerpents);
    model->addMeleeWeapon(&m_bloodwrackSpear);
    model->addMeleeWeapon(&m_goadstaves);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *BloodwrackShrine::Create(const ParameterList &parameters)
{
    auto unit = new BloodwrackShrine();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodwrackShrine::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            BloodwrackShrine::Create,
            nullptr,
            nullptr,
            BloodwrackShrine::ComputePoints,
            {
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Bloodwrack Shrine", *factoryMethod);
    }
}

void BloodwrackShrine::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_goadstaves.setAttacks(g_damageTable[damageIndex].m_staveAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void BloodwrackShrine::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int BloodwrackShrine::getDamageTableIndex() const
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

void BloodwrackShrine::onCharged()
{
    Unit::onCharged();

    // Bladed Impact
    auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
    if (unit && (distanceTo(unit) <= 1.0f))
    {
        if (Dice::rollD6() >= 2) unit->applyDamage({0, Dice::rollD3()});
    }
}

void BloodwrackShrine::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    // Aura of Agony
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 7.0f);
    const int auraToHit = g_damageTable[getDamageTableIndex()].m_auraOfAgony;
    for (auto unit : units)
    {
        if (Dice::rollD6() >= auraToHit)
        {
            unit->applyDamage({0, Dice::rollD3()});
        }
    }
}

Wounds BloodwrackShrine::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if (weapon->name() == m_bloodwrackStare.name())
    {
        Dice::RollResult result;
        Dice::rollD6(target->remainingModels(), result);
        return {0, result.rollsGE(5)};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace DaughtersOfKhaine

