/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/WitchAelves.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {

static FactoryMethod factoryMethod = {
    WitchAelves::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = WitchAelves::MIN_UNIT_SIZE}, WitchAelves::MIN_UNIT_SIZE,
         WitchAelves::MAX_UNIT_SIZE, WitchAelves::MIN_UNIT_SIZE},
        {ParamType::Boolean, "pairedKnives", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "standardBearers", {.m_boolValue = true}, false, false},
    }
};

Weapon WitchAelves::s_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1);
Weapon WitchAelves::s_sacrificialKnifeHag(Weapon::Type::Melee, "Sacrificial Knife (Hag)", 1, 2, 2, 4, 0, 1);

bool WitchAelves::s_registered = false;

WitchAelves::WitchAelves() :
    DaughterOfKhaine("Witch Aelves", 6, WOUNDS, 7, 6, false)
{
    m_keywords = { ORDER, AELF, DAUGHTERS_OF_KHAINE, WITCH_AELVES };
}

bool WitchAelves::configure(int numModels, bool pairedKnives, bool hornblowers, bool standardBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_pairedKnives = pairedKnives;
    m_hornblowers = hornblowers;
    m_standardBearers = standardBearers;

    if (m_hornblowers)
        m_runAndCharge = true;

    Model hag(BASESIZE, WOUNDS);
    hag.addMeleeWeapon(&s_sacrificialKnifeHag);
    addModel(hag);

    for (auto i = 1; i < numModels; i++)
    {
        Model witch(BASESIZE, WOUNDS);
        witch.addMeleeWeapon(&s_sacrificialKnife);
        addModel(witch);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_sacrificialKnife.name() << ": " << s_sacrificialKnife.strength() << std::endl;
        std::cout << "\t" << s_sacrificialKnifeHag.name() << ": " << s_sacrificialKnifeHag.strength() << std::endl;
    }

    return true;
}

Unit *WitchAelves::Create(const ParameterList &parameters)
{
    auto unit = new WitchAelves();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool pairedKnives = GetBoolParam("pairedKnives", parameters, true);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);

    bool ok = unit->configure(numModels, pairedKnives, hornblowers, standardBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WitchAelves::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Witch Aelves", factoryMethod);
    }
}

int WitchAelves::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);
    if (m_pairedKnives) attacks += 1;
    return attacks;
}

int WitchAelves::rollBattleshock() const
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

int WitchAelves::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    // Bladed Bucklers
    if (!m_pairedKnives)
        modifier += 1;
    return modifier;
}

Wounds WitchAelves::computeReturnedDamage(const Weapon *weapon,
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