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

namespace DaughtersOfKhaine
{

static FactoryMethod factoryMethod = {
    WitchAelves::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = WitchAelves::MIN_UNIT_SIZE}, WitchAelves::MIN_UNIT_SIZE,
            WitchAelves::MAX_UNIT_SIZE, WitchAelves::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Paired Knives", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Hornblowers", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Standard Bearers", {.m_boolValue = true}, false, false},
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};

bool WitchAelves::s_registered = false;

WitchAelves::WitchAelves() :
    DaughterOfKhaine("Witch Aelves", 6, WOUNDS, 7, 6, false),
    m_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1),
    m_sacrificialKnifeHag(Weapon::Type::Melee, "Sacrificial Knife (Hag)", 1, 2, 2, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, WITCH_AELVES};
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
    {
        m_runAndCharge = true;
    }

    Model hag(BASESIZE, WOUNDS);
    hag.addMeleeWeapon(&m_sacrificialKnifeHag);
    addModel(hag);

    for (auto i = 1; i < numModels; i++)
    {
        Model witch(BASESIZE, WOUNDS);
        witch.addMeleeWeapon(&m_sacrificialKnife);
        addModel(witch);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *WitchAelves::Create(const ParameterList &parameters)
{
    auto unit = new WitchAelves();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool pairedKnives = GetBoolParam("Paired Knives", parameters, true);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
    bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);

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

int WitchAelves::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = DaughterOfKhaine::extraAttacks(nullptr, weapon, target);
    if (m_pairedKnives)
    { attacks += 1; }
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
    return DaughterOfKhaine::rollBattleshock();
}

int WitchAelves::toSaveModifier(const Weapon *weapon) const
{
    int modifier = DaughterOfKhaine::toSaveModifier(weapon);
    // Bladed Bucklers
    if (!m_pairedKnives)
    {
        modifier += 1;
    }
    return modifier;
}

Wounds WitchAelves::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
    // Bladed Bucklers
    if (!weapon->isMissile())
    {
        // 1 mortal wound for each save of a 6
        wounds += {0, 1};
    }
    return wounds;
}

void WitchAelves::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_sacrificialKnife);
    visitor(&m_sacrificialKnifeHag);
}

} // namespace DaughtersOfKhaine