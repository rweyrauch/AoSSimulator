/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesonOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunesonOnMagmadroth::Create,
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
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, AuricRunesonOnMagmadroth::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, RAND_D6,  6},
        {10, RAND_D6,  5},
        {8,  RAND_2D6, 4},
        {7,  RAND_2D6, 3},
        {6,  RAND_3D6, 2}
    };

bool AuricRunesonOnMagmadroth::s_registered = false;

AuricRunesonOnMagmadroth::AuricRunesonOnMagmadroth() :
    Fyreslayer("Auric Runeson on Magmadroth", 12, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
    m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
    m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3),
    m_javelin(Weapon::Type::Missile, "Wyrmslayer Javelin", 12, 1, 4, 3, -1, RAND_D3),
    m_warAxe(Weapon::Type::Melee, "Ancestral War-axe", 1, 3, 3, 4, 0, RAND_D3),
    m_javelinMelee(Weapon::Type::Melee, "Wyrmslayer Javelin", 3, 1, 3, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, AURIC_RUNEFATHER};
}

bool AuricRunesonOnMagmadroth::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_throwingAxe);
    model.addMissileWeapon(&m_fyrestream);
    model.addMeleeWeapon(&m_clawsAndHorns);
    model.addMeleeWeapon(&m_blazingMaw);
    model.addMeleeWeapon(&m_javelin);
    model.addMeleeWeapon(&m_warAxe);
    model.addMeleeWeapon(&m_javelinMelee);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int AuricRunesonOnMagmadroth::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void AuricRunesonOnMagmadroth::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_throwingAxe);
    visitor(&m_fyrestream);
    visitor(&m_clawsAndHorns);
    visitor(&m_blazingMaw);
    visitor(&m_javelin);
    visitor(&m_warAxe);
    visitor(&m_javelinMelee);
}

Unit *AuricRunesonOnMagmadroth::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunesonOnMagmadroth();

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

void AuricRunesonOnMagmadroth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runeson on Magmadroth", factoryMethod);
    }
}

void AuricRunesonOnMagmadroth::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
}

int AuricRunesonOnMagmadroth::getDamageTableIndex() const
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

void AuricRunesonOnMagmadroth::onStartShooting(PlayerId player)
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

void AuricRunesonOnMagmadroth::onEndCombat(PlayerId player)
{
    Fyreslayer::onEndCombat(player);

    // Lashing Tail
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    for (auto ip : units)
    {
        Dice dice;
        if (dice.rollD6() < ip->remainingModels())
        {
            ip->applyDamage({0, dice.rollD3()});
        }
    }
}

Wounds AuricRunesonOnMagmadroth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
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

Rerolls AuricRunesonOnMagmadroth::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Vying for Glory
    if (Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, AURIC_RUNESON, 6.0f))
    {
        return RerollFailed;
    }
    return Fyreslayer::toHitRerolls(weapon, target);
}

Wounds AuricRunesonOnMagmadroth::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Wyrmslayer Javelins
    if ((weapon->name() == m_javelin.name()) && target->hasKeyword(MONSTER))
    {
        return {weapon->damage()+2, 0};
    }
    return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Fyreslayers