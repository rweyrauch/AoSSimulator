/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/ChameleonSkinks.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

bool ChameleonSkinks::s_registered = false;

ChameleonSkinks::ChameleonSkinks() :
    SeraphonBase("Chameleon Skinks", 8, WOUNDS, 5, 6, false),
    m_dartpipe(Weapon::Type::Missile, "Dartpipe", 16, 2, 3, 4, 0, 1),
    m_dagger(Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINKS, CHAMELEON_SKINKS};
    m_weapons = {&m_dartpipe, &m_dagger};
}

bool ChameleonSkinks::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_dartpipe);
        model->addMeleeWeapon(&m_dagger);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *ChameleonSkinks::Create(const ParameterList &parameters)
{
    auto unit = new ChameleonSkinks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChameleonSkinks::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Chameleon Skinks", factoryMethod);
    }
}

int ChameleonSkinks::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

Wounds ChameleonSkinks::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Star-venom
    if ((hitRoll == 6) && (weapon->name() == m_dartpipe.name()))
    {
        return { 0, 1};
    }
    return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Seraphon
