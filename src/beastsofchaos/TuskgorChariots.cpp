/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/TuskgorChariots.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{

bool TuskgorChariots::s_registered = false;

TuskgorChariots::TuskgorChariots() :
    BeastsOfChaosBase("Tuskgor Chariots", 10, WOUNDS, 6, 4, false),
    m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
    m_gnarledSpear(Weapon::Type::Melee, "Gnarled Spear", 2, 1, 4, 4, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, GORS, BEASTS_OF_CHAOS, BRAYHERD, TUSKGOR_CHARIOTS};
    m_weapons = { &m_despoilerAxe, &m_gnarledSpear, &m_tusksAndHooves };
}

bool TuskgorChariots::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_despoilerAxe);
        model->addMeleeWeapon(&m_gnarledSpear);
        model->addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *TuskgorChariots::Create(const ParameterList &parameters)
{
    auto unit = new TuskgorChariots();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TuskgorChariots::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
            },
            CHAOS,
            { BEASTS_OF_CHAOS }
        };

        s_registered = UnitFactory::Register("Tuskgor Chariots", factoryMethod);
    }
}

std::string TuskgorChariots::ValueToString(const Parameter &parameter)
{
    return BeastsOfChaosBase::ValueToString(parameter);
}

int TuskgorChariots::EnumStringToInt(const std::string &enumString)
{
    return BeastsOfChaosBase::EnumStringToInt(enumString);
}

int TuskgorChariots::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    int modifier = Unit::toHitModifier(weapon, unit);
    if (unit->remainingModels() >= 10)
    {
        modifier += 1;
    }
    return modifier;
}

Rerolls TuskgorChariots::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    if (unit->hasKeyword(ORDER))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

int TuskgorChariots::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Unit::extraAttacks(attackingModel, weapon, target);
    // Tuskgor Charge
    if (m_charged)
    {
        extra++;
    }
    return extra;
}

Rerolls TuskgorChariots::chargeRerolls() const
{
    // Tuskgor Charge
    return RerollFailed;
}

int TuskgorChariots::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace BeastsOfChaos