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

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Alarielle::Create,
    nullptr,
    {
    }
};

bool Alarielle::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_spearKurnothRange;
    int m_greatAntlerDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 4, 7, 10, 13, Alarielle::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { 16, 30, 5 },
    { 14, 25, 4 },
    { 10, 20, 3 },
    { 8, 15, 2 },
    { 6, 10, 1 }
};

Weapon Alarielle::s_spearOfKurnoth(Weapon::Type::Missile, "Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6);
Weapon Alarielle::s_talonOfDwindling(Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1);
Weapon Alarielle::s_beetleGreatAntlers(Weapon::Type::Melee, "Wardroth Beetle's Great Antlers", 2, 5, 4, 3, -2, 5);

Alarielle::Alarielle() :
    Unit("Alarielle", 16, WOUNDS, 10, 3, true)
{
    m_keywords = { ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN };
}

bool Alarielle::configure()
{
    Model model(BASESIZE, WOUNDS);

    m_pSpearOfKurnoth = new Weapon(s_spearOfKurnoth);
    m_pBeetleGreatAntlers = new Weapon(s_beetleGreatAntlers);

    model.addMissileWeapon(m_pSpearOfKurnoth);
    model.addMeleeWeapon(&s_talonOfDwindling);
    model.addMeleeWeapon(m_pBeetleGreatAntlers);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_spearOfKurnoth.name() << ": " << s_spearOfKurnoth.strength() << std::endl;
        std::cout << "\t" << s_talonOfDwindling.name() << ": " << s_talonOfDwindling.strength() << std::endl;
        std::cout << "\t" << s_beetleGreatAntlers.name() << ": " << s_beetleGreatAntlers.strength() << std::endl;
    }

    return true;
}

int Alarielle::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

int Alarielle::toHitModifier(const Weapon* weapon, const Unit* unit) const
{
    // Sweeping Blows
    if (weapon->name() == s_beetleGreatAntlers.name() && unit->remainingModels() >= 5)
    {
        return 1;
    }
    return Unit::toHitModifier(weapon, unit);
}

void Alarielle::hero(PlayerId player)
{
    if (remainingWounds() < WOUNDS && remainingWounds() > 0)
    {
        Dice dice;
        // Lifebloom - heal herself D3
        int woundsHealed = dice.rollD3();
        // Soul Amphorae (healing part).  TODO: only do this if not summoning
        woundsHealed += dice.rollD3();
        for (auto& m : m_models)
        {
            m.woundsRemaining() += woundsHealed;
            if (m.woundsRemaining() > WOUNDS)
                m.woundsRemaining() = WOUNDS;
        }
    }
}

void Alarielle::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pSpearOfKurnoth->setRange(g_damageTable[damageIndex].m_spearKurnothRange);
    m_pBeetleGreatAntlers->setDamage(g_damageTable[damageIndex].m_greatAntlerDamage);
}

int Alarielle::getDamageTableIndex() const
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

Unit *Alarielle::Create(const ParameterList &parameters)
{
    auto unit = new Alarielle();

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

} // namespace Sylvaneth
