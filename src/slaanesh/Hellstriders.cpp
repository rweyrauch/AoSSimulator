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
    },
    CHAOS,
    SLAANESH
};

bool Hellstriders::s_registered = false;

Hellstriders::Hellstriders() :
    Unit("Hellstriders", 14, WOUNDS, 6, 4, false),
    m_clawSpear(Weapon::Type::Melee, "Claw-spear", 1, 1, 3, 4, -1, 1),
    m_clawSpearReaver(Weapon::Type::Melee, "Claw-spear (Hellreaver)", 1, 2, 3, 4, -1, 1),
    m_hellscourge(Weapon::Type::Melee, "Hellscourge", 3, 1, 3, 4, 0, 1),
    m_hellscourgeReaver(Weapon::Type::Melee, "Hellscourge (Hellreaver)", 3, 2, 3, 4, 0, 1),
    m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
}

bool Hellstriders::configure(int numModels, Hellstriders::WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_bannerBearer = bannerBearer;
    m_hornblower = hornblower;

    return true;
}

void Hellstriders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_clawSpear);
    visitor(&m_clawSpearReaver);
    visitor(&m_hellscourge);
    visitor(&m_hellscourgeReaver);
    visitor(&m_poisonedTongue);
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
    return ParameterValueToString(parameter);
}

int Hellstriders::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Claw-spear") { return ClawSpear; }
    else if (enumString == "Hellscourge") { return Hellscourge; }
    return 0;
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
