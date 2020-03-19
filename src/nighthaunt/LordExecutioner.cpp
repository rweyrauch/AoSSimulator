/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/LordExecutioner.h"

namespace Nighthaunt
{

bool LordExecutioner::s_registered = false;

Unit *LordExecutioner::Create(const ParameterList &parameters)
{
    auto unit = new LordExecutioner();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordExecutioner::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            LordExecutioner::Create,
            nullptr,
            nullptr,
            LordExecutioner::ComputePoints,
            {
            },
            DEATH,
            { NIGHTHAUNT }
        };
        s_registered = UnitFactory::Register("Lord Executioner", factoryMethod);
    }
}

LordExecutioner::LordExecutioner() :
    Nighthaunt("Lord Executioner", 6, WOUNDS, 10, 4, true),
    m_greataxe(Weapon::Type::Melee, "Decapitating Greataxe", 1, 3, 3, 3, -2, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, LORD_EXECUTIONER};
    m_weapons = {&m_greataxe};
}

bool LordExecutioner::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_greataxe);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Wounds LordExecutioner::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Beheading Strike
    if ((woundRoll == 6) && (weapon->name() == m_greataxe.name()))
    {
        return {weapon->damage()+2, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void LordExecutioner::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    // Staring Death in the Face
    auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0f);
    if (unit)
    {
        unit->buffModifier(ToHitMelee, -1, {Phase::Battleshock, m_battleRound, player});
    }

}

Wounds LordExecutioner::applyWoundSave(const Wounds &wounds)
{
    auto unsavedWounds = Nighthaunt::applyWoundSave(wounds);

    // Disembodied Skulls
    if (unsavedWounds.mortal)
    {
        Dice::RollResult result;
        Dice::rollD6(unsavedWounds.mortal, result);
        unsavedWounds.mortal -= result.rollsGE(5);
        unsavedWounds.mortal = std::max(0, unsavedWounds.mortal);
    }
    return unsavedWounds;
}

} // namespace Nighthaunt
