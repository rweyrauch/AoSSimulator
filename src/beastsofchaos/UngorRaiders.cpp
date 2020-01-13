/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/UngorRaiders.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    UngorRaiders::Create,
    UngorRaiders::ValueToString,
    UngorRaiders::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", UngorRaiders::MIN_UNIT_SIZE, UngorRaiders::MIN_UNIT_SIZE,
            UngorRaiders::MAX_UNIT_SIZE, UngorRaiders::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    { BEASTS_OF_CHAOS }
};

bool UngorRaiders::s_registered = false;

UngorRaiders::UngorRaiders() :
    BeastsOfChaosBase("Ungor Raiders", 6, WOUNDS, 4, 6, false),
    m_raiderBow(Weapon::Type::Missile, "Raider Bow", 18, 1, 4, 4, 0, 1),
    m_raiderBowHalfhorn(Weapon::Type::Missile, "Raider Bow", 18, 1, 3, 4, 0, 1),
    m_jaggedShank(Weapon::Type::Melee, "Jagged Shank", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, UNGOR, BEASTS_OF_CHAOS, BRAYHERD, UNGOR_RAIDERS};
    m_weapons = { &m_raiderBow, &m_raiderBowHalfhorn, &m_jaggedShank };
}

bool UngorRaiders::configure(int numModels, bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;

    m_runAndCharge = m_brayhorn;

    auto halfhorn = new Model(BASESIZE, WOUNDS);
    halfhorn->addMissileWeapon(&m_raiderBowHalfhorn);
    halfhorn->addMeleeWeapon(&m_jaggedShank);
    addModel(halfhorn);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_raiderBow);
        model->addMeleeWeapon(&m_jaggedShank);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *UngorRaiders::Create(const ParameterList &parameters)
{
    auto unit = new UngorRaiders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure(numModels, brayhorn, bannerBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void UngorRaiders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ungor Raiders", factoryMethod);
    }
}

std::string UngorRaiders::ValueToString(const Parameter &parameter)
{
    return BeastsOfChaosBase::ValueToString(parameter);
}

int UngorRaiders::EnumStringToInt(const std::string &enumString)
{
    return BeastsOfChaosBase::EnumStringToInt(enumString);
}

Rerolls UngorRaiders::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Baying Anger
    if (weapon->isMissile())
    {
        if (remainingModels() >= 30)
        {
            return RerollOnesAndTwos;
        }
        else if (remainingModels() >= 20)
        {
            return RerollOnes;
        }
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace BeastsOfChaos