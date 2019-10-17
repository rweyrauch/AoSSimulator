/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SkitterstrandArachnarok.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SkitterstrandArachnarok::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool SkitterstrandArachnarok::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_legsAttacks;
    int m_fangsToHit;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SkitterstrandArachnarok::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 8, 2},
        {7, 7, 3},
        {6, 6, 3},
        {5, 5, 4},
        {4, 4, 4}
    };

SkitterstrandArachnarok::SkitterstrandArachnarok() :
    GloomspiteGitzBase("Skitterstrand Arachnarok", 8, WOUNDS, 6, 4, true),
    m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
    m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, SKITTERSTRAND, MONSTER};
}

bool SkitterstrandArachnarok::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_chitinousLegs);
    model.addMeleeWeapon(&m_monstrousFangs);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int SkitterstrandArachnarok::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void SkitterstrandArachnarok::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
    m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
}

int SkitterstrandArachnarok::getDamageTableIndex() const
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

Unit *SkitterstrandArachnarok::Create(const ParameterList &parameters)
{
    auto unit = new SkitterstrandArachnarok();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SkitterstrandArachnarok::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skitterstrand Arachnarok", factoryMethod);
    }
}

void SkitterstrandArachnarok::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_chitinousLegs);
    visitor(m_monstrousFangs);
}

Wounds SkitterstrandArachnarok::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Spider Venom
    int threshold = inLightOfTheBadMoon() ? 5 : 6;
    if ((hitRoll >= threshold) && (weapon->name() == m_monstrousFangs.name()))
    {
        Dice dice;
        return {0, dice.rollD3()};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace GloomspiteGitz
