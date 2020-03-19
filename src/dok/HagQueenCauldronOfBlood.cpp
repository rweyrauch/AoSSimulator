/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/HagQueenCauldronOfBlood.h>
#include <UnitFactory.h>
#include <Board.h>

namespace DaughtersOfKhaine
{
struct TableEntry
{
    int m_move;
    int m_knifeAttacks;
    int m_bloodshield;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 5, 8, 10, HagQueenOnCauldronOfBlood::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 8, 18},
        {5, 7, 14},
        {4, 6, 10},
        {3,  5, 6},
        {2,  4, 2}
    };

bool HagQueenOnCauldronOfBlood::s_registered = false;

HagQueenOnCauldronOfBlood::HagQueenOnCauldronOfBlood() :
    DaughterOfKhaine("Hag Queen on Cauldron of Blood", 6, WOUNDS, 8, 5, false),
    m_burningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
    m_knives(Weapon::Type::Melee, "Witch Aelves' Sacrificial Knives", 1, 8, 3, 4, 0, 1),
    m_blade(Weapon::Type::Melee, "Haq Queen's Blade of Khaine", 1, 4, 3, 4, -1, 1),
    m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, WITCH_AELVES, HAG_QUEEN, AVATAR_OF_KHAINE, CAULDRON_OF_BLOOD};
    m_weapons = {&m_burningBlood, &m_knives, &m_blade, &m_sword};

    s_globalBraveryMod.connect(this, &HagQueenOnCauldronOfBlood::idolOfWorship, &m_idolSlot);
    s_globalSaveMod.connect(this, &HagQueenOnCauldronOfBlood::bloodShield, &m_bloodshieldSlot);
}

HagQueenOnCauldronOfBlood::~HagQueenOnCauldronOfBlood()
{
    m_idolSlot.disconnect();
    m_bloodshieldSlot.disconnect();
}

bool HagQueenOnCauldronOfBlood::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_burningBlood);
    model->addMeleeWeapon(&m_knives);
    model->addMeleeWeapon(&m_blade);
    model->addMeleeWeapon(&m_sword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *HagQueenOnCauldronOfBlood::Create(const ParameterList &parameters)
{
    auto unit = new HagQueenOnCauldronOfBlood();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HagQueenOnCauldronOfBlood::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            HagQueenOnCauldronOfBlood::Create,
            nullptr,
            nullptr,
            HagQueenOnCauldronOfBlood::ComputePoints,
            {
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Hag Queen on Cauldron of Blood", *factoryMethod);
    }
}

void HagQueenOnCauldronOfBlood::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_knives.setAttacks(g_damageTable[damageIndex].m_knifeAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void HagQueenOnCauldronOfBlood::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int HagQueenOnCauldronOfBlood::getDamageTableIndex() const
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

void HagQueenOnCauldronOfBlood::onCharged()
{
    Unit::onCharged();

    // Bladed Impact
    auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
    if (unit && (distanceTo(unit) <= 1.0f))
    {
        if (Dice::rollD6() >= 2) unit->applyDamage({0, Dice::rollD3()});
    }
}

int HagQueenOnCauldronOfBlood::idolOfWorship(const Unit *unit)
{
    // Idol of Worship
    if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && (distanceTo(unit) <= 7.0f)) return 1;

    return 0;
}

int HagQueenOnCauldronOfBlood::bloodShield(const Unit *target, const Weapon *weapon)
{
    if (isFriendly(target) && target->hasKeyword(DAUGHTERS_OF_KHAINE) &&
        (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_bloodshield))
    {
        return 1;
    }
    return 0;
}

} //namespace DaughtersOfKhaine

