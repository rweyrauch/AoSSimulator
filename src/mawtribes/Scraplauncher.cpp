/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Scraplauncher.h"

namespace OgorMawtribes
{
static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 9;
static const int POINTS_PER_UNIT = 120;

bool GnoblarScraplauncher::s_registered = false;

Unit *GnoblarScraplauncher::Create(const ParameterList &parameters)
{
    auto unit = new GnoblarScraplauncher();

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string GnoblarScraplauncher::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int GnoblarScraplauncher::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

void GnoblarScraplauncher::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            GnoblarScraplauncher::Create,
            GnoblarScraplauncher::ValueToString,
            GnoblarScraplauncher::EnumStringToInt,
            GnoblarScraplauncher::ComputePoints,
            {
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Gnoblar Scraplauncher", factoryMethod);
    }
}

GnoblarScraplauncher::GnoblarScraplauncher() :
    MawtribesBase("Gnoblar Scraplauncher", 7, WOUNDS, 5, 4, false),
    m_scrap(Weapon::Type::Missile, "Piles of Old Scrap", 36, 3, 3, 4, 0, RAND_D3),
    m_scrapperWeapons(Weapon::Type::Melee, "Gnoblar Scrappers' Weapons",1, 7, 5, 5, 0, 1),
    m_horns(Weapon::Type::Melee, "Rhinox's Sharp Horns", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, RHINOX, OGOR_MAWTRIBES, GUTBUSTERS, GNOBLAR_SCRAPLAUNCHER};
    m_weapons = {&m_scrap, &m_scrapperWeapons, &m_horns};
}

bool GnoblarScraplauncher::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMissileWeapon(&m_scrap);
    model->addMeleeWeapon(&m_scrapperWeapons);
    model->addMeleeWeapon(&m_horns);

    addModel(model);

    m_points = GnoblarScraplauncher::ComputePoints(1);

    return true;
}

int GnoblarScraplauncher::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Deadly Rain of Scrap
    if ((weapon->name() == m_scrap.name()) && (target->remainingModels() >= 10))
    {
        mod++;
    }

    return mod;
}

Wounds GnoblarScraplauncher::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deadly Rain of Scrap
    if ((weapon->name() == m_scrap.name()) && (target->remainingModels() >= 10))
    {
        return {RAND_D6, 0};
    }
    // Rhinox Charge
    else if ((weapon->name() == m_horns.name()) && m_charged)
    {
        return {weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int GnoblarScraplauncher::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OgorMawtribes
