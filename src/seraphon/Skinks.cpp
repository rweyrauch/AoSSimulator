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
            ParamType::Enum, "Weapons", Skinks::BoltspittersDaggersAndBucklers, Skinks::JavelinsDaggersAndBucklers,
            Skinks::ClubsAndBucklers, 1
        },
    },
    ORDER,
    { SERAPHON }
};

bool Skinks::s_registered = false;

Skinks::Skinks() :
    SeraphonBase("Skinks", 8, WOUNDS, 5, 6, false),
    m_javelin(Weapon::Type::Missile, "Meteoric Javelin", 8, 1, 5, 4, 0, 1),
    m_boltspitter(Weapon::Type::Missile, "Boltspitter", 16, 1, 5, 5, 0, 1),
    m_dagger(Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1),
    m_daggerAlpha(Weapon::Type::Melee, "Celestite Dagger", 1, 2, 5, 5, 0, 1),
    m_club(Weapon::Type::Melee, "Moonstone Club", 1, 1, 4, 4, 0, 1),
    m_clubAlpha(Weapon::Type::Melee, "Moonstone Club", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINKS};
    m_weapons = {&m_javelin, &m_boltspitter, &m_dagger, &m_daggerAlpha, &m_club, &m_clubAlpha};
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
        case JavelinsDaggersAndBucklers:
            alpha->addMissileWeapon(&m_javelin);
            alpha->addMeleeWeapon(&m_daggerAlpha);
            break;
        case BoltspittersDaggersAndBucklers:
            alpha->addMissileWeapon(&m_boltspitter);
            alpha->addMeleeWeapon(&m_daggerAlpha);
            break;
        case BoltspittersAndClubs:
            alpha->addMissileWeapon(&m_boltspitter);
            alpha->addMeleeWeapon(&m_clubAlpha);
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
            case JavelinsDaggersAndBucklers:
                model->addMissileWeapon(&m_javelin);
                model->addMeleeWeapon(&m_dagger);
                break;
            case BoltspittersDaggersAndBucklers:
                model->addMissileWeapon(&m_boltspitter);
                model->addMeleeWeapon(&m_dagger);
                break;
            case BoltspittersAndClubs:
                model->addMissileWeapon(&m_boltspitter);
                model->addMeleeWeapon(&m_club);
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
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, BoltspittersDaggersAndBucklers);

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
        if (parameter.intValue == JavelinsDaggersAndBucklers) { return "Javelins, Daggers and Bucklers"; }
        else if (parameter.intValue == BoltspittersAndClubs) { return "Boltspitters and Clubs"; }
        else if (parameter.intValue == BoltspittersDaggersAndBucklers) { return "Boltspitters, Daggers and Bucklers"; }
        else if (parameter.intValue == ClubsAndBucklers) { return "Moonstone Clubs and Bucklers"; }
    }
    return SeraphonBase::ValueToString(parameter);
}

int Skinks::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Javelins, Daggers and Bucklers") { return JavelinsDaggersAndBucklers; }
    else if (enumString == "Boltspitters and Clubs") { return BoltspittersAndClubs; }
    else if (enumString == "Boltspitters, Daggers and Bucklers") { return BoltspittersDaggersAndBucklers; }
    else if (enumString == "Moonstone Clubs and Bucklers") { return ClubsAndBucklers; }
    return SeraphonBase::EnumStringToInt(enumString);
}

int Skinks::toSaveModifier(const Weapon *weapon) const
{
    int modifier = SeraphonBase::toSaveModifier(weapon);

    if (m_weaponOption != BoltspittersAndClubs)
    {
        // Star-bucklers
        modifier++;
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

int Skinks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);

    // Swarming Cohort
    if (remainingModels() >= 15) extra++;

    return extra;
}

} //namespace Seraphon
