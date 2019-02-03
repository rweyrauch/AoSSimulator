/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ColossalSquig.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    ColossalSquig::Create,
    nullptr,
    nullptr,
    {
    }
};

bool ColossalSquig::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_jawsToHit;
    int m_tramplingAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 3, 7, 10, 13, ColossalSquig::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { RAND_4D6, 2, 10 },
    { RAND_3D6, 3, 8 },
    { RAND_2D6, 4, 6 },
    { RAND_2D6, 5, 4 },
    { RAND_D6, 6, 2 }
};

Weapon ColossalSquig::s_puffSpores(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3);
Weapon ColossalSquig::s_enormousJaws(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3);
Weapon ColossalSquig::s_tramplingFeet(Weapon::Type::Melee, "Trampling Feet", 1, 10, 5, 3, -1, 1);

ColossalSquig::ColossalSquig() :
    Unit("Colossal Squig", RAND_4D6, WOUNDS, 10, 5, false)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, COLOSSAL_SQUIG };
}

int ColossalSquig::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ColossalSquig::charge(PlayerId player)
{
    // TODO: should charge
    bool charging = false;
    if (charging)
    {
        // Crazed Charge - 6+ mortal wounds for all units within 1" of this model at the end of the charge.
    }
}

bool ColossalSquig::configure()
{
    Model model(BASESIZE, WOUNDS);

    m_pEnormousJaws = new Weapon(s_enormousJaws);
    m_pTramplingFeet = new Weapon(s_tramplingFeet);

    model.addMeleeWeapon(m_pEnormousJaws);
    model.addMeleeWeapon(m_pTramplingFeet);
    model.addMeleeWeapon(&s_puffSpores);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ColossalSquig::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pEnormousJaws->setToHit(g_damageTable[damageIndex].m_jawsToHit);
    m_pTramplingFeet->setAttacks(g_damageTable[damageIndex].m_tramplingAttacks);
}

void ColossalSquig::onSlain()
{
    // Fungoid Squig Explosion
    // TODO: Roll D6 for each enemy unit w/in 3", on 2+ unit suffers D3 mortal wounds.

    // TODO: Setup 5 cave squigs w/in 9" of this model and outside of 3" from enemy models.
}

int ColossalSquig::generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits, const WoundingHits& wounds)
{
    // Swallowed Whole
    if ((hits.rolls.numUnmodified6s() > 0) && (weapon->name() == s_enormousJaws.name()))
    {
        Dice dice;

        int mortalWounds = 0;
        for (auto i = 0; i < hits.rolls.numUnmodified6s(); i++)
        {
            mortalWounds += dice.rollD3();
        }
        return mortalWounds;
    }
    return 0;
}

int ColossalSquig::getDamageTableIndex() const
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


Unit *ColossalSquig::Create(const ParameterList &parameters)
{
    auto unit = new ColossalSquig();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ColossalSquig::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Colossal Squig", factoryMethod);
    }
}

void ColossalSquig::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_enormousJaws);
    visitor(&s_tramplingFeet);
    visitor(&s_puffSpores);
}

} // namespace GloomspiteGitz
