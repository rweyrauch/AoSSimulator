/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Kroxigor.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Kroxigor::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    Kroxigor::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Kroxigor::MIN_UNIT_SIZE, Kroxigor::MIN_UNIT_SIZE,
            Kroxigor::MAX_UNIT_SIZE, Kroxigor::MIN_UNIT_SIZE
        },
        {ParamType::Integer, "Moon Hammers", 1, 0, Kroxigor::MAX_UNIT_SIZE/3, 1},
    },
    ORDER,
    { SERAPHON }
};

bool Kroxigor::s_registered = false;

Kroxigor::Kroxigor() :
    SeraphonBase("Kroxigor", 8, WOUNDS, 10, 4, false),
    m_maul(Weapon::Type::Melee, "Drakebite Maul", 2, 4, 4, 3, 0, 2),
    m_hammer(Weapon::Type::Melee, "Moon Hammer", 2, 1, 4, 3, -1, 2),
    m_jaws(Weapon::Type::Melee, "Vice-like Jaws", 1, 1, 4, 3, -1, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SAURUS, SAURUS_GUARD};
    m_weapons = {&m_maul, &m_hammer, &m_jaws};
}

bool Kroxigor::configure(int numModels, int numMoonhammers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxHammers = numModels / 3;
    if (numMoonhammers > maxHammers)
    {
        return false;
    }

    for (auto i = 0; i < numMoonhammers; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }
    for (auto i = numMoonhammers; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_maul);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Kroxigor::Create(const ParameterList &parameters)
{
    auto unit = new Kroxigor();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numHammers = GetIntParam("Moon Hammers", parameters, MIN_UNIT_SIZE/3);

    bool ok = unit->configure(numModels, numHammers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Kroxigor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kroxigor", factoryMethod);
    }
}


int Kroxigor::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

Rerolls Kroxigor::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Energy Transference
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SKINKS, 3.0f);
    if (unit)
    {
        return RerollOnes;
    }
    return Unit::toWoundRerolls(weapon, target);
}

} //namespace Seraphon
