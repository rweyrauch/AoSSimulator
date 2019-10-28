/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/OgorGluttons.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    OgorGluttons::Create,
    OgorGluttons::ValueToString,
    OgorGluttons::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", OgorGluttons::MIN_UNIT_SIZE, OgorGluttons::MIN_UNIT_SIZE,
            OgorGluttons::MAX_UNIT_SIZE, OgorGluttons::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", OgorGluttons::ClubOrBladeAndIronfist, OgorGluttons::ClubOrBladeAndIronfist, OgorGluttons::PairedClubOrBlade, 1},
        {ParamType::Boolean, "Beast Skull Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Lookout Gnoblar", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bellower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    DESTRUCTION,
    OGOR_MAWTRIBES
};

bool OgorGluttons::s_registered = false;

Unit *OgorGluttons::Create(const ParameterList &parameters)
{
    auto unit = new OgorGluttons();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool skullBearer = GetBoolParam("Beast Skull Bearer", parameters, true);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, true);
    bool lookout = GetBoolParam("Lookout Gnoblar", parameters, true);
    bool bellower = GetBoolParam("Bellower", parameters, true);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, ClubOrBladeAndIronfist);

    bool ok = unit->configure(numModels, weapons, skullBearer, bannerBearer, lookout, bellower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string OgorGluttons::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == ClubOrBladeAndIronfist) return "Club or Blade and Ironfist";
        else if (parameter.m_intValue == PairedClubOrBlade) return "Paired Club or Blade";
    }
    return MawtribesBase::ValueToString(parameter);
}

int OgorGluttons::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Club or Blade and Ironfist") return ClubOrBladeAndIronfist;
    else if (enumString == "Paired Club or Blade") return PairedClubOrBlade;

    return MawtribesBase::EnumStringToInt(enumString);
}

void OgorGluttons::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ogor Gluttons", factoryMethod);
    }
}

OgorGluttons::OgorGluttons() :
    MawtribesBase("Ogor Gluttons", 6, WOUNDS, 6, 5, false),
    m_clubOrBlade(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 3, 3, 3, 0, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
    m_clubOrBladeCrusher(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 4, 3, 3, 0, 2)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, OGOR_GLUTTONS};
}

bool OgorGluttons::configure(int numModels, WeaponOption option, bool skullBearer, bool bannerBearer, bool lookoutGnoblar, bool bellower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = option;
    m_skullBearer = skullBearer;
    m_bannerBearer = bannerBearer;
    m_lookoutGnoblar = lookoutGnoblar;
    m_bellower = bellower;

    Model crusher(BASESIZE, WOUNDS);
    crusher.addMeleeWeapon(&m_clubOrBladeCrusher);
    crusher.addMeleeWeapon(&m_bite);
    addModel(crusher);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_clubOrBlade);
        model.addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void OgorGluttons::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_clubOrBlade);
    visitor(m_bite);
    visitor(m_clubOrBladeCrusher);
}

int OgorGluttons::braveryModifier() const
{
    auto mod = Unit::braveryModifier();

    // Tribal Banner Bearer
    if (m_bannerBearer) mod++;

    return mod;
}

Rerolls OgorGluttons::chargeRerolls() const
{
    // Beast Skull Bearer
    if (m_skullBearer)
    {
        return RerollFailed;
    }
    return Unit::chargeRerolls();
}

int OgorGluttons::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Paired Clubs or Blades
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_clubOrBlade.name()) && (m_weaponOption == PairedClubOrBlade))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

Wounds OgorGluttons::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    // Ironfist
    if ((saveRoll == 6) && (m_weaponOption == ClubOrBladeAndIronfist))
    {
        return {0, 1};
    }
    return Unit::computeReturnedDamage(weapon, saveRoll);
}

} // namespace OgorMawtribes
