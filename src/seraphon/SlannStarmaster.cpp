/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SlannStarmaster.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>

namespace Seraphon
{
static const int BASESIZE = 50;
static const int WOUNDS = 7;
static const int POINTS_PER_UNIT = 260;

bool SlannStarmaster::s_registered = false;

SlannStarmaster::SlannStarmaster() :
    SeraphonBase("Slann Starmaster", 5, WOUNDS, 9, 4, true),
    m_lightning(Weapon::Type::Melee, "Azure Lightning", 3, 6, 4, 3, -1, 1)
{
    m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER};
    m_weapons = {&m_lightning};
}

bool SlannStarmaster::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_lightning);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SlannStarmaster::Create(const ParameterList &parameters)
{
    auto unit = new SlannStarmaster();

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SlannStarmaster::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Slann Starmaster", factoryMethod);
    }
}

void SlannStarmaster::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    // Foresight
    if (owningPlayer() == player)
    {
        Dice::RollResult result;
        Dice::rollD6(2, result);
        m_roster->addCommandPoints(result.rollsGE(4));
    }
}

int SlannStarmaster::castingModifier() const
{
    auto mod = Unit::castingModifier();

    // Masters of Order
    mod++;

    return mod;
}

int SlannStarmaster::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Seraphon
