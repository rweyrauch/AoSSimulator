/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriest.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven
{
bool PlaguePriest::s_registered = false;

Unit *PlaguePriest::Create(const ParameterList &parameters)
{
    auto unit = new PlaguePriest();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlaguePriest::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Plague Priest", *factoryMethod);
    }
}

PlaguePriest::PlaguePriest() :
    Skaventide("Plague Priest", 6, WOUNDS, 6, 5, false),
    m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, HERO, PRIEST, PLAGUE_PRIEST};
    m_weapons = {&m_staff, &m_censer};
}

bool PlaguePriest::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_censer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds PlaguePriest::onEndCombat(PlayerId player)
{
    Dice dice;

    Wounds wounds = Unit::onEndCombat(player);

    // Poisonous Fumes
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    for (auto unit : units)
    {
        if (!unit->hasKeyword(CLANS_PESTILENS))
        {
            int mortalWounds = 0;
            int roll = dice.rollD6();
            if (roll == 6) mortalWounds = dice.rollD3();
            else if (roll >= 4) mortalWounds = 1;

            unit->applyDamage({0, mortalWounds});
            wounds.mortal += mortalWounds;
        }
    }
    return wounds;
}

int PlaguePriest::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

    // Frenzied Assault
    if (m_charged) extra++;

    return extra;
}

void PlaguePriest::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if (player == owningPlayer())
    {
        // Plague Prayers
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 13.0f);
        if (!units.empty())
        {
            Dice dice;
            Dice::RollResult result;

            auto prayerRoll = dice.rollD6();
            if (prayerRoll == 1)
            {
                // Failed - take one mortal wound.
                applyDamage({0, 1});
            }
            else if (prayerRoll >= 3)
            {
                // TODO: Implement area-of-effect Pestilence-pestilence!

                // Disease-disease!
                auto numModels = units.front()->remainingModels();
                dice.rollD6(numModels, result);
                units.front()->applyDamage({0, result.rollsGE(6)});
            }
        }
    }
}

} //namespace Skaven
