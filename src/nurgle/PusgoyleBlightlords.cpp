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
static const int BASESIZE = 60;
static const int WOUNDS = 7;
static const int MIN_UNIT_SIZE = 3;
static const int MAX_UNIT_SIZE = 12;
static const int POINTS_PER_BLOCK = 200;
static const int POINTS_MAX_UNIT_SIZE = 800;

bool PusgoyleBlightlords::s_registered = false;

PusgoyleBlightlords::PusgoyleBlightlords() :
    NurgleBase("Pusgoyle Blightlords", 8, WOUNDS, 10, 4, true),
    m_blightedWeapon(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1),
    m_dolorousTocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2),
    m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
    m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, PUSGOYLE_BLIGHTLORDS};
    m_weapons = {&m_blightedWeapon, &m_dolorousTocsin, &m_mouthparts, &m_venemousSting};
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
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blightedWeapon);
        model->addMeleeWeapon(&m_mouthparts);
        model->addMeleeWeapon(&m_venemousSting);
        if (i < numTocsins)
        {
            model->addMeleeWeapon(&m_dolorousTocsin);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
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
        static FactoryMethod factoryMethod = {
            PusgoyleBlightlords::Create,
            NurgleBase::ValueToString,
            NurgleBase::EnumStringToInt,
            PusgoyleBlightlords::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Integer, "Dolorous Tocsin", 0, 0, MAX_UNIT_SIZE/2},
            },
            CHAOS,
            { NURGLE }
        };
        s_registered = UnitFactory::Register("Pusgoyle Blightlords", factoryMethod);
    }
}

Wounds PusgoyleBlightlords::applyWoundSave(const Wounds &wounds)
{
    // Disgustingly Resilient
    Dice::RollResult woundSaves, mortalSaves;
    Dice::rollD6(wounds.normal, woundSaves);
    Dice::rollD6(wounds.mortal, mortalSaves);

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
        return Dice::rollD6();
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

void PusgoyleBlightlords::onStartHero(PlayerId player)
{
    // Virulent Discharge
    auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(NURGLE))
        {
            // Heal D3
            ip->heal(Dice::rollD3());
        }
        else
        {
            // Inflict D3 mortal wounds
            ip->applyDamage({0, Dice::rollD3()});
        }
    }
    Unit::onStartHero(player);
}

int PusgoyleBlightlords::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Nurgle

