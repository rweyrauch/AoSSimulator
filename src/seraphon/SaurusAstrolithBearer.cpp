/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusAstrolithBearer.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

bool SaurusAstrolithBearer::s_registered = false;

SaurusAstrolithBearer::SaurusAstrolithBearer() :
    SeraphonBase("Saurus Astrolith Bearer", 5, WOUNDS, 8, 4, false),
    m_warpick(Weapon::Type::Melee, "Celestite Warpick", 1, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, HERO, TOTEM, ASTROLITH_BEARER};
    m_weapons = {&m_warpick, &m_jaws};
}

bool SaurusAstrolithBearer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_warpick);
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SaurusAstrolithBearer::Create(const ParameterList &parameters)
{
    auto unit = new SaurusAstrolithBearer();

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

void SaurusAstrolithBearer::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
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

        s_registered = UnitFactory::Register("Saurus Astrolith Bearer", *factoryMethod);
    }
}

} //namespace Seraphon
