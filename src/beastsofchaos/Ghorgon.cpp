/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Ghorgon.h>
#include <UnitFactory.h>
#include <iostream>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    Ghorgon::Create,
    nullptr,
    nullptr,
    {
    }
};

bool Ghorgon::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_bladesAttacks;
    int m_greatMawToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 3, 6, 9, 12, Ghorgon::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        { 8, 5, 2 },
        { 6, 4, 3 },
        { 5, 3, 3 },
        { 4, 2, 4 },
        { 3, 1, 4 }
    };

Weapon Ghorgon::s_butcheringBlades(Weapon::Type::Melee, "Butchering Blades", 2, 5, 3, 3, -1, 3);
Weapon Ghorgon::s_hugeSlaveringMaw(Weapon::Type::Melee, "Huge Slavering Maw", 1, 1, 4, 2, -1, RAND_D6);

BeastsOfChaos::Ghorgon::Ghorgon() :
    Unit("Ghorgon", 8, WOUNDS, 7, 5, false)
{
    m_keywords = { CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, MONSTER, GHORGON };
}

bool BeastsOfChaos::Ghorgon::configure()
{
    Model model(BASESIZE, WOUNDS);

    m_pButcheringBlades = new Weapon(s_butcheringBlades);
    m_pHugeSlaveringMaw = new Weapon(s_hugeSlaveringMaw);

    model.addMeleeWeapon(m_pButcheringBlades);
    model.addMeleeWeapon(m_pHugeSlaveringMaw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_butcheringBlades.name() << ": " << s_butcheringBlades.strength() << std::endl;
        std::cout << "\t" << s_hugeSlaveringMaw.name() << ": " << s_hugeSlaveringMaw.strength() << std::endl;
    }

    return true;
}

int BeastsOfChaos::Ghorgon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *BeastsOfChaos::Ghorgon::Create(const ParameterList &parameters)
{
    auto unit = new Ghorgon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BeastsOfChaos::Ghorgon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ghorgon", factoryMethod);
    }
}

void BeastsOfChaos::Ghorgon::onWounded()
{
    const int damageIndex = getDamageTableIndex();

    m_pButcheringBlades->setAttacks(g_damageTable[damageIndex].m_bladesAttacks);
    m_pHugeSlaveringMaw->setDamage(g_damageTable[damageIndex].m_greatMawToWound);

    Unit::onWounded();
}

int BeastsOfChaos::Ghorgon::getDamageTableIndex() const
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

int Ghorgon::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    // Ravenous Bloodgreed
    Dice dice;
    int mortalWounds = 0;
    for (auto i = 0; i < hits.rolls.numUnmodified6s(); i++)
    {
        mortalWounds += dice.rollD3();
    }
    return mortalWounds;
 }

void Ghorgon::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    // Swallowed Whole
    if (m_meleeTarget)
    {
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= m_meleeTarget->wounds())
        {
            m_meleeTarget->slay(1);
        }
    }
}

void Ghorgon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_butcheringBlades);
    visitor(&s_hugeSlaveringMaw);
}

} // namespace BeastsOfChaos
