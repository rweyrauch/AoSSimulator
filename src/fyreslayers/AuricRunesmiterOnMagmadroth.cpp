/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiterOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunesmiterOnMagmadroth::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

struct TableEntry
{
    int m_move;
    int m_roaringFyrestream;
    int m_clawsHornsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, AuricRunesmiterOnMagmadroth::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, RAND_D6,  6},
        {10, RAND_D6,  5},
        {8,  RAND_2D6, 4},
        {7,  RAND_2D6, 3},
        {6,  RAND_3D6, 2}
    };

bool AuricRunesmiterOnMagmadroth::s_registered = false;

AuricRunesmiterOnMagmadroth::AuricRunesmiterOnMagmadroth() :
    Fyreslayer("Auric Runesmiter on Magmadroth", 12, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
    m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
    m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3),
    m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 3, 3, 0, 2),
    m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, PRIEST, AURIC_RUNESMITER};
    m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_latchAxe, &m_runicIron};
}

bool AuricRunesmiterOnMagmadroth::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_throwingAxe);
    model->addMissileWeapon(&m_fyrestream);
    model->addMeleeWeapon(&m_clawsAndHorns);
    model->addMeleeWeapon(&m_blazingMaw);
    model->addMeleeWeapon(&m_latchAxe);
    model->addMeleeWeapon(&m_runicIron);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int AuricRunesmiterOnMagmadroth::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *AuricRunesmiterOnMagmadroth::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunesmiterOnMagmadroth();

    auto lodge = (Lodge) GetEnumParam("Lodge", parameters, Fyreslayer::None);
    unit->setLodge(lodge);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AuricRunesmiterOnMagmadroth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runesmiter on Magmadroth", factoryMethod);
    }
}

void AuricRunesmiterOnMagmadroth::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
}

int AuricRunesmiterOnMagmadroth::getDamageTableIndex() const
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

void AuricRunesmiterOnMagmadroth::onStartShooting(PlayerId player)
{
    Fyreslayer::onStartShooting(player);
    if (player == m_owningPlayer)
    {
        // Roaring Fyrestream
        if (m_shootingTarget)
        {
            float dist = distanceTo(m_shootingTarget);
            if (dist <= m_fyrestream.range())
            {
                Dice dice;
                int rs = dice.rollSpecial(g_damageTable[getDamageTableIndex()].m_roaringFyrestream);
                if (rs <= m_shootingTarget->remainingModels())
                {
                    if (dist < 6.0f)
                    {
                        m_shootingTarget->applyDamage({0, dice.rollD6()});
                    }
                    else
                    {
                        m_shootingTarget->applyDamage({0, dice.rollD3()});
                    }
                }
            }
        }
    }
}


Wounds AuricRunesmiterOnMagmadroth::onEndCombat(PlayerId player)
{
    auto wounds = Fyreslayer::onEndCombat(player);

    // Lashing Tail
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    for (auto ip : units)
    {
        Dice dice;
        if (dice.rollD6() < ip->remainingModels())
        {
            Wounds tailWounds = {0,dice.rollD3()};
            ip->applyDamage(tailWounds);
            wounds += tailWounds;
        }
    }
    return wounds;
}

Wounds AuricRunesmiterOnMagmadroth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    if (!weapon->isMissile())
    {
        // Volcanic Blood
        Dice dice;
        if (dice.rollD6() >= 4)
        {
            return {0, 1};
        }
    }
    return Fyreslayer::computeReturnedDamage(weapon, saveRoll);
}

} // namespace Fyreslayers