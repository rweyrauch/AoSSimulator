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
        {
            ParamType::Integer, "Models", SquiqHerd::MIN_UNIT_SIZE, SquiqHerd::MIN_UNIT_SIZE,
            SquiqHerd::MAX_UNIT_SIZE, SquiqHerd::MIN_UNIT_SIZE
        }
    },
    DESTRUCTION,
    { GLOOMSPITE_GITZ }
};

bool SquiqHerd::s_registered = false;

SquiqHerd::SquiqHerd() :
    GloomspiteGitzBase("Squig Herd", 5, WOUNDS, 3, 6, false),
    m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
    m_squigProdder(Weapon::Type::Melee, "Squig Prodder", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HERD};
    m_weapons = {&m_fangFilledGob, &m_squigProdder};
}

bool SquiqHerd::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    const int numHerders = (numModels + 5) / 6;

    // Add the herder
    for (auto i = 0; i < numHerders; i++)
    {
        auto herder = new Model(BASESIZE, WOUNDS);
        herder->addMeleeWeapon(&m_squigProdder);
        addModel(herder);
    }

    // and the squigs
    for (auto i = numHerders; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_fangFilledGob);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *SquiqHerd::Create(const ParameterList &parameters)
{
    auto unit = new SquiqHerd();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
    {
        return RerollFailed;
    }

    return GloomspiteGitzBase::runRerolls();
}

Rerolls SquiqHerd::chargeRerolls() const
{
    // Go Dat Way!
    if (hasHerder())
    {
        return RerollFailed;
    }

    return GloomspiteGitzBase::chargeRerolls();
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
        auto closestTarget = board->getNearestUnit(this, PlayerId::None);
        if (closestTarget && distanceTo(closestTarget) <= 6.0f)
        {
            closestTarget->applyDamage({0, numMortalWounds});
        }
    }
    GloomspiteGitzBase::onFlee(numFled);
}

} // namespace GloomspiteGitz
