/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Hellstriders.h>
#include <UnitFactory.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    Hellstriders::Create,
    Hellstriders::ValueToString,
    Hellstriders::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Hellstriders::MIN_UNIT_SIZE, Hellstriders::MIN_UNIT_SIZE,
            Hellstriders::MAX_UNIT_SIZE, Hellstriders::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", Hellstriders::ClawSpear, Hellstriders::ClawSpear, Hellstriders::Hellscourge, 1},
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
    },
    CHAOS,
    SLAANESH
};

bool Hellstriders::s_registered = false;

Hellstriders::Hellstriders() :
    SlaaneshBase("Hellstriders", 14, WOUNDS, 6, 4, false),
    m_clawSpear(Weapon::Type::Melee, "Claw-spear", 1, 1, 3, 4, -1, 1),
    m_clawSpearReaver(Weapon::Type::Melee, "Claw-spear", 1, 2, 3, 4, -1, 1),
    m_hellscourge(Weapon::Type::Melee, "Hellscourge", 3, 1, 3, 4, 0, 1),
    m_hellscourgeReaver(Weapon::Type::Melee, "Hellscourge", 3, 2, 3, 4, 0, 1),
    m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
    m_weapons = {&m_clawSpear, &m_clawSpearReaver, &m_hellscourge, &m_hellscourgeReaver, &m_poisonedTongue};
}

bool Hellstriders::configure(int numModels, WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_bannerBearer = bannerBearer;
    m_hornblower = hornblower;

    auto reaver = new Model(BASESIZE, WOUNDS);
    if (weapons == ClawSpear)
        reaver->addMeleeWeapon(&m_clawSpearReaver);
    else if (weapons == Hellscourge)
        reaver->addMeleeWeapon(&m_hellscourgeReaver);
    reaver->addMeleeWeapon(&m_poisonedTongue);
    addModel(reaver);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == ClawSpear)
            model->addMeleeWeapon(&m_clawSpear);
        else if (weapons == Hellscourge)
            model->addMeleeWeapon(&m_hellscourge);
        model->addMeleeWeapon(&m_poisonedTongue);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Hellstriders::Create(const ParameterList &parameters)
{
    auto unit = new Hellstriders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, ClawSpear);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    bool ok = unit->configure(numModels, weapons, iconBearer, bannerBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Hellstriders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Hellstriders", factoryMethod);
    }
}

std::string Hellstriders::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == ClawSpear)
        {
            return "Claw-spear";
        }
        else if (parameter.m_intValue == Hellscourge)
        {
            return "Hellscourge";
        }
    }
    return SlaaneshBase::ValueToString(parameter);
}

int Hellstriders::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Claw-spear") { return ClawSpear; }
    else if (enumString == "Hellscourge") { return Hellscourge; }
    return SlaaneshBase::EnumStringToInt(enumString);
}

Rerolls Hellstriders::chargeRerolls() const
{
    if (m_bannerBearer)
    {
        return RerollFailed;
    }
    return Unit::chargeRerolls();
}

int Hellstriders::braveryModifier() const
{
    int modifier = Unit::braveryModifier();
    if (m_iconBearer)
    {
        modifier += 2;
    }
    return modifier;
}

} // namespace Slaanesh
