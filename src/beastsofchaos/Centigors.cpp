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
static const int BASESIZE = 60; // x35 oval
static const int WOUNDS = 2;
static const int MIN_UNIT_SIZE = 5;
static const int MAX_UNIT_SIZE = 20;
static const int POINTS_PER_BLOCK = 80;
static const int POINTS_MAX_UNIT_SIZE = 320;

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

    auto gorehoof = new Model(BASESIZE, wounds());
    gorehoof->addMeleeWeapon(&m_centigorSpearGorehoof);
    gorehoof->addMeleeWeapon(&m_clawedForelimbs);
    addModel(gorehoof);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_centigorSpear);
        model->addMeleeWeapon(&m_clawedForelimbs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

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
        static FactoryMethod factoryMethod = {
            Create,
            BeastsOfChaosBase::ValueToString,
            BeastsOfChaosBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
            },
            CHAOS,
            { BEASTS_OF_CHAOS }
        };

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

int Centigors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace BeastsOfChaos