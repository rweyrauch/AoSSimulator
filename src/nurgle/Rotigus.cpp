/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Rotigus.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    Rotigus::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    Rotigus::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

struct TableEntry
{
    int m_rodToHit;
    int m_mawToWound;
    int m_delugeOfNurgle;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, Rotigus::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {2, 2, 4},
        {3, 2, 5},
        {3, 3, 5},
        {4, 3, 6},
        {4, 4, 6}
    };

bool Rotigus::s_registered = false;

Rotigus::Rotigus() :
    NurgleBase("Rotigus", 5, WOUNDS, 10, 4, false),
    m_gnarlrod(Weapon::Type::Melee, "Gnarlrod", 3, 5, 2, 3, -1, 2),
    m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 3, 2, -2, 2),
    m_nurglings(Weapon::Type::Melee, "Host of Nurglings", 1, 3, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, NURGLE, MONSTER, HERO, WIZARD, GREAT_UNCLEAN_ONE, ROTIGUS};
    m_weapons = {&m_gnarlrod, &m_fangedMaw, &m_nurglings};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool Rotigus::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_gnarlrod);
    model->addMeleeWeapon(&m_fangedMaw);
    model->addMeleeWeapon(&m_nurglings);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Rotigus::Create(const ParameterList &parameters)
{
    auto unit = new Rotigus();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Rotigus::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Rotigus", factoryMethod);
    }
}

void Rotigus::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_gnarlrod.setToHit(g_damageTable[damageIndex].m_rodToHit);
    m_fangedMaw.setToWound(g_damageTable[damageIndex].m_mawToWound);
}

int Rotigus::getDamageTableIndex() const
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

void Rotigus::onCharged()
{
    // Mountain of Loathsome Flesh
    if (m_charged)
    {
        Dice dice;
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto ip : units)
        {
            int roll = dice.rollD6();
            if (roll >= 4)
            {
                ip->applyDamage({dice.rollD3(), 0});
            }
        }
    }
    Unit::onCharged();
}

} // namespace Nurgle

