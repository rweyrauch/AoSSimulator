/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodthirsterOfInsensateRage.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    BloodthirsterOfInsensateRage::Create,
    nullptr,
    nullptr,
    {
    }
};

struct TableEntry
{
    int m_move;
    int m_axeAttacks;
    int m_outrageousCarnage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 3, 6, 9, 12, BloodthirsterOfInsensateRage::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        { 10, 4, 3 },
        { 9, 4, RAND_D3 },
        { 8, 3, RAND_D3 },
        { 7, 3, 1 },
        { 6, 2, 1 }
    };

bool BloodthirsterOfInsensateRage::s_registered = false;

BloodthirsterOfInsensateRage::BloodthirsterOfInsensateRage() :
    Unit("Bloodthirster Of Insensate Rage", 14, WOUNDS, 10, 4, true),
    m_greatAxeOfKhorne(Weapon::Type::Melee, "Great Axe of Khorne", 2, 4, 4, 2, -2, RAND_D6)
{
    m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, BLOODTHIRSTER_OF_INSENSATE_RAGE};
}

bool BloodthirsterOfInsensateRage::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_greatAxeOfKhorne);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void BloodthirsterOfInsensateRage::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_greatAxeOfKhorne);
}

Unit *BloodthirsterOfInsensateRage::Create(const ParameterList &parameters)
{
    auto unit = new BloodthirsterOfInsensateRage();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodthirsterOfInsensateRage::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodthirster Of Insensate Rage", factoryMethod);
    }
}

int BloodthirsterOfInsensateRage::getDamageTableIndex() const
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

int BloodthirsterOfInsensateRage::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Rerolls BloodthirsterOfInsensateRage::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Rage Unbound
    if (m_charged)
        return RerollOnes;
    return Unit::toHitRerolls(weapon, target);
}

void BloodthirsterOfInsensateRage::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_greatAxeOfKhorne.setAttacks(g_damageTable[damageIndex].m_axeAttacks);

    Unit::onWounded();
}

int BloodthirsterOfInsensateRage::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits &wounds)
{
    // Outrageous Carnage
    if (weapon->name() == m_greatAxeOfKhorne.name())
    {
        const int damageIndex = getDamageTableIndex();

        Dice dice;
        int mortalWounds = 0;
        int numWound6s = wounds.rolls.rollsGE(6);
        for (auto i = 0; i < numWound6s; i++)
        {
            mortalWounds = dice.rollSpecial(g_damageTable[damageIndex].m_outrageousCarnage);
        }
        // TODO: these mortal wounds are applied to all enemy units within 8".
        return mortalWounds;
    }
    return Unit::generateMortalWounds(weapon, unit, hits, wounds);
}

} // namespace Khorne