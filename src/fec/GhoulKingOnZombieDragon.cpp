/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/GhoulKingOnZombieDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantGhoulKingOnZombieDragon::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool AbhorrantGhoulKingOnZombieDragon::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_breathToWound;
    int m_clawsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, AbhorrantGhoulKingOnZombieDragon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 2, 7},
        {12, 3, 6},
        {10, 4, 5},
        {8,  5, 4},
        {6,  6, 3}
    };

AbhorrantGhoulKingOnZombieDragon::AbhorrantGhoulKingOnZombieDragon() :
    FleshEaterCourts("Abhorrant Ghoul King on Zombie Dragon", 14, WOUNDS, 10, 4, true),
    m_pestilentialBreath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 5, 3, 3, -1, 1),
    m_snappingMaw(Weapon::Type::Melee, "Snapping Maw", 3, 3, 4, 3, -2, RAND_D6),
    m_swordlikeClaws(Weapon::Type::Melee, "Sword-like Claws", 2, 7, 4, 3, -1, 2)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, MONSTER, HERO,
        WIZARD, ABHORRANT_GHOUL_KING};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool AbhorrantGhoulKingOnZombieDragon::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_pestilentialBreath);
    model.addMeleeWeapon(&m_goryTalonsAndFangs);
    model.addMeleeWeapon(&m_snappingMaw);
    model.addMeleeWeapon(&m_swordlikeClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void AbhorrantGhoulKingOnZombieDragon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_pestilentialBreath);
    visitor(&m_goryTalonsAndFangs);
    visitor(&m_snappingMaw);
    visitor(&m_swordlikeClaws);
}

Unit *AbhorrantGhoulKingOnZombieDragon::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantGhoulKingOnZombieDragon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AbhorrantGhoulKingOnZombieDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Abhorrant Ghoul King on Zombie Dragon", factoryMethod);
    }
}

Wounds AbhorrantGhoulKingOnZombieDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void AbhorrantGhoulKingOnZombieDragon::onStartHero(PlayerId player)
{
    // Royal Blood
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m.woundsRemaining() += woundsHealed;
                if (m.woundsRemaining() > WOUNDS)
                {
                    m.woundsRemaining() = WOUNDS;
                }
            }
        }
    }
}

int AbhorrantGhoulKingOnZombieDragon::getDamageTableIndex() const
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

void AbhorrantGhoulKingOnZombieDragon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pestilentialBreath.setToWound(g_damageTable[damageIndex].m_breathToWound);
    m_swordlikeClaws.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
}

int AbhorrantGhoulKingOnZombieDragon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

} // namespace FleshEasterCourt
