/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylChief.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static const int BASESIZE = 50;
static const int WOUNDS = 4;
static const int POINTS_PER_UNIT = 80;

bool RipperdactylChief::s_registered = false;

RipperdactylChief::RipperdactylChief() :
    SeraphonBase("Ripperdactyl Chief", 12, WOUNDS, 6, 5, true),
    m_skyblade(Weapon::Type::Melee, "Skyblade", 1, 4, 3, 4, -1, 1),
    m_jaws(Weapon::Type::Melee, "Tearing Jaws", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, HERO, RIPPERDACTYL_CHIEF};
    m_weapons = {&m_skyblade, &m_jaws};
}

bool RipperdactylChief::configure()
{
    // Add the Alpha
    auto alpha = new Model(BASESIZE, wounds());
    alpha->addMeleeWeapon(&m_skyblade);
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    m_points = ComputePoints(1);

    return true;
}

Unit *RipperdactylChief::Create(const ParameterList &parameters)
{
    auto unit = new RipperdactylChief();

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

void RipperdactylChief::Init()
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

        s_registered = UnitFactory::Register("Ripperdactyl Chief", factoryMethod);
    }
}

int RipperdactylChief::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Voracious Appetite
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_jaws.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int RipperdactylChief::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Seraphon
