/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanumOnDracoline.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordArcanumOnDracoline::Create,
    LordArcanumOnDracoline::ValueToString,
    LordArcanumOnDracoline::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordArcanumOnDracoline::s_registered = false;

LordArcanumOnDracoline::LordArcanumOnDracoline() :
    StormcastEternal("Lord-Arcanum on Celestial Dracoline", 12, WOUNDS, 9, 3, false),
    m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
    m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, LORD_ARCANUM};
}

bool LordArcanumOnDracoline::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_aetherstave);
    model.addMeleeWeapon(&m_monstrousClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateStormLance(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordArcanumOnDracoline::Create(const ParameterList &parameters)
{
    auto unit = new LordArcanumOnDracoline();
    auto storm = (LoreOfTheStorm)GetEnumParam("Lore of the Storm", parameters, (int)LoreOfTheStorm::None);
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    bool ok = unit->configure(storm, invigoration);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordArcanumOnDracoline::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Arcanum on Celestial Dracoline", factoryMethod);
    }
}

std::string LordArcanumOnDracoline::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of the Storm")
    {
        return ToString((LoreOfTheStorm) parameter.m_intValue);
    }
    else if (parameter.m_name == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.m_intValue);
    }
    return ParameterValueToString(parameter);
}

int LordArcanumOnDracoline::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheStorm storm;
    LoreOfInvigoration invigoration;
    if (FromString(enumString, storm))
    {
        return (int)storm;
    }
    else if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return 0;
}

void LordArcanumOnDracoline::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_aetherstave);
    visitor(&m_monstrousClaws);
}

Wounds LordArcanumOnDracoline::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Thunderous Pounce
    if (m_charged && weapon->name() == m_monstrousClaws.name())
    {
        Dice dice;
        return {dice.rollD3(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls LordArcanumOnDracoline::chargeRerolls() const
{
    // Thunderous Pounce
    return RerollFailed;
}

} // namespace StormcastEternals