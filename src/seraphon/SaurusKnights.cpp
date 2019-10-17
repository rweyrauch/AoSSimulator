/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusKnights.h>
#include <UnitFactory.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusKnights::Create,
    SaurusKnights::ValueToString,
    SaurusKnights::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", SaurusKnights::MIN_UNIT_SIZE, SaurusKnights::MIN_UNIT_SIZE,
            SaurusKnights::MAX_UNIT_SIZE, SaurusKnights::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", SaurusKnights::CelestiteBlade, SaurusKnights::CelestiteBlade,
            SaurusKnights::CelestiteLance, 1
        },
        {ParamType::Boolean, "Stardrake Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Wardrum", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    ORDER,
    SERAPHON
};

bool SaurusKnights::s_registered = false;

SaurusKnights::SaurusKnights() :
    Unit("Saurus Knights", 7, WOUNDS, 10, 5, false),
    m_celestiteBlade(Weapon::Type::Melee, "Celestite Blade", 1, 1, 3, 3, 0, 1),
    m_celestiteBladeAlpha(Weapon::Type::Melee, "Celestite Blade", 1, 2, 3, 3, 0, 1),
    m_celestiteLance(Weapon::Type::Melee, "Celestite Lance", 1, 1, 4, 3, 0, 1),
    m_celestiteLanceAlpha(Weapon::Type::Melee, "Celestite Lance", 1, 2, 4, 3, 0, 1),
    m_jawsAndShield(Weapon::Type::Melee, "Powerful Jaws and Stardrake Shield", 1, 1, 5, 4, 0, 1),
    m_coldOneBite(Weapon::Type::Melee, "Cold One's Vicious Bite", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SAURUS, SAURUS_KNIGHTS};
}

bool SaurusKnights::configure(int numModels, SaurusKnights::WeaponOption weapons, bool iconBearer, bool wardrum)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_iconBearer = iconBearer;
    m_wardrum = wardrum;

    // Add the Alpha
    Model alpha(BASESIZE, WOUNDS);
    if (m_weaponOption == CelestiteBlade)
    {
        alpha.addMeleeWeapon(&m_celestiteBladeAlpha);
    }
    else if (m_weaponOption == CelestiteLance)
    {
        alpha.addMeleeWeapon(&m_celestiteLanceAlpha);
    }
    alpha.addMeleeWeapon(&m_jawsAndShield);
    alpha.addMeleeWeapon(&m_coldOneBite);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == CelestiteBlade)
        {
            model.addMeleeWeapon(&m_celestiteBlade);
        }
        else if (m_weaponOption == CelestiteLance)
        {
            model.addMeleeWeapon(&m_celestiteLance);
        }
        model.addMeleeWeapon(&m_jawsAndShield);
        model.addMeleeWeapon(&m_coldOneBite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SaurusKnights::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_celestiteBlade);
    visitor(m_celestiteBladeAlpha);
    visitor(m_celestiteLance);
    visitor(m_celestiteLanceAlpha);
    visitor(m_jawsAndShield);
    visitor(m_coldOneBite);
}

Unit *SaurusKnights::Create(const ParameterList &parameters)
{
    auto unit = new SaurusKnights();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, CelestiteBlade);
    bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
    bool wardrum = GetBoolParam("Wardrum", parameters, false);

    bool ok = unit->configure(numModels, weapons, iconBearer, wardrum);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SaurusKnights::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == CelestiteBlade) { return "Celestite Blade"; }
        else if (parameter.m_intValue == CelestiteLance) { return "Celestite Lance"; }
    }
    return ParameterValueToString(parameter);
}

int SaurusKnights::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Celestite Blade") { return CelestiteBlade; }
    else if (enumString == "Celestite Lance") { return CelestiteLance; }
    return 0;
}

void SaurusKnights::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Knights", factoryMethod);
    }
}

Wounds SaurusKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Blazing Lances
    if ((hitRoll >= 6) && m_charged && (weapon->name() == m_celestiteLance.name()))
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int SaurusKnights::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Stardrake Shields - ignore rend of -1 by cancelling it out.
    if (weapon->rend() == -1)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

} //namespace Seraphon
