/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/SistersOfSlaughter.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {

static FactoryMethod factoryMethod = {
    SistersOfSlaughter::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = SistersOfSlaughter::MIN_UNIT_SIZE}, SistersOfSlaughter::MIN_UNIT_SIZE,
         SistersOfSlaughter::MAX_UNIT_SIZE, SistersOfSlaughter::MIN_UNIT_SIZE},
        {ParamType::Boolean, "sacrificialKnife", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "standardBearers", {.m_boolValue = true}, false, false},
    }
};

Weapon SistersOfSlaughter::s_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1);
Weapon SistersOfSlaughter::s_sacrificialKnifeHandmaiden(Weapon::Type::Melee, "Sacrificial Knife (Handmaiden)", 1, 2, 2, 4, 0, 1);
Weapon SistersOfSlaughter::s_barbedWhip(Weapon::Type::Melee, "Barbed Whip", 2, 2, 3, 4, 0, 1);
Weapon SistersOfSlaughter::s_barbedWhipHandmaiden(Weapon::Type::Melee, "Barbed Whip (Handmaiden)", 2, 2, 2, 4, 0, 1);

bool SistersOfSlaughter::s_registered = false;

SistersOfSlaughter::SistersOfSlaughter() :
    DaughterOfKhaine("Sisters of Slaughter", 6, WOUNDS, 7, 6, false)
{
    m_keywords = { ORDER, AELF, DAUGHTERS_OF_KHAINE, SISTERS_OF_SLAUGHTER };
}

bool SistersOfSlaughter::configure(int numModels, bool sacrificialKnife, bool hornblowers, bool standardBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_sacrificialKnife = sacrificialKnife;
    m_hornblowers = hornblowers;
    m_standardBearers = standardBearers;

    if (m_hornblowers)
        m_runAndCharge = true;

    Model handmaiden(BASESIZE, WOUNDS);
    if (m_sacrificialKnife)
        handmaiden.addMeleeWeapon(&s_sacrificialKnifeHandmaiden);
    handmaiden.addMeleeWeapon(&s_barbedWhipHandmaiden);
    addModel(handmaiden);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_sacrificialKnife)
            model.addMeleeWeapon(&s_sacrificialKnife);
        model.addMeleeWeapon(&s_barbedWhip);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_sacrificialKnife.name() << ": " << s_sacrificialKnife.strength() << std::endl;
        std::cout << "\t" << s_sacrificialKnifeHandmaiden.name() << ": " << s_sacrificialKnifeHandmaiden.strength() << std::endl;
        std::cout << "\t" << s_barbedWhip.name() << ": " << s_barbedWhip.strength() << std::endl;
        std::cout << "\t" << s_barbedWhipHandmaiden.name() << ": " << s_barbedWhipHandmaiden.strength() << std::endl;
    }

    return true;
}

Unit *SistersOfSlaughter::Create(const ParameterList &parameters)
{
    auto unit = new SistersOfSlaughter();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool sacrificialKnife = GetBoolParam("sacrificialKnife", parameters, true);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);

    bool ok = unit->configure(numModels, sacrificialKnife, hornblowers, standardBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SistersOfSlaughter::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sisters of Slaughter", factoryMethod);
    }
}

int SistersOfSlaughter::rollBattleshock() const
{
    if (m_standardBearers)
    {
        Dice dice;
        int r1 = dice.rollD6();
        int r2 = dice.rollD6();
        return std::min(r1, r2);
    }
    return Unit::rollBattleshock();
}

int SistersOfSlaughter::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    // Bladed Bucklers
    if (!m_sacrificialKnife)
        modifier += 1;
    return modifier;
}

Wounds SistersOfSlaughter::computeReturnedDamage(const Weapon *weapon,
                                                 const Dice::RollResult &saveRolls) const
{
    // Bladed Bucklers
    if (!weapon->isMissile())
    {
        // 1 mortal wound for each save of a 6
        Wounds returnedDamage = {0, saveRolls.numUnmodified6s()};
        return returnedDamage;
    }
    return Unit::computeReturnedDamage(weapon, saveRolls);
}

} // namespace DaughtersOfKhaine