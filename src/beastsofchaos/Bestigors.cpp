/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bestigors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Bestigors::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    Bestigors::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Bestigors::MIN_UNIT_SIZE, Bestigors::MIN_UNIT_SIZE,
            Bestigors::MAX_UNIT_SIZE, Bestigors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    { BEASTS_OF_CHAOS }
};

bool Bestigors::s_registered = false;

Bestigors::Bestigors() :
    BeastsOfChaosBase("Bestigors", 6, WOUNDS, 6, 4, false),
    m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
    m_despoilerAxeGougeHorn(Weapon::Type::Melee, "Despoiler Axe", 1, 3, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, BESTIGORS};
    m_weapons = { &m_despoilerAxe, &m_despoilerAxeGougeHorn };
}

bool Bestigors::configure(int numModels, bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;

    m_runAndCharge = m_brayhorn;

    auto gougehorn = new Model(BASESIZE, WOUNDS);
    gougehorn->addMeleeWeapon(&m_despoilerAxeGougeHorn);
    addModel(gougehorn);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_despoilerAxe);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Bestigors::Create(const ParameterList &parameters)
{
    auto unit = new Bestigors();
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

void Bestigors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bestigors", factoryMethod);
    }
}

int Bestigors::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    int modifier = Unit::toHitModifier(weapon, unit);
    if (unit->remainingModels() >= 10)
    {
        modifier += 1;
    }
    return modifier;
}

Rerolls Bestigors::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    if (unit->hasKeyword(ORDER))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

int Bestigors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Beastial Charge
    int attacks = Unit::extraAttacks(nullptr, weapon, target);
    if (m_charged)
    {
        attacks += 1;
    }
    return attacks;
}

int Bestigors::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_bannerBearer)
    {
        modifier += 1;
    }
    return modifier;
}

int Bestigors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }

    return points;
}

} // namespace BeastsOfChaos