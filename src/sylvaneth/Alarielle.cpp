/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Alarielle.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Alarielle::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool Alarielle::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_spearKurnothRange;
    int m_greatAntlerDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 7, 10, 13, Alarielle::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 30, 5},
        {14, 25, 4},
        {10, 20, 3},
        {8,  15, 2},
        {6,  10, 1}
    };


Alarielle::Alarielle() :
    SylvanethBase("Alarielle", 16, WOUNDS, 10, 3, true),
    m_spearOfKurnoth(Weapon::Type::Missile, "Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6),
    m_talonOfDwindling(Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1),
    m_beetleGreatAntlers(Weapon::Type::Melee, "Great Antlers", 2, 5, 4, 3, -2, 5)
{
    m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN};
    m_weapons = {&m_spearOfKurnoth, &m_talonOfDwindling, &m_beetleGreatAntlers};

    m_totalUnbinds = 3;
    m_totalSpells = 3;
}

bool Alarielle::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_spearOfKurnoth);
    model->addMeleeWeapon(&m_talonOfDwindling);
    model->addMeleeWeapon(&m_beetleGreatAntlers);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

int Alarielle::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

int Alarielle::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    // Sweeping Blows
    if (weapon->name() == m_beetleGreatAntlers.name() && unit->remainingModels() >= 5)
    {
        return 1;
    }
    return Unit::toHitModifier(weapon, unit);
}

void Alarielle::onStartHero(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        Dice dice;
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            // Lifebloom - heal herself D3
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }

        // Lifebloom
        auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 30.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(SYLVANETH))
                ip->heal(dice.rollD3());
        }
    }
}

void Alarielle::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_spearOfKurnoth.setRange(g_damageTable[damageIndex].m_spearKurnothRange);
    m_beetleGreatAntlers.setDamage(g_damageTable[damageIndex].m_greatAntlerDamage);
}

int Alarielle::getDamageTableIndex() const
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

Unit *Alarielle::Create(const ParameterList &parameters)
{
    auto unit = new Alarielle();

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Alarielle::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Alarielle", factoryMethod);
    }
}

void Alarielle::onCharged()
{
    Dice dice;
    // Living Battering Ram
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    for (auto ip : units)
    {
        if (dice.rollD6() >= 4)
        {
            Wounds wounds = {0, 0};
            wounds.mortal = dice.rollD3();
            ip->applyDamage(wounds);
        }
    }
    Unit::onCharged();
}

Wounds Alarielle::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Talon of the Dwindling
    if (weapon->name() == m_talonOfDwindling.name() && hitRoll == 6)
    {
        Dice dice;
        return {weapon->damage(), dice.rollD3()};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Sylvaneth
