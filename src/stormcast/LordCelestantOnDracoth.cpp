/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnDracoth.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCelestantOnDracoth::Create,
    LordCelestantOnDracoth::ValueToString,
    LordCelestantOnDracoth::EnumStringToInt,
    {
        {ParamType::Integer, "weapons", {.m_intValue = LordCelestantOnDracoth::TempestosHammer}, LordCelestantOnDracoth::TempestosHammer,
         LordCelestantOnDracoth::StormstrikeGlaive, 1},
        {ParamType::Boolean, "sigmariteThundershield", {.m_boolValue = false}, false, false},
    }
};

bool LordCelestantOnDracoth::s_registered = false;

Weapon LordCelestantOnDracoth::s_stormstrikeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 4, 3, 4, -1, 1);
Weapon LordCelestantOnDracoth::s_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2);
Weapon LordCelestantOnDracoth::s_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 3, 3, -1, 2);
Weapon LordCelestantOnDracoth::s_tempestosHammer(Weapon::Type::Melee, "Tempestos Hammer", 2, 3, 3, 2, -1, RAND_D3);
Weapon LordCelestantOnDracoth::s_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1);

LordCelestantOnDracoth::LordCelestantOnDracoth() :
    StormcastEternal("Lord-Celestant on Dracoth", 10, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, LORD_CELESTANT };
}

bool LordCelestantOnDracoth::configure(WeaponOption weapons, bool sigmariteThundershield)
{
    m_weapon = weapons;
    m_sigmariteThundershield = sigmariteThundershield;

    Model model(BASESIZE, WOUNDS);
    if (m_weapon == StormstrikeGlaive)
    {
        m_pStormstrikeGlaive = new Weapon(s_stormstrikeGlaive);
        model.addMeleeWeapon(m_pStormstrikeGlaive);
    }
    else if (m_weapon == LightningHammer)
        model.addMeleeWeapon(&s_lightningHammer);
    else if (m_weapon == Thunderaxe)
        model.addMeleeWeapon(&s_thunderaxe);
    else if (m_weapon == TempestosHammer)
        model.addMeleeWeapon(&s_tempestosHammer);

    model.addMeleeWeapon(&s_clawsAndFangs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordCelestantOnDracoth::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestantOnDracoth();
    auto weapons = (WeaponOption)GetIntParam("weapons", parameters, LightningHammer);
    bool sigmariteThundershield = GetBoolParam("sigmariteThundershield", parameters, false);

    bool ok = unit->configure(weapons, sigmariteThundershield);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordCelestantOnDracoth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Celestant-on-Dracoth", factoryMethod);
    }
}

std::string LordCelestantOnDracoth::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == TempestosHammer)
            return "TempestosHammer";
        else if (parameter.m_intValue == Thunderaxe)
            return "Thunderaxe";
        else if (parameter.m_intValue == LightningHammer)
            return "LightningHammer";
        else if (parameter.m_intValue == StormstrikeGlaive)
            return "StormstrikeGlaive";
    }
    return ParameterValueToString(parameter);
}

int LordCelestantOnDracoth::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);

    // Tempestos Hammer
    if (m_charged && weapon->name() == s_tempestosHammer.name())
    {
        Dice dice;
        attacks += dice.rollD3();
    }
    return attacks;
}

Rerolls LordCelestantOnDracoth::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Thundershield
    if (m_sigmariteThundershield)
        return RerollOnes;
    return StormcastEternal::toSaveRerolls(weapon);
}

Wounds LordCelestantOnDracoth::computeReturnedDamage(const Weapon *weapon,
                                                       const Dice::RollResult &saveRolls) const
{
    // Sigmarite Thundershield
    if (m_sigmariteThundershield)
    {
        // 1 mortal wound for each save of a 6
        Wounds returnedDamage = {0, saveRolls.numUnmodified6s()};
        return returnedDamage;
    }
    return StormcastEternal::computeReturnedDamage(weapon, saveRolls);
}


int LordCelestantOnDracoth::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    // Lightning Hammer
    if (weapon->name() == s_lightningHammer.name())
    {
        return hits.rolls.numUnmodified6s() * 2;
    }
    return StormcastEternal::generateMortalWounds(weapon, unit, hits, wounds);
}

void LordCelestantOnDracoth::onCharged()
{
    // TODO: replace this with damageModifier()
    if (m_pStormstrikeGlaive)
    {
        m_pStormstrikeGlaive->setDamage(s_stormstrikeGlaive.damage()+2);
    }
    StormcastEternal::onCharged();
}

void LordCelestantOnDracoth::onBeginTurn(int battleRound)
{
    if (m_pStormstrikeGlaive)
    {
        m_pStormstrikeGlaive->setDamage(s_stormstrikeGlaive.damage());
    }
    StormcastEternal::onBeginTurn(battleRound);
}

int LordCelestantOnDracoth::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "TempestosHammer")
        return TempestosHammer;
    else if (enumString == "Thunderaxe")
        return Thunderaxe;
    else if (enumString == "LightningHammer")
        return LightningHammer;
    else if (enumString == "StormstrikeGlaive")
        return StormstrikeGlaive;
    return 0;
}

void LordCelestantOnDracoth::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_stormstrikeGlaive);
    visitor(&s_lightningHammer);
    visitor(&s_thunderaxe);
    visitor(&s_tempestosHammer);
    visitor(&s_clawsAndFangs);
}

int LordCelestantOnDracoth::damageModifier(const Weapon *weapon, const Unit *target, const Dice::RollResult &woundRolls) const
{
    // Intolerable Damage
    if (weapon->name() == s_clawsAndFangs.name())
    {
        Dice dice;
        int modifier = 0;
        for (auto i = 0; i < woundRolls.numUnmodified6s(); i++)
        {
            modifier += dice.rollD3() - 1; // D3 instead of 1
        }
        return modifier;
    }
    return StormcastEternal::damageModifier(weapon, target, woundRolls);
}

} // namespace StormcastEternals