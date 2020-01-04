/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "mawtribes/Tyrant.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Tyrant::Create,
    Tyrant::ValueToString,
    Tyrant::EnumStringToInt,
    {
        {ParamType::Enum, "Big Name", Tyrant::Fateseeker, Tyrant::Deathcheater, Tyrant::Wallcrusher, 1},
    },
    DESTRUCTION,
    OGOR_MAWTRIBES
};

bool Tyrant::s_registered = false;

Unit *Tyrant::Create(const ParameterList &parameters)
{
    auto unit = new Tyrant();

    auto bigName = (BigName)GetEnumParam("Big Name", parameters, Fateseeker);

    bool ok = unit->configure(bigName);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Tyrant::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Big Name")
    {
        if (parameter.m_intValue == Deathcheater) return "Deathcheater";
        else if (parameter.m_intValue == Brawlerguts) return "Brawlerguts";
        else if (parameter.m_intValue == Fateseeker) return "Fateseeker";
        else if (parameter.m_intValue == Longstrider) return "Longstrider";
        else if (parameter.m_intValue == Giantbreaker) return "Giantbreaker";
        else if (parameter.m_intValue == Wallcrusher) return "Wallcrusher";
    }
    return MawtribesBase::ValueToString(parameter);
}

int Tyrant::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Deathcheater") return Deathcheater;
    else if (enumString == "Brawlerguts") return Brawlerguts;
    else if (enumString == "Fateseeker") return Fateseeker;
    else if (enumString == "Longstrider") return Longstrider;
    else if (enumString == "Giantbreaker") return Giantbreaker;
    else if (enumString == "Wallcrusher") return Wallcrusher;

    return MawtribesBase::EnumStringToInt(enumString);
}

void Tyrant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tyrant", factoryMethod);
    }
}

Tyrant::Tyrant() :
    MawtribesBase("Tyrant", 6, WOUNDS, 8, 4, false),
    m_pistols(Weapon::Type::Missile, "Ogor Pistols", 12, 2, 4, 3, -1, RAND_D3),
    m_thundermace(Weapon::Type::Melee, "Thundermace", 1, 3, 3, 3, -2, 3),
    m_glaive(Weapon::Type::Melee, "Beastskewer Glaive", 3, 2, 3, 3, -1, RAND_D3),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, TYRANT};
}

bool Tyrant::configure(BigName bigName)
{
    if (bigName == Deathcheater) m_wounds = WOUNDS + 1;

    auto model = new Model(BASESIZE, wounds());

    model->addMissileWeapon(&m_pistols);
    model->addMeleeWeapon(&m_thundermace);
    model->addMeleeWeapon(&m_glaive);
    model->addMeleeWeapon(&m_bite);

    addModel(model);

    if (bigName == Fateseeker) m_save = 3;
    else if (bigName == Longstrider) m_move = 8;

    m_bigName = bigName;
    m_points = POINTS_PER_UNIT;

    return true;
}

void Tyrant::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_pistols);
    visitor(m_thundermace);
    visitor(m_glaive);
    visitor(m_bite);
}

Wounds Tyrant::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if ((hitRoll == 6) && (weapon->name() == m_glaive.name()))
    {
        if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))
        {
            Dice dice;
            return {dice.rollD6(), 0};
        }
    }
    if ((hitRoll == 6) && (weapon->name() == m_thundermace.name()))
    {
        if (target->remainingModels() > 3)
        {
            Dice dice;
            return {weapon->damage(), dice.rollD3()};
        }
        else
        {
            return { weapon->damage(), 1};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int Tyrant::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toWoundModifier(weapon, target);

    if (m_charged && (m_bigName == Brawlerguts)) mod++;

    return mod;
}

} // namespace OgorMawtribes
