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
    nullptr,
    nullptr,
    {
    }
};

struct TableEntry
{
    Skarbrand::Rage m_rage;
    int m_slaughterAttacks;
    int m_totalCarnage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 3, 6, 9, 12, Skarbrand::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        { Skarbrand::Angry, 4, 5 },
        { Skarbrand::Furious, 5, 4},
        { Skarbrand::Seething, 6, 3 },
        { Skarbrand::Enraged, 7, 2 },
        { Skarbrand::Incandescent, 8, 1 }
    };

bool Skarbrand::s_registered = false;

Skarbrand::Skarbrand() :
    Unit("Skarbrand", 8, WOUNDS, 10, 4, true),
    m_slaughter(Weapon::Type::Melee, "Slaughter", 2, 4, 4, 3, -2, 3),
    m_carnage(Weapon::Type::Melee, "Carnage", 2, 1, 4, 0, 0, 0)
{
    m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, SKARBRAND};
}

bool Skarbrand::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_slaughter);
    model.addMeleeWeapon(&m_carnage);
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

    Unit::onWounded();
}

int Skarbrand::getDamageTableIndex() const
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

} // namespace Khorne