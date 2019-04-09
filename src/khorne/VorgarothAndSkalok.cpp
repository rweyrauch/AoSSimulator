/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/VorgarothAndSkalok.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    VorgarothAndSkalok::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    KHORNE
};

struct TableEntry
{
    int m_move;
    int m_clawsRend;
    int m_jawsToWound;
    int m_tailAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {5, 10, 15, 20, VorgarothAndSkalok::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, -3, 2, RAND_3D6},
        {11, -3, 3, RAND_2D6},
        {10, -2, 4, RAND_D6},
        {9,  -1, 5, RAND_D3},
        {8,  -1, 6, 1}
    };

bool VorgarothAndSkalok::s_registered = false;

VorgarothAndSkalok::VorgarothAndSkalok() :
    KhorneBase("Vorgaroth the Scarred & Skalok the Skull Host of Khorne", 14, WOUNDS, 10, 3, true),
    m_balefire(Weapon::Type::Missile, "White-hot Balefire", 20, 3, 4, 2, -3, RAND_D6),
    m_skullCleaverAxeOfKhorne(Weapon::Type::Melee, "Skull Cleaver Axe of Khorne", 1, 12, 3, 3, -2, 2),
    m_evisceratingClaws(Weapon::Type::Melee, "Eviscerating Claws", 3, RAND_D6, 3, 3, -3, 3),
    m_cavernousJaws(Weapon::Type::Melee, "Cavernous Jaws", 3, 2, 4, 2, -2, 3),
    m_brassPlatedTail(Weapon::Type::Melee, "Brass-plated Tail", 3, RAND_3D6, 4, 4, -1, 3)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, MONSTER, VORGAROTH_THE_SCARRED_AND_SKALOK_THE_SKULL_HOST_OF_KHORNE};
}

bool VorgarothAndSkalok::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_balefire);
    model.addMeleeWeapon(&m_skullCleaverAxeOfKhorne);
    model.addMeleeWeapon(&m_evisceratingClaws);
    model.addMeleeWeapon(&m_cavernousJaws);
    model.addMeleeWeapon(&m_brassPlatedTail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void VorgarothAndSkalok::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_balefire);
    visitor(&m_skullCleaverAxeOfKhorne);
    visitor(&m_evisceratingClaws);
    visitor(&m_cavernousJaws);
    visitor(&m_brassPlatedTail);
}

Unit *VorgarothAndSkalok::Create(const ParameterList &parameters)
{
    auto unit = new VorgarothAndSkalok();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VorgarothAndSkalok::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vorgaroth the Scarred & Skalok the Skull Host of Khorne", factoryMethod);
    }
}

void VorgarothAndSkalok::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_evisceratingClaws.setRend(g_damageTable[damageIndex].m_clawsRend);
    m_cavernousJaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
    m_brassPlatedTail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);

    KhorneBase::onWounded();
}

int VorgarothAndSkalok::getDamageTableIndex() const
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

int VorgarothAndSkalok::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Rerolls VorgarothAndSkalok::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Monstrous Trophies
    if (weapon->name() == m_skullCleaverAxeOfKhorne.name())
    {
        if (target->hasKeyword(MONSTER))
        {
            return RerollFailed;
        }
    }
    return KhorneBase::toWoundRerolls(weapon, target);
}

Wounds VorgarothAndSkalok::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Monstrous Trophies
    if ((hitRoll == 6) && (weapon->name() == m_skullCleaverAxeOfKhorne.name()))
    {
        Dice dice;
        return {weapon->damage(), dice.rollD3()};
    }
    return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void VorgarothAndSkalok::onCharged()
{
    Dice dice;
    // Crushing Bulk
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= 3)
        {
            ip->applyDamage({0, dice.rollD3()});
        }
    }
    KhorneBase::onCharged();
}

Wounds VorgarothAndSkalok::applyWoundSave(const Wounds &wounds)
{
    return Unit::applyWoundSave(wounds);
}

} // namespace Khorne