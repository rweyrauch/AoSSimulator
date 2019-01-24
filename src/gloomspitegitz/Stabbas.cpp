/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Stabbas.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz {

static FactoryMethod factoryMethod = {
    Stabbas::Create,
    Stabbas::ValueToString,
    Stabbas::EnumStringToInt,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Stabbas::MIN_UNIT_SIZE},
         Stabbas::MIN_UNIT_SIZE, Stabbas::MAX_UNIT_SIZE, Stabbas::MIN_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = Stabbas::Stabba}, Stabbas::Stabba, Stabbas::PokinSpear, 1},
        {ParamType::Integer, "bossWeapon", {.m_intValue = Stabbas::Stabba}, Stabbas::Stabba, Stabbas::PokinSpear, 1},
        {ParamType::Integer, "numBarbedNets", {.m_intValue = 3}, 0, 3 * Stabbas::MAX_UNIT_SIZE/Stabbas::MIN_UNIT_SIZE},
        {ParamType::Integer, "numGongbashers", {.m_intValue = 1}, 0, Stabbas::MAX_UNIT_SIZE/Stabbas::MIN_UNIT_SIZE},
        {ParamType::Integer, "numFlagbearers", {.m_intValue = 1}, 0, Stabbas::MAX_UNIT_SIZE/Stabbas::MIN_UNIT_SIZE},
        {ParamType::Integer, "numIconbearers", {.m_intValue = 0}, 0, Stabbas::MAX_UNIT_SIZE/Stabbas::MIN_UNIT_SIZE},
    }
};

Weapon Stabbas::s_stabba(Weapon::Type::Melee, "Stabba", 1, 1, 4, 4, 0, 1);
Weapon Stabbas::s_stabbaBoss(Weapon::Type::Melee, "Stabba (Boss)", 1, 1, 3, 4, 0, 1);
Weapon Stabbas::s_pokinSpear(Weapon::Type::Melee, "Pokin' Spear", 2, 1, 5, 4, 0, 1);
Weapon Stabbas::s_pokinSpearBoss(Weapon::Type::Melee, "Pokin' Spear (Boss)", 2, 1, 4, 4, 0, 1);
Weapon Stabbas::s_barbedNet(Weapon::Type::Melee, "Barbed Net", 2, 3, 4, 5, 0, 1);

bool Stabbas::s_registered = false;

Stabbas::Stabbas() :
    Unit("Stabbas", 5, WOUNDS, 4, 6, false)
{
    m_keywords = { DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, STABBAS };
}

bool Stabbas::configure(int numModels, WeaponOption weapons, WeaponOption bossWeapon, int numBarbedNets,
               int numGongbashers, int numFlagbearers, int numIconbearers)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    if (numBarbedNets > 3 * numModels / MIN_UNIT_SIZE)
    {
        return false;
    }
    if (numGongbashers > numModels / MIN_UNIT_SIZE)
    {
        return false;
    }
    if (numFlagbearers + numIconbearers > numModels / MIN_UNIT_SIZE)
    {
        return false;
    }

    m_numFlagbearers = numFlagbearers;
    m_numGongbashers = numGongbashers;
    m_numIconbearers = numIconbearers;

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    if (bossWeapon == Stabba)
        boss.addMeleeWeapon(&s_stabbaBoss);
    else if (bossWeapon == PokinSpear)
        boss.addMeleeWeapon(&s_pokinSpearBoss);
    addModel(boss);

    // add the nets
    for (auto i = 1; i < numBarbedNets; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_barbedNet);
        addModel(model);
    }

    // and the rest
    for (auto i = numBarbedNets + 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == Stabba)
            model.addMeleeWeapon(&s_stabba);
        else if (weapons == PokinSpear)
            model.addMeleeWeapon(&s_pokinSpear);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_stabba.name() << ": " << s_stabba.strength() << std::endl;
        std::cout << "\t" << s_stabbaBoss.name() << ": " << s_stabbaBoss.strength() << std::endl;
        std::cout << "\t" << s_pokinSpear.name() << ": " << s_pokinSpear.strength() << std::endl;
        std::cout << "\t" << s_pokinSpearBoss.name() << ": " << s_pokinSpearBoss.strength() << std::endl;
        std::cout << "\t" << s_barbedNet.name() << ": " << s_barbedNet.strength() << std::endl;
    }

    return true;
}

Unit *Stabbas::Create(const ParameterList &parameters)
{
    auto unit = new Stabbas();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, Stabba);
    WeaponOption bossWeapon = (WeaponOption)GetIntParam("bossWeapon", parameters, Stabba);
    int numBarbedNets = GetIntParam("numBarbedNets", parameters, 0);
    int numGongbashers = GetIntParam("numGongbashers", parameters, 0);
    int numFlagbearers = GetIntParam("numFlagbearers", parameters, 0);
    int numIconbearers = GetIntParam("numIconbearers", parameters, 0);

    bool ok = unit->configure(numModels, weapons, bossWeapon, numBarbedNets, numGongbashers, numFlagbearers, numIconbearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Stabbas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stabbas", factoryMethod);
    }
}

int Stabbas::toWoundModifier(const Weapon* weapon, const Unit* unit) const
{
    int modifier = Unit::toWoundModifier(weapon, unit);
    if (remainingModels() >= 30) modifier += 2;
    else if (remainingModels() >= 15) modifier += 1;
    return modifier;
}

int Stabbas::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_numGongbashers > 0) modifier += 2;
    return modifier;
}

int Stabbas::toSaveModifier(const Weapon* weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    if (m_numIconbearers > 0 && weapon->isMissile())
        modifier += 1;
    if (remainingModels() >= 10)
        modifier += 1;
    return modifier;
}

int Stabbas::battlshockModifier() const
{
    int modifier = Unit::battlshockModifier();
    if (m_numFlagbearers > 0) modifier += 1;
    return modifier;
}

std::string Stabbas::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons" || parameter.m_name == "bossWeapon")
    {
        if (parameter.m_intValue == Stabba)
            return "Stabba";
        else if (parameter.m_intValue == PokinSpear)
            return "PokinSpear";
    }
    return ParameterValueToString(parameter);
}

int Stabbas::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Stabba")
        return Stabba;
    else if (enumString == "PokinSpear")
        return PokinSpear;
    return 0;
}

} // namespace GloomspiteGitz