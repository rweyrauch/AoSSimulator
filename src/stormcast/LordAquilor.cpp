/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordAquilor.h>
#include <iostream>

#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordAquilor::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Boolean, "Astral Compass", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordAquilor::s_registered = false;

LordAquilor::LordAquilor() :
    StormcastEternal("Lord-Aquilor", 12, WOUNDS, 9, 3, false),
    m_boltstormPistol(Weapon::Type::Missile, "Heavy Boltstorm Pistol", 9, 4, 3, 3, 0, 1),
    m_starboundBlade(Weapon::Type::Melee, "Starbound Blade", 1, 3, 3, 3, -1, 2),
    m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 3, 3, 0, 1),
    m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, HERO, LORD_AQUILOR};
}

bool LordAquilor::configure(bool astralCompass)
{
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_boltstormPistol);
    model.addMeleeWeapon(&m_starboundBlade);
    model.addMeleeWeapon(&m_shockHandaxe);
    model.addMeleeWeapon(&m_beakAndClaws);
    addModel(model);

    m_astralCompass = astralCompass;

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordAquilor::Create(const ParameterList &parameters)
{
    auto unit = new LordAquilor();
    bool astralCompass = GetBoolParam("Astral Compass", parameters, false);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(astralCompass);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordAquilor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Aquilor", factoryMethod);
    }
}

void LordAquilor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boltstormPistol);
    visitor(&m_starboundBlade);
    visitor(&m_shockHandaxe);
    visitor(&m_beakAndClaws);
}

Wounds LordAquilor::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Aethereal Strike
    if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name()))
    {
        return {0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace StormcastEternals