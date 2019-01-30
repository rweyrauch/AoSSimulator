/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/RockgutTroggoths.h>
#include <Board.h>
#include <Roster.h>
#include <UnitFactory.h>

namespace GloomspiteGitz
{

static FactoryMethod factoryMethod = {
    RockgutTroggoths::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = RockgutTroggoths::MIN_UNIT_SIZE}, RockgutTroggoths::MIN_UNIT_SIZE,
         RockgutTroggoths::MAX_UNIT_SIZE, RockgutTroggoths::MIN_UNIT_SIZE},
    }
};

Weapon RockgutTroggoths::s_massiveStoneMaul(Weapon::Type::Melee, "Massive Stone Maul", 2, 2, 3, 3, -2, 3);

bool RockgutTroggoths::s_registered = false;

RockgutTroggoths::RockgutTroggoths() :
    Unit("Rockgut Troggoths", 6, WOUNDS, 5, 5, false)
{
    m_keywords = { DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, ROCKGUT };
}

bool RockgutTroggoths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_massiveStoneMaul);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void RockgutTroggoths::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_massiveStoneMaul);
}

Unit *RockgutTroggoths::Create(const ParameterList &parameters)
{
    auto unit = new RockgutTroggoths();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RockgutTroggoths::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Rockgut Troggoths", factoryMethod);
    }
}

void RockgutTroggoths::hero(PlayerId player)
{
    Unit::hero(player);

    if (remainingWounds() < WOUNDS && remainingWounds() > 0)
    {
        Dice dice;
        // Regeneration - heal D3
        if (dice.rollD6() >= 4)
        {
            int woundsHealed = dice.rollD3();
            for (auto& m : m_models)
            {
                if (!m.slain() || !m.fled())
                {
                    if (m.woundsRemaining() < WOUNDS)
                    {
                        int numToHeal = std::min(woundsHealed, WOUNDS - m.woundsRemaining());
                        m.woundsRemaining() += numToHeal;
                        woundsHealed -= numToHeal;
                        if (woundsHealed <= 0) break;
                    }
                }
            }
        }
    }
}

Wounds RockgutTroggoths::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Stony Skin
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

void RockgutTroggoths::onStartShooting(PlayerId player)
{
    auto board = Board::Instance();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        // Throwin Boulders
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < 12)
        {
            Dice dice;
            int roll = dice.rollD6();
            if (roll <= nearestUnit->remainingModels())
            {
                nearestUnit->applyDamage({0, dice.rollD3()});
            }
        }
    }

    Unit::onStartShooting(player);
}

} //namespace GloomspiteGitz

