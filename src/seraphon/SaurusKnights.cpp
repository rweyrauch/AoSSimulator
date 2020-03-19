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

bool SaurusKnights::s_registered = false;

SaurusKnights::SaurusKnights() :
    SeraphonBase("Saurus Knights", 8, WOUNDS, 8, 4, false),
    m_celestiteBlade(Weapon::Type::Melee, "Celestite Blade", 1, 2, 3, 3, 0, 1),
    m_celestiteBladeAlpha(Weapon::Type::Melee, "Celestite Blade", 1, 3, 3, 3, 0, 1),
    m_celestiteSpear(Weapon::Type::Melee, "Celestite Warspear", 1, 2, 4, 3, 0, 1),
    m_celestiteSpearAlpha(Weapon::Type::Melee, "Celestite Warspear", 1, 3, 4, 3, 0, 1),
    m_jaws(Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1),
    m_coldOneJaws(Weapon::Type::Melee, "Snapping Jaws", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, COLD_ONE, SAURUS_KNIGHTS};
    m_weapons = {&m_celestiteBlade, &m_celestiteBladeAlpha, &m_celestiteSpear, &m_celestiteSpearAlpha, &m_jaws, &m_coldOneJaws};

    s_globalBraveryMod.connect(this, &SaurusKnights::stardrakeIcon, &m_connection);
}

SaurusKnights::~SaurusKnights()
{
    m_connection.disconnect();
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
    auto alpha = new Model(BASESIZE, WOUNDS);
    if (m_weaponOption == CelestiteBlade)
    {
        alpha->addMeleeWeapon(&m_celestiteBladeAlpha);
    }
    else if (m_weaponOption == CelestiteWarspear)
    {
        alpha->addMeleeWeapon(&m_celestiteSpearAlpha);
    }
    alpha->addMeleeWeapon(&m_jaws);
    alpha->addMeleeWeapon(&m_coldOneJaws);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == CelestiteBlade)
        {
            model->addMeleeWeapon(&m_celestiteBlade);
        }
        else if (m_weaponOption == CelestiteWarspear)
        {
            model->addMeleeWeapon(&m_celestiteSpear);
        }
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_coldOneJaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *SaurusKnights::Create(const ParameterList &parameters)
{
    auto unit = new SaurusKnights();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, CelestiteBlade);
    bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
    bool wardrum = GetBoolParam("Wardrum", parameters, false);

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

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
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == CelestiteBlade) { return "Celestite Blade"; }
        else if (parameter.intValue == CelestiteWarspear) { return "Celestite Warspear"; }
    }
    return SeraphonBase::ValueToString(parameter);
}

int SaurusKnights::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Celestite Blade") { return CelestiteBlade; }
    else if (enumString == "Celestite Warspear") { return CelestiteWarspear; }
    return SeraphonBase::EnumStringToInt(enumString);
}

void SaurusKnights::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Weapons", CelestiteBlade, CelestiteBlade, CelestiteWarspear, 1},
                {ParamType::Boolean, "Stardrake Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Wardrum", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Saurus Knights", factoryMethod);
    }
}

Wounds SaurusKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Celestite Warspear
    if ((hitRoll >= 6) && m_charged && (weapon->name() == m_celestiteSpear.name()))
    {
        return {weapon->damage(), 1};
    }
    return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int SaurusKnights::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

Rerolls SaurusKnights::chargeRerolls() const
{
    if (m_wardrum) return RerollFailed;

    return SeraphonBase::chargeRerolls();
}

int SaurusKnights::stardrakeIcon(const Unit *target)
{
    // Icon Bearer
    if (m_iconBearer && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0f))
    {
        return -1;
    }

    return 0;
}

} //namespace Seraphon
