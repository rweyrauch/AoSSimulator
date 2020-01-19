/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Warriors.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Warriors::Create,
    Warriors::ValueToString,
    Warriors::EnumStringToInt,
    Warriors::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Warriors::MIN_UNIT_SIZE, Warriors::MIN_UNIT_SIZE,
            Warriors::MAX_UNIT_SIZE, Warriors::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Warriors::DuardinAxeOrHammer, Warriors::DuardinAxeOrHammer,
            Warriors::DoubleHandedDuardinAxe, 1
        },
        {ParamType::Boolean, "Duardin Shields", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Hornblowers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Enum, "Grudge", Dispossessed::StuckUp, Dispossessed::StuckUp, Dispossessed::SneakyAmbushers, 1}
    },
    ORDER,
    { DISPOSSESSED }
};

bool Warriors::s_registered = false;

Warriors::Warriors() :
    Dispossessed("Warriors", 4, WOUNDS, 6, 5, false),
    m_duardinAxeOrHammer(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer", 1, 1, 3, 4, 0, 1),
    m_duardinAxeOrHammerVeteran(Weapon::Type::Melee, "Duardin Axe or Duardin Hammer", 1, 2, 3, 4, 0, 1),
    m_doubleHandedAxe(Weapon::Type::Melee, "Double-handed Duardin Axe", 1, 1, 4, 3, -1, 1),
    m_doubleHandedAxeVeteran(Weapon::Type::Melee, "Double-handed Duardin Axe", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, WARRIORS};
    m_weapons = {&m_duardinAxeOrHammer, &m_duardinAxeOrHammerVeteran, &m_doubleHandedAxe, &m_doubleHandedAxeVeteran};
}

bool Warriors::configure(int numModels, WeaponOptions weapons, bool duardinShields, StandardOptions standard, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standard = standard;
    m_hornblowers = hornblowers;
    m_duardinShields = duardinShields;

    auto veteran = new Model(BASESIZE, WOUNDS);
    if (weapons == DuardinAxeOrHammer)
    {
        veteran->addMeleeWeapon(&m_duardinAxeOrHammerVeteran);
    }
    else if (weapons == DoubleHandedDuardinAxe)
    {
        veteran->addMeleeWeapon(&m_doubleHandedAxeVeteran);
    }
    addModel(veteran);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == DuardinAxeOrHammer)
        {
            model->addMeleeWeapon(&m_duardinAxeOrHammer);
        }
        else if (weapons == DoubleHandedDuardinAxe)
        {
            model->addMeleeWeapon(&m_doubleHandedAxe);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Warriors::Create(const ParameterList &parameters)
{
    auto unit = new Warriors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetEnumParam("Weapons", parameters, DuardinAxeOrHammer);
    bool duardinShields = GetBoolParam("Duardin Shields", parameters, false);
    auto standard = (StandardOptions)GetEnumParam("Standard", parameters, None);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapons, duardinShields, standard, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Warriors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Warriors", factoryMethod);
    }
}

Rerolls Warriors::toSaveRerolls(const Weapon *weapon) const
{
    // Duardin Shields
    if (!m_ran && !m_charged)
    {
        if (!weapon->isMissile())
            return RerollFailed;
    }
    return Dispossessed::toSaveRerolls(weapon);
}

Rerolls Warriors::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Resolute in Defence
    if (m_opponentsCombat)
    {
        if (remainingModels() >= 20)
            return RerollFailed;
        else
            return RerollOnes;
    }
    return Dispossessed::toWoundRerolls(weapon, target);
}

void Warriors::onStartCombat(PlayerId player)
{
    Dispossessed::onStartCombat(player);

    m_opponentsCombat = (player != owningPlayer());
}

std::string Warriors::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == DuardinAxeOrHammer)
        {
            return "Duardin Axe Or Hammer";
        }
        else if (parameter.intValue == DoubleHandedDuardinAxe)
        {
            return "Double Handed Duardin Axe";
        }
    }
    else if (std::string(parameter.name) == "Standard")
    {
        if (parameter.intValue == None)
        {
            return "None";
        }
        else if (parameter.intValue == RunicIcon)
        {
            return "Runic Icon";
        }
        else if (parameter.intValue == ClanBanner)
        {
            return "Clan Banner";
        }
    }

    return Dispossessed::ValueToString(parameter);
}

int Warriors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Duardin Axe Or Hammer")
    {
        return DuardinAxeOrHammer;
    }
    else if (enumString == "Double Handed Duardin Axe")
    {
        return DoubleHandedDuardinAxe;
    }
    else if (enumString == "None")
    {
        return None;
    }
    else if (enumString == "Runic Icon")
    {
        return RunicIcon;
    }
    else if (enumString == "Clan Banner")
    {
        return ClanBanner;
    }
    return Dispossessed::EnumStringToInt(enumString);
}

int Warriors::rollRunDistance() const
{
    // Sound the Advance
    if (m_hornblowers)
    {
        return 4;
    }
    return Unit::rollRunDistance();
}

void Warriors::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

    if (m_standard == ClanBanner)
    {
        numFled = (numFled+1)/2;
    }
}

int Warriors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Dispossessed
