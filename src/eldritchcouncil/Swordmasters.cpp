/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Swordmasters.h>
#include <UnitFactory.h>

namespace EldritchCouncil
{
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 160;
static const int POINTS_MAX_UNIT_SIZE = 420;

bool Swordmasters::s_registered = false;

Swordmasters::Swordmasters() :
    Unit("Swordmasters", 6, WOUNDS, 7, 4, false),
    m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 2, 3, 3, -1, 1),
    m_greatswordLord(Weapon::Type::Melee, "Greatsword", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, SWORDMASTERS};
    m_weapons = {&m_greatsword, &m_greatswordLord};
}

bool Swordmasters::configure(int numModels, bool hornblower, bool standardBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    auto lord = new Model(BASESIZE, wounds());
    lord->addMeleeWeapon(&m_greatswordLord);
    addModel(lord);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_greatsword);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Swordmasters::Create(const ParameterList &parameters)
{
    auto unit = new Swordmasters();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, hornblower, standardBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Swordmasters::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            nullptr,
            nullptr,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
            },
            ORDER,
            { ELDRITCH_COUNCIL }
        };
        s_registered = UnitFactory::Register("Swordmasters", factoryMethod);
    }
}

Rerolls Swordmasters::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // A Blur of Blades
    return RerollOnes;
}

Rerolls Swordmasters::toSaveRerolls(const Weapon *weapon) const
{
    // Deflect Shots
    if (weapon->isMissile())
    {
        return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

int Swordmasters::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace EldritchCouncil