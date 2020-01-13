/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <seraphon/DreadSaurian.h>
#include <UnitFactory.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    DreadSaurian::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    { SERAPHON }
};

bool DreadSaurian::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_jawsDamage;
    int m_clawsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 7, 10, 13, DreadSaurian::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 6, 10},
        {9,  5,  8},
        {8,  4,  6},
        {7,  3,  4},
        {6,  2,  2}
    };

DreadSaurian::DreadSaurian() :
    Unit("Dread Saurian", 10, WOUNDS, 10, 3, false),
    m_gargantuanJaws(Weapon::Type::Melee, "Gargantuan Jaws", 2, 3, 3, 3, -2, 6),
    m_rakingClaws(Weapon::Type::Melee, "Raking Claws", 2, 10, 3, 3, -1, 1),
    m_armouredTail(Weapon::Type::Melee, "Armoured Tail", 1, 1, 4, 3, -1, RAND_D6)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, MONSTER, DREAD_SAURIAN};
    m_weapons = {&m_gargantuanJaws, &m_rakingClaws, &m_armouredTail};
}

bool DreadSaurian::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_gargantuanJaws);
    model->addMeleeWeapon(&m_rakingClaws);
    model->addMeleeWeapon(&m_armouredTail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int DreadSaurian::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *DreadSaurian::Create(const ParameterList &parameters)
{
    auto unit = new DreadSaurian();

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
        s_registered = UnitFactory::Register("Dread Saurian", factoryMethod);
    }
}

void DreadSaurian::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_rakingClaws.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
    m_gargantuanJaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
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

Rerolls DreadSaurian::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Devourer of Beasts
    if (target->hasKeyword(MONSTER))
        return RerollOnes;

    return Unit::toHitRerolls(weapon, target);
}

Rerolls DreadSaurian::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Devourer of Beasts
    if (target->hasKeyword(MONSTER))
        return RerollOnes;

    return Unit::toWoundRerolls(weapon, target);
}

void DreadSaurian::onStartHero(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            // Arcane Glyph
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }
    }
}

} //namespace Seraphon