/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Skinks.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Skinks::Create,
    Skinks::ValueToString,
    Skinks::EnumStringToInt,
    Skinks::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Skinks::MIN_UNIT_SIZE, Skinks::MIN_UNIT_SIZE,
            Skinks::MAX_UNIT_SIZE, Skinks::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Skinks::BoltspittersAndBucklers, Skinks::JavelinsAndBucklers,
            Skinks::ClubsAndBucklers, 1
        },
    },
    ORDER,
    { SERAPHON }
};

bool Skinks::s_registered = false;

Skinks::Skinks() :
    Unit("Skinks", 8, WOUNDS, 10, 6, false),
    m_javelinMissile(Weapon::Type::Missile, "Meteoric Javelin", 8, 1, 5, 4, 0, 1),
    m_boltspitterMissile(Weapon::Type::Missile, "Boltspitter", 16, 1, 5, 5, 0, 1),
    m_javelin(Weapon::Type::Melee, "Meteoric Javelin", 1, 1, 6, 5, 0, 1),
    m_javelinAlpha(Weapon::Type::Melee, "Meteoric Javelin", 1, 2, 6, 5, 0, 1),
    m_boltspitter(Weapon::Type::Melee, "Boltspitter", 1, 1, 5, 6, 0, 1),
    m_boltspitterAlpha(Weapon::Type::Melee, "Boltspitter", 1, 2, 5, 6, 0, 1),
    m_club(Weapon::Type::Melee, "Moonstone Club", 1, 1, 5, 4, 0, 1),
    m_clubAlpha(Weapon::Type::Melee, "Moonstone Club", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINKS};
    m_weapons = {&m_javelinMissile, &m_boltspitterMissile, &m_javelin, &m_javelinAlpha, &m_boltspitter, &m_boltspitterAlpha, &m_club, &m_clubAlpha};
}

bool Skinks::configure(int numModels, WeaponOption weapons)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    
    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    switch (m_weaponOption)
    {
        case JavelinsAndBucklers:
            alpha->addMissileWeapon(&m_javelinMissile);
            alpha->addMeleeWeapon(&m_javelinAlpha);
            break;
        case BoltspittersAndBucklers:
            alpha->addMissileWeapon(&m_boltspitterMissile);
            alpha->addMeleeWeapon(&m_boltspitterAlpha);
            break;
        case BoltspittersAndClubs:
            alpha->addMissileWeapon(&m_boltspitterMissile);
            alpha->addMeleeWeapon(&m_clubAlpha);
            alpha->addMeleeWeapon(&m_boltspitterAlpha);
            break;
        case ClubsAndBucklers:
            alpha->addMeleeWeapon(&m_clubAlpha);
            break;
    }
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        switch (m_weaponOption)
        {
            case JavelinsAndBucklers:
                model->addMissileWeapon(&m_javelinMissile);
                model->addMeleeWeapon(&m_javelin);
                break;
            case BoltspittersAndBucklers:
                model->addMissileWeapon(&m_boltspitterMissile);
                model->addMeleeWeapon(&m_boltspitter);
                break;
            case BoltspittersAndClubs:
                model->addMissileWeapon(&m_boltspitterMissile);
                model->addMeleeWeapon(&m_club);
                model->addMeleeWeapon(&m_boltspitter);
                break;
            case ClubsAndBucklers:
                model->addMeleeWeapon(&m_club);
                break;
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Skinks::Create(const ParameterList &parameters)
{
    auto unit = new Skinks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, BoltspittersAndBucklers);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skinks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skinks", factoryMethod);
    }
}

std::string Skinks::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == JavelinsAndBucklers) { return "Javelins and Bucklers"; }
        else if (parameter.intValue == BoltspittersAndClubs) { return "Boltspitters and Clubs"; }
        else if (parameter.intValue == BoltspittersAndBucklers) { return "Boltspitters and Bucklers"; }
        else if (parameter.intValue == ClubsAndBucklers) { return "ClubsAndBucklers"; }
    }
    return ParameterValueToString(parameter);
}

int Skinks::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Javelins and Bucklers") { return JavelinsAndBucklers; }
    else if (enumString == "Boltspitters and Clubs") { return BoltspittersAndClubs; }
    else if (enumString == "Boltspitters and Bucklers") { return BoltspittersAndBucklers; }
    else if (enumString == "Clubs and Bucklers") { return ClubsAndBucklers; }
    return 0;
}

int Skinks::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    if (m_weaponOption != BoltspittersAndBucklers)
    {
        // Star-bucklers - ignore rend of -1 by cancelling it out.
        if (weapon->rend() == -1)
        {
            modifier = -weapon->rend();
        }
    }

    return modifier;
}

int Skinks::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Celestial Cohort
    if (weapon->isMissile())
    {
        if (remainingModels() >= 30)
            modifier += 2;
        else if (remainingModels() >= 20)
            modifier += 1;
    }
    return modifier;
}

int Skinks::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Seraphon
