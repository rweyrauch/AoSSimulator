/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/SpiritOfDurthu.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Sylvaneth
{

bool SpiritOfDurthu::s_registered = false;

struct TableEntry
{
    int m_verdantBlastAttacks;
    int m_guardianSwordDamage;
    int m_talonsToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SpiritOfDurthu::WOUNDS};
const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 6,       2},
        {5, RAND_D6, 2},
        {4, RAND_D6, 3},
        {3, RAND_D6, 3},
        {2, RAND_D3, 4}
    };

SpiritOfDurthu::SpiritOfDurthu() :
    SylvanethBase("Spirit of Durthu", 5, WOUNDS, 9, 3, false),
    m_verdantBlast(Weapon::Type::Missile, "Verdant Blast", 15, 6, 4, 3, -1, RAND_D3),
    m_guardianSword(Weapon::Type::Melee, "Guardian Sword", 3, 3, 3, 3, -2, 6),
    m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1)
{
    m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, MONSTER, HERO, SPIRIT_OF_DURTHU};
    m_weapons = {&m_verdantBlast, &m_guardianSword, &m_massiveImpalingTalons};

    s_globalBraveryMod.connect(this, &SpiritOfDurthu::championOfTheEverqueensWill, &m_connection);
}

SpiritOfDurthu::~SpiritOfDurthu()
{
    m_connection.disconnect();
}

bool SpiritOfDurthu::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_verdantBlast);
    model->addMeleeWeapon(&m_guardianSword);
    model->addMeleeWeapon(&m_massiveImpalingTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void SpiritOfDurthu::onStartHero(PlayerId id)
{
}

void SpiritOfDurthu::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_verdantBlast.setAttacks(g_damageTable[damageIndex].m_verdantBlastAttacks);
    m_guardianSword.setDamage(g_damageTable[damageIndex].m_guardianSwordDamage);
    m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
}

int SpiritOfDurthu::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();

    // Our Roots Run Deep
    if (hasKeyword(OAKENBROW)) woundsInflicted += 2;

    for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

Unit *SpiritOfDurthu::Create(const ParameterList &parameters)
{
    auto unit = new SpiritOfDurthu();

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

void SpiritOfDurthu::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            SpiritOfDurthu::Create,
            SylvanethBase::ValueToString,
            SylvanethBase::EnumStringToInt,
            SpiritOfDurthu::ComputePoints,
            {
                {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
            },
            ORDER,
            { SYLVANETH }
        };
        s_registered = UnitFactory::Register("Spirit of Durthu", factoryMethod);
    }
}

Wounds SpiritOfDurthu::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impale
    if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name()))
    {
        return {0, Dice::rollD6()};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void SpiritOfDurthu::onStartCombat(PlayerId id)
{
    Unit::onStartCombat(id);

    // Groundshaking Stomp
    if (m_meleeTarget && distanceTo(m_meleeTarget) <= 3.0f)
    {
        if (Dice::rollD6() >= 4)
        {
            // TODO: Make m_meleeTarget fight last
        }
    }
}

int SpiritOfDurthu::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

    // Wrathful Guardian
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 8.0f);
    if (unit != nullptr)
    {
        attacks += 2;
    }

    return attacks;
}

int SpiritOfDurthu::championOfTheEverqueensWill(const Unit *target)
{
    // Champion of the Everqueen's Will
    if (target->hasKeyword(SYLVANETH) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 12.0f))
    {
        return 1;
    }

    return 0;
}

} // namespace Sylvaneth
