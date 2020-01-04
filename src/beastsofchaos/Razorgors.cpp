/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Razorgors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Razorgors::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Razorgors::MIN_UNIT_SIZE, Razorgors::MIN_UNIT_SIZE,
            Razorgors::MAX_UNIT_SIZE, Razorgors::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Razorgors::s_registered = false;

Razorgors::Razorgors() :
    BeastsOfChaosBase("Razorgors", 10, WOUNDS, 6, 5, false),
    m_tusksAndHooves(Weapon::Type::Melee, "Large Tusks and Hooves", 1, 4, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, RAZORGORS};
}

bool Razorgors::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
            auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Razorgors::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_tusksAndHooves);
}

Unit *Razorgors::Create(const ParameterList &parameters)
{
    auto unit = new Razorgors();
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

void Razorgors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Razorgors", factoryMethod);
    }
}

Wounds Razorgors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    Wounds damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Uncontrollable Stampede
    if (m_charged && (hitRoll == 6))
    {
        damage.mortal += 1;
    }
    return damage;
}

} // namespace BeastsOfChaos