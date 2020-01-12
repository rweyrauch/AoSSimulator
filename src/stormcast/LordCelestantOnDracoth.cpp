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
        {
            ParamType::Enum, "Weapon", LordCelestantOnDracoth::TempestosHammer, LordCelestantOnDracoth::TempestosHammer, LordCelestantOnDracoth::StormstrikeGlaive, 1
        },
        {ParamType::Boolean, "Sigmarite Thundershield", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordCelestantOnDracoth::s_registered = false;

LordCelestantOnDracoth::LordCelestantOnDracoth() :
    StormcastEternal("Lord-Celestant on Dracoth", 10, WOUNDS, 9, 3, false),
    m_stormstrikeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 4, 3, 4, -1, 1),
    m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
    m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 3, 3, -1, 2),
    m_tempestosHammer(Weapon::Type::Melee, "Tempestos Hammer", 2, 3, 3, 2, -1, RAND_D3),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, LORD_CELESTANT};
    m_weapons = {&m_stormstrikeGlaive, &m_lightningHammer, &m_thunderaxe, &m_tempestosHammer, &m_clawsAndFangs};
}

bool LordCelestantOnDracoth::configure(WeaponOption weapons, bool sigmariteThundershield)
{
    m_weapon = weapons;
    m_sigmariteThundershield = sigmariteThundershield;

    auto model = new Model(BASESIZE, WOUNDS);
    if (m_weapon == StormstrikeGlaive)
    {
        model->addMeleeWeapon(&m_stormstrikeGlaive);
    }
    else if (m_weapon == LightningHammer)
    {
        model->addMeleeWeapon(&m_lightningHammer);
    }
    else if (m_weapon == Thunderaxe)
    {
        model->addMeleeWeapon(&m_thunderaxe);
    }
    else if (m_weapon == TempestosHammer)
    {
        model->addMeleeWeapon(&m_tempestosHammer);
    }

    model->addMeleeWeapon(&m_clawsAndFangs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordCelestantOnDracoth::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestantOnDracoth();
    auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, LightningHammer);
    bool sigmariteThundershield = GetBoolParam("Sigmarite Thundershield", parameters, false);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

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
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == TempestosHammer)
        {
            return "Tempestos Hammer";
        }
        else if (parameter.intValue == Thunderaxe)
        {
            return "Thunderaxe";
        }
        else if (parameter.intValue == LightningHammer)
        {
            return "Lightning Hammer";
        }
        else if (parameter.intValue == StormstrikeGlaive)
        {
            return "Stormstrike Glaive";
        }
    }
    return StormcastEternal::ValueToString(parameter);
}

int LordCelestantOnDracoth::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);

    // Tempestos Hammer
    if (m_charged && weapon->name() == m_tempestosHammer.name())
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
    {
        return RerollOnes;
    }
    return StormcastEternal::toSaveRerolls(weapon);
}

Wounds LordCelestantOnDracoth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto wounds = StormcastEternal::computeReturnedDamage(weapon, saveRoll);
    // Sigmarite Thundershield
    if (m_sigmariteThundershield)
    {
        // 1 mortal wound for each save of a 6
        if (saveRoll == 6)
        {
            wounds += {0, 1};
        }
    }
    return wounds;
}

int LordCelestantOnDracoth::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Tempestos Hammer")
    {
        return TempestosHammer;
    }
    else if (enumString == "Thunderaxe")
    {
        return Thunderaxe;
    }
    else if (enumString == "Lightning Hammer")
    {
        return LightningHammer;
    }
    else if (enumString == "Stormstrike Glaive")
    {
        return StormstrikeGlaive;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

Wounds LordCelestantOnDracoth::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Intolerable Damage
    if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name()))
    {
        Dice dice;
        // D6 instead of 1
        return { dice.rollD6(), 0 };
    }

    // Lightning Hammer
    if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name()))
    {
        return {weapon->damage(), 2};
    }

    if (m_charged && (m_weapon == StormstrikeGlaive))
    {
        return { weapon->damage() + 2, 0};
    }

    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace StormcastEternals