/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <freeagents/GotrekGurnisson.h>
#include <Board.h>
#include <UnitFactory.h>

namespace FreeAgent
{
static FactoryMethod factoryMethod = {
    GotrekGurnisson::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    FREE_AGENT
};

bool GotrekGurnisson::s_registered = false;

GotrekGurnisson::GotrekGurnisson() :
    Unit("Gotrek Gurnisson", 4, WOUNDS, 10, 4, false),
    m_zangromThaz(Weapon::Type::Melee, "Zangrom-Thaz", 1, 6, 3, 3, -2, 3)
{
    m_keywords = {ORDER, DUARDIN, HERO, GOTREK_GURNISSON};
}

bool GotrekGurnisson::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_zangromThaz);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void GotrekGurnisson::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_zangromThaz);
}

Unit *GotrekGurnisson::Create(const ParameterList &parameters)
{
    auto unit = new GotrekGurnisson();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GotrekGurnisson::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gotrek Gurnisson", factoryMethod);
    }
}

Wounds GotrekGurnisson::applyWoundSave(const Wounds &wounds)
{
    // Avatar of Grimnir
    Wounds appliedWounds;
    appliedWounds.normal = std::min(1, wounds.normal);
    appliedWounds.mortal = std::min(1, wounds.mortal);

    Dice dice;
    // Should Plate of Edassa
    if (appliedWounds.normal > 0)
    {
        if (dice.rollD6() >= 3)
        {
            appliedWounds.normal = 0;
        }
    }
    if (appliedWounds.mortal > 0)
    {
        if (dice.rollD6() >= 3)
        {
            appliedWounds.mortal = 0;
        }
    }
    return appliedWounds;
}

Rerolls GotrekGurnisson::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Krag Blackhammer's Master Rune
    return RerollFailed;
}

Rerolls GotrekGurnisson::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Krag Blackhammer's Master Rune
    return RerollFailed;
}

Wounds GotrekGurnisson::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    Wounds wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Krag Blackhammer's Master Rune
    if (hitRoll >= 6)
    {
        Dice dice;
        wounds.mortal += dice.rollD6();
    }
    return wounds;
}

Wounds GotrekGurnisson::onEndCombat(PlayerId player)
{
    Unit* meleeTarget = m_meleeTarget;
    if (!meleeTarget)
    {
        meleeTarget = Board::Instance()->getNearestUnit(this, GetEnemyId(m_owningPlayer));
    }

    Wounds wounds = {0, 0};

    // Unstoppable Battle Fury
    auto totalMoveDistance = distanceTo(meleeTarget);
    if (meleeTarget && (totalMoveDistance <= 3.0f))
    {
        Math::Ray ray(position(), meleeTarget->position());
        auto newPos = ray.point_at(totalMoveDistance);
        setPosition(newPos, ray.get_direction());

        int numSlain = 0;
        wounds += fight(-1, meleeTarget, numSlain);
        if (wounds.normal)
        {
            SimLog(Verbosity::Narrative, "Gotrek attacks again for {%d, %d} wounds.", wounds.normal, wounds.mortal);
        }
    }

    wounds += Unit::onEndCombat(player);

    return wounds;
}


} // namespace Fyreslayers