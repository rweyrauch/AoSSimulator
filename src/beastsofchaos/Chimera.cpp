/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Chimera.h>
#include <UnitFactory.h>
#include <iostream>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    Chimera::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    Chimera::ComputePoints,
    {
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    { BEASTS_OF_CHAOS }
};

bool Chimera::s_registered = false;

struct TableEntry
{
    int m_fieryBreath;
    int m_avianHeadRend;
    int m_leonineHeadDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, Chimera::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {RAND_D6, -3, RAND_D6},
        {RAND_D3, -2, RAND_D3},
        {RAND_D3, -2, RAND_D3},
        {1, -1, 1},
        {1, -1, 1}
    };

Chimera::Chimera() :
    BeastsOfChaosBase("Chimera", 10, WOUNDS, 6, 5, true),
    m_fieryBreath(Weapon::Type::Missile, "Fiery Breath", 14, 1, 0, 0, 0, RAND_D6),
    m_avianHead(Weapon::Type::Melee, "Avian Head", 1, 3, 3, 4, -3, RAND_D3),
    m_draconicHead(Weapon::Type::Melee, "Draconic Head", 1, 3, 4, 4, -1, 2),
    m_leonineHead(Weapon::Type::Melee, "Leonine Head", 1, 3, 4, 3, -1, RAND_D6),
    m_maulingClaws(Weapon::Type::Melee, "Mauling Claws", 2, 6, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, CHIMERA};
    m_weapons = {&m_fieryBreath, &m_avianHead, &m_draconicHead, &m_leonineHead, &m_maulingClaws };
}

bool Chimera::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);

    // NOTE: Fiery Breath attack is special, do not treat it as a weapon

    model->addMeleeWeapon(&m_avianHead);
    model->addMeleeWeapon(&m_draconicHead);
    model->addMeleeWeapon(&m_leonineHead);
    model->addMeleeWeapon(&m_maulingClaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Chimera::Create(const ParameterList &parameters)
{
    auto unit = new Chimera();

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Chimera::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chimera", factoryMethod);
    }
}

void Chimera::onWounded()
{
    const int damageIndex = getDamageTableIndex();

    m_avianHead.setRend(g_damageTable[damageIndex].m_avianHeadRend);
    m_leonineHead.setDamage(g_damageTable[damageIndex].m_leonineHeadDamage);

    Unit::onWounded();
}

int Chimera::getDamageTableIndex() const
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

int Chimera::chargeModifier() const
{
    int modifier = Unit::chargeModifier();

    // Vicious Charge
    modifier += 2;

    return modifier;
}

void Chimera::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (player == owningPlayer())
    {
        // Fiery Breath
        if (m_shootingTarget)
        {
            if (distanceTo(m_shootingTarget) <= m_fieryBreath.range())
            {
                Dice dice;
                // Auto-hit and inflict mortal wounds.
                Wounds breathDamage = {0, dice.rollSpecial(g_damageTable[getDamageTableIndex()].m_fieryBreath)};
                m_shootingTarget->applyDamage(breathDamage);
            }
        }
    }
}

} // namespace BeastsOfChaos
