/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosSorcererOnManticore.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosSorcererOnManticore::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

struct TableEntry
{
    int m_move;
    int m_fangsToWound;
    int m_tailAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ChaosSorcererOnManticore::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 1, 5},
        {10, 2,4},
        {8, 3,3},
        {6, 4,2},
        {6, 5,1}
    };

bool ChaosSorcererOnManticore::s_registered = false;

Unit *ChaosSorcererOnManticore::Create(const ParameterList &parameters)
{
    auto unit = new ChaosSorcererOnManticore();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosSorcererOnManticore::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Sorcerer On Manticore", factoryMethod);
    }
}

ChaosSorcererOnManticore::ChaosSorcererOnManticore() :
    Unit("Chaos Sorcerer On Manticore", 12, WOUNDS, 8, 4, true),
    m_staff(Weapon::Type::Melee, "Sorcerous Reaping Staff", 2, 3, 3, 3, -1, RAND_D3),
    m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
    m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1)
{
    m_keywords = { CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO, WIZARD, CHAOS_SORCERER_LORD };
}

bool ChaosSorcererOnManticore::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_staff);
    model.addMeleeWeapon(&m_fangsAndClaws);
    model.addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosSorcererOnManticore::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_staff);
    visitor(m_fangsAndClaws);
    visitor(m_tail);
}

int ChaosSorcererOnManticore::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ChaosSorcererOnManticore::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
    m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
}

int ChaosSorcererOnManticore::getDamageTableIndex() const
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

Rerolls ChaosSorcererOnManticore::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Territorial Predator
    if ((weapon->name() == m_fangsAndClaws.name()) && (target->hasKeyword(MONSTER)))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

} //namespace SlavesToDarkness
