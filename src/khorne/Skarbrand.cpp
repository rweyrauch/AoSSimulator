/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skarbrand.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skarbrand::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

struct TableEntry
{
    int m_roarOfTotalRage;
    int m_slaughterAttacks;
    int m_totalCarnage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, Skarbrand::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {1, 5, 5},
        {2, 6, 4},
        {3, 7, 3},
        {4, 8, 2},
        {5, 9, 1}
    };

bool Skarbrand::s_registered = false;

Skarbrand::Skarbrand() :
    KhorneBase("Skarbrand", 8, WOUNDS, 10, 4, true),
    m_slaughter(Weapon::Type::Melee, "Slaughter", 2, 5, 4, 3, -2, 3),
    m_carnage(Weapon::Type::Melee, "Carnage", 2, 1, 4, 0, 0, 0)
{
    m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, SKARBRAND};
}

bool Skarbrand::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_slaughter);
    // Do not add Carnage or Roar of Total Rage, their attacks are special.

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Skarbrand::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_slaughter);
    visitor(&m_carnage);
}

Unit *Skarbrand::Create(const ParameterList &parameters)
{
    auto unit = new Skarbrand();

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skarbrand::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skarbrand", factoryMethod);
    }
}

void Skarbrand::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_slaughter.setAttacks(g_damageTable[damageIndex].m_slaughterAttacks);

    KhorneBase::onWounded();
}

int Skarbrand::getDamageTableIndex() const
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

Rerolls Skarbrand::chargeRerolls() const
{
    // Inescapable Wrath
    return RerollFailed;
}

int Skarbrand::generateMortalWounds(const Unit *unit)
{
    // Total Carnage
    if (distanceTo(unit) <= m_carnage.range())
    {
        int index = getDamageTableIndex();
        if (!m_attackedInPreviousRound)
            index = NUM_TABLE_ENTRIES - 1;

        m_attackedInPreviousRound = true;

        Dice dice;
        int roll = dice.rollD6();
        if (roll >= g_damageTable[index].m_totalCarnage)
        {
            int mortals = 8;
            if (roll == 6)
                mortals = 16;

            SimLog(Verbosity::Narrative, "Skarbrand Total Carnage inflicts %d mortal wounds on to %s",
                mortals, unit->name().c_str());

            return mortals;
        }
    }

    return KhorneBase::generateMortalWounds(unit);
}

void Skarbrand::onStartShooting(PlayerId player)
{
    // Roar of Total Rage
    if (m_shootingTarget)
    {
        if (distanceTo(m_shootingTarget) <= 8.0f)
        {
            Dice dice;
            Dice::RollResult results;
            dice.rollD6(g_damageTable[getDamageTableIndex()].m_roarOfTotalRage, results);
            if (results.rollsGE(4))
            {
                Wounds wounds = {0, results.rollsGE(4)};
                m_shootingTarget->applyDamage(wounds);

                SimLog(Verbosity::Narrative, "Skarbrand Roar of Total Rage inflicts %d mortal wounds on to %s",
                    wounds.mortal, m_shootingTarget->name().c_str());
            }
            m_attackedInPreviousRound = true;
        }
    }
    KhorneBase::onStartShooting(player);
}

} // namespace Khorne