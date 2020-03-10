/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/KairosFateweaver.h"

namespace Tzeentch
{

struct TableEntry
{
    int m_move;
    int m_staffToWound;
    int m_giftOfChange;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, KairosFateweaver::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 1, 6},
        {10, 2, RAND_D6},
        {9, 3, 3},
        {8,  4, RAND_D3},
        {7,  5, 1}
    };


static FactoryMethod factoryMethod = {
    KairosFateweaver::Create,
    TzeentchBase::ValueToString,
    TzeentchBase::EnumStringToInt,
    KairosFateweaver::ComputePoints,
    {
        {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
    },
    CHAOS,
    { TZEENTCH }
};

bool KairosFateweaver::s_registered = false;

Unit *KairosFateweaver::Create(const ParameterList &parameters)
{
    auto unit = new KairosFateweaver();

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KairosFateweaver::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kairos Fateweaver", factoryMethod);
    }
}

KairosFateweaver::KairosFateweaver() :
    TzeentchBase("Kairos Fateweaver", 12, WOUNDS, 10, 4, true),
    m_staff(Weapon::Type::Melee, "Staff of Tomorrow", 3, 3, 3, 1, -1, 2),
    m_beakAndTalons(Weapon::Type::Melee, "Beak and Claws", 1, 5, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, MONSTER, HERO, WIZARD, LORD_OF_CHANGE, KAIROS_FATEWEAVER};
    m_weapons = {&m_staff,
                 &m_beakAndTalons};

    m_totalSpells = 3;
    m_totalUnbinds = 3;
}

bool KairosFateweaver::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_beakAndTalons);
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void KairosFateweaver::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

void KairosFateweaver::onWounded()
{
    TzeentchBase::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_staff.setToWound(g_damageTable[damageIndex].m_staffToWound);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int KairosFateweaver::getDamageTableIndex() const
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

int KairosFateweaver::rollCasting() const
{
    // Mastery of Magic
    Dice dice;
    auto r0 = dice.rollD6();
    auto r1 = dice.rollD6();
    return std::max(r0, r1) * 2 + castingModifier();
}

} // Tzeentch