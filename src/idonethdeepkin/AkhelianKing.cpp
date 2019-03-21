/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/AkhelianKing.h>
#include <UnitFactory.h>

namespace IdonethDeepkin
{
static FactoryMethod factoryMethod = {
    AkhelianKing::Create,
    AkhelianKing::ValueToString,
    AkhelianKing::EnumStringToInt,
    {
        {ParamType::Enum, "Weapon", {.m_intValue = AkhelianKing::BladedPolearm}, AkhelianKing::BladedPolearm, AkhelianKing::Greatsword, 1},
    },
    ORDER,
    IDONETH_DEEPKIN
};

bool AkhelianKing::s_registered = false;

AkhelianKing::AkhelianKing() :
    Unit("Akhelian King", 14, WOUNDS, 8, 3, true),
    m_bladedPolearm(Weapon::Type::Melee, "Bladed Polearm", 2, 3, 3, 3, -2, RAND_D3),
    m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 4, 3, 3, -1, RAND_D3),
    m_falchion(Weapon::Type::Melee, "Falchion", 1, 3, 3, 4, 0, 1),
    m_deepmareJawsTalons(Weapon::Type::Melee, "Deepmare's Fanged Jaw and Talons", 2, 3, 3, 3, -1, 1),
    m_deepmareTails(Weapon::Type::Melee, "Deepmare's Lashing Tails", 2, 3, 3, 3, 0, 2)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, HERO, AKHELIAN, AKHELIAN_KING};
}

bool AkhelianKing::configure(WeaponOption weapon)
{
    Model model(BASESIZE, WOUNDS);
    if (weapon == BladedPolearm)
    {
        model.addMeleeWeapon(&m_bladedPolearm);
    }
    else
    {
        model.addMeleeWeapon(&m_greatsword);
    }
    model.addMeleeWeapon(&m_falchion);
    model.addMeleeWeapon(&m_deepmareJawsTalons);
    model.addMeleeWeapon(&m_deepmareTails);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AkhelianKing::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bladedPolearm);
    visitor(&m_greatsword);
    visitor(&m_falchion);
    visitor(&m_deepmareJawsTalons);
    visitor(&m_deepmareTails);
}

Unit *AkhelianKing::Create(const ParameterList &parameters)
{
    auto unit = new AkhelianKing();
    WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BladedPolearm);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AkhelianKing::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Akhelian King", factoryMethod);
    }
}

std::string AkhelianKing::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == BladedPolearm)
        {
            return "Bladed Polearm";
        }
        else if (parameter.m_intValue == Greatsword)
        {
            return "Greatsword";
        }
    }
    return ParameterValueToString(parameter);
}

int AkhelianKing::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Bladed Polearm")
    {
        return BladedPolearm;
    }
    else if (enumString == "Greatsword")
    {
        return Greatsword;
    }
    return 0;
}

} //namespace IdonethDeepkin
