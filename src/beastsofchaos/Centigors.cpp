/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Centigors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Centigors::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Centigors::MIN_UNIT_SIZE, Centigors::MIN_UNIT_SIZE,
            Centigors::MAX_UNIT_SIZE, Centigors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    { BEASTS_OF_CHAOS }
};

bool Centigors::s_registered = false;

Centigors::Centigors() :
    BeastsOfChaosBase("Centigors", 14, WOUNDS, 5, 5, false),
    m_centigorSpear(Weapon::Type::Melee, "Centigor Spear", 2, 2, 4, 4, 0, 1),
    m_centigorSpearGorehoof(Weapon::Type::Melee, "Centigor Spear", 2, 3, 4, 4, 0, 1),
    m_clawedForelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, CENTIGORS};
    m_weapons = { &m_centigorSpear, &m_centigorSpearGorehoof, &m_clawedForelimbs };
}

bool Centigors::configure(int numModels, bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;

    m_runAndCharge = m_brayhorn;

    auto gorehoof = new Model(BASESIZE, WOUNDS);
    gorehoof->addMeleeWeapon(&m_centigorSpearGorehoof);
    gorehoof->addMeleeWeapon(&m_clawedForelimbs);
    addModel(gorehoof);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_centigorSpear);
        model->addMeleeWeapon(&m_clawedForelimbs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Centigors::Create(const ParameterList &parameters)
{
    auto unit = new Centigors();
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

void Centigors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Centigors", factoryMethod);
    }
}

int Centigors::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);
    // Drunken Revelry
    if (m_drunkenRevelry)
    {
        modifier += 1;
    }
    return modifier;
}

int Centigors::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_bannerBearer)
    {
        modifier += 1;
    }
    return modifier;
}

int Centigors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);
    // Drunken Revelry
    if (m_drunkenRevelry)
    {
        modifier += 1;
    }
    return modifier;
}

int Centigors::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Beastbucklers
    if (!weapon->isMissile())
    {
        modifier += 1;
    }
    return modifier;
}

Rerolls Centigors::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Charging Spear
    if (m_charged && (weapon->name() == m_centigorSpear.name()))
    {
        return RerollFailed;
    }
    return Unit::toWoundRerolls(weapon, target);
}

} // namespace BeastsOfChaos