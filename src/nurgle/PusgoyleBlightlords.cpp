/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PusgoyleBlightlords.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    PusgoyleBlightlords::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", PusgoyleBlightlords::MIN_UNIT_SIZE, PusgoyleBlightlords::MIN_UNIT_SIZE,
            PusgoyleBlightlords::MAX_UNIT_SIZE, PusgoyleBlightlords::MIN_UNIT_SIZE
        },
        {ParamType::Integer, "Dolorous Tocsin", 0, 0, PusgoyleBlightlords::MAX_UNIT_SIZE/2},
    },
    CHAOS,
    NURGLE
};

bool PusgoyleBlightlords::s_registered = false;

PusgoyleBlightlords::PusgoyleBlightlords() :
    Unit("Pusgoyle Blightlords", 8, WOUNDS, 10, 4, true),
    m_blightedWeapon(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1),
    m_dolorousTocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2),
    m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
    m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, PUSGOYLE_BLIGHTLORDS};
}

bool PusgoyleBlightlords::configure(int numModels, int numTocsins)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    if (numTocsins > MAX_UNIT_SIZE/2)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_blightedWeapon);
        model.addMeleeWeapon(&m_mouthparts);
        model.addMeleeWeapon(&m_venemousSting);
        if (i < numTocsins)
        {
            model.addMeleeWeapon(&m_dolorousTocsin);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void PusgoyleBlightlords::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_blightedWeapon);
    visitor(&m_dolorousTocsin);
    visitor(&m_mouthparts);
    visitor(&m_venemousSting);
}

Unit *PusgoyleBlightlords::Create(const ParameterList &parameters)
{
    auto unit = new PusgoyleBlightlords();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numTocsins = GetIntParam("Dolorous Tocsin", parameters, 0);

    bool ok = unit->configure(numModels, numTocsins);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PusgoyleBlightlords::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Pusgoyle Blightlords", factoryMethod);
    }
}

Wounds PusgoyleBlightlords::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Disgustingly Resilient
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

int PusgoyleBlightlords::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Blighted Weapon
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_blightedWeapon.name()))
    {
        Dice dice;
        return dice.rollD6();
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

void PusgoyleBlightlords::onStartHero(PlayerId player)
{
    Dice dice;
    // Virulent Discharge
    auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(NURGLE))
        {
            // Heal D3
            ip->heal(dice.rollD3());
        }
        else
        {
            // Inflict D3 mortal wounds
            ip->applyDamage({0, dice.rollD3()});
        }
    }
    Unit::onStartHero(player);
}

} // namespace Nurgle
