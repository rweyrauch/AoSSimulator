/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BlackKnights.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    BlackKnights::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = BlackKnights::MIN_UNIT_SIZE}, BlackKnights::MIN_UNIT_SIZE, BlackKnights::MAX_UNIT_SIZE, BlackKnights::MIN_UNIT_SIZE},
        {ParamType::Boolean, "standardBearers", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false, false},
    }
};

Weapon BlackKnights::s_barrowLance(Weapon::Type::Melee, "Barrow Lance", 1, 2, 3, 4, 0, 1);
Weapon BlackKnights::s_barrowLanceKnight(Weapon::Type::Melee, "Barrow Lance (Hell Knight", 1, 3, 3, 4, 0, 1);
Weapon BlackKnights::s_hoovesAndTeeth(Weapon::Type::Melee, "Skeletal Steed's Hooves and Teeth", 1, 2, 4, 5, 0, 1);

bool BlackKnights::s_registered = false;

BlackKnights::BlackKnights() :
    Unit("Black Knights", 12, WOUNDS, 10, 5, false)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, BLACK_KNIGHTS};
}

bool BlackKnights::configure(int numModels, bool standardBearers, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearers = standardBearers;
    m_hornblowers = hornblowers;

    Model hellKnight(BASESIZE, WOUNDS);
    hellKnight.addMeleeWeapon(&s_barrowLanceKnight);
    hellKnight.addMeleeWeapon(&s_hoovesAndTeeth);
    addModel(hellKnight);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_barrowLance);
        model.addMeleeWeapon(&s_hoovesAndTeeth);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void BlackKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_barrowLance);
    visitor(&s_barrowLanceKnight);
    visitor(&s_hoovesAndTeeth);
}

Unit *BlackKnights::Create(const ParameterList &parameters)
{
    auto unit = new BlackKnights();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

    bool ok = unit->configure(numModels, standardBearers, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BlackKnights::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Black Knights", factoryMethod);
    }
}

int BlackKnights::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toWoundModifier(weapon, target);

    // Deathly Charge
    if (m_charged && weapon->name() == s_barrowLance.name())
    {
        modifier += 1;
    }
    return modifier;
}

int BlackKnights::damageModifier(const Weapon *weapon, const Unit *target, const Dice::RollResult& woundRolls) const
{
    int modifier = Unit::damageModifier(weapon, target, woundRolls);

    // Deathly Charge
    if (m_charged && weapon->name() == s_barrowLance.name())
    {
        modifier += 1;
    }
    return modifier;
}

int BlackKnights::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Crypt Shields
    if (weapon->rend() == 0)
        modifier += 1;

    return modifier;
}

} //namespace Death
