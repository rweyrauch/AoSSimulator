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
static FactoryMethod factoryMethod = {
    TuskgorChariots::Create,
    TuskgorChariots::ValueToString,
    TuskgorChariots::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", TuskgorChariots::MIN_UNIT_SIZE, TuskgorChariots::MIN_UNIT_SIZE,
            TuskgorChariots::MAX_UNIT_SIZE, TuskgorChariots::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool TuskgorChariots::s_registered = false;

TuskgorChariots::TuskgorChariots() :
    BeastsOfChaosBase("Tuskgor Chariots", 10, WOUNDS, 6, 4, false),
    m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
    m_gnarledSpear(Weapon::Type::Melee, "Gnarled Spear", 2, 1, 4, 4, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, GORS, BEASTS_OF_CHAOS, BRAYHERD, TUSKGOR_CHARIOTS};
}

bool TuskgorChariots::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_despoilerAxe);
        model.addMeleeWeapon(&m_gnarledSpear);
        model.addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void TuskgorChariots::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_despoilerAxe);
    visitor(m_gnarledSpear);
    visitor(m_tusksAndHooves);
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
    // Tuskgor Charge
    if (m_charged)
    {
        return (weapon->attacks()+1);
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

Rerolls TuskgorChariots::chargeRerolls() const
{
    // Tuskgor Charge
    return RerollFailed;
}

} // namespace BeastsOfChaos