/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHerd.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SquiqHerd::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = SquiqHerd::MIN_UNIT_SIZE}, SquiqHerd::MIN_UNIT_SIZE,
         SquiqHerd::MAX_UNIT_SIZE, SquiqHerd::MIN_UNIT_SIZE}
    }
};

bool SquiqHerd::s_registered = false;

Weapon SquiqHerd::s_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon SquiqHerd::s_squigProdder(Weapon::Type::Melee, "Squig Prodder", 1, 2, 5, 5, 0, 1);

SquiqHerd::SquiqHerd() :
    Unit("Squig Herd", 5, WOUNDS, 3, 6, false)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HERD };
}

bool SquiqHerd::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    const int numHerders = (numModels+5) / 6;

    // Add the herder
    for (auto i = 0; i < numHerders; i++)
    {
        Model herder(BASESIZE, WOUNDS);
        herder.addMeleeWeapon(&s_squigProdder);
        addModel(herder);
    }

    // and the squigs
    for (auto i = numHerders; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_fangFilledGob);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_squigProdder.name() << ": " << s_squigProdder.strength() << std::endl;
        std::cout << "\t" << s_fangFilledGob.name() << ": " << s_fangFilledGob.strength() << std::endl;
    }

    return true;
}

Unit *SquiqHerd::Create(const ParameterList &parameters)
{
    auto unit = new SquiqHerd();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SquiqHerd::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Squiq Herd", factoryMethod);
    }
}

Rerolls SquiqHerd::runRerolls() const
{
    // Go Dat Way!
    if (hasHerder())
        return RerollFailed;

    return Unit::runRerolls();
}

Rerolls SquiqHerd::chargeRerolls() const
{
    // Go Dat Way!
    if (hasHerder())
        return RerollFailed;

    return Unit::chargeRerolls();
}

bool SquiqHerd::hasHerder() const
{
    // TODO: check for herders - assuming herder is always the last model removed.
    return true;
}

void SquiqHerd::onFlee(int numFled)
{
    Dice dice;

    // Squigs Go Wild
    Dice::RollResult rolls;
    dice.rollD6(numFled, rolls);
    int numMortalWounds = rolls.rollsGE(4);

    if (numMortalWounds)
    {
        auto board = Board::Instance();

        PlayerId otherPlayer = PlayerId::Red;
        if (m_owningPlayer == PlayerId::Red)
            otherPlayer = PlayerId::Blue;
        auto otherRoster = board->getPlayerRoster(otherPlayer);
        auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
        if (closestTarget && distanceTo(closestTarget) <= 6.0f)
        {
            closestTarget->applyDamage({0, numMortalWounds});
        }
    }
    Unit::onFlee(numFled);
}

} // namespace GloomspiteGitz
