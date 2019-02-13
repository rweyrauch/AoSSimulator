/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Quarrellers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Quarrellers::Create,
    Quarrellers::ValueToString,
    Quarrellers::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Quarrellers::MIN_UNIT_SIZE}, Quarrellers::MIN_UNIT_SIZE,
            Quarrellers::MAX_UNIT_SIZE, Quarrellers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "duardinBucklers", {.m_boolValue = false}, false, false, false},
        {ParamType::Enum, "standard", {.m_intValue = Quarrellers::None}, Quarrellers::None, Quarrellers::ClanBanner, 1},
        {ParamType::Boolean, "drummer", {.m_boolValue = false}, false, false, false}
    },
    ORDER,
    DISPOSSESSED
};

bool Quarrellers::s_registered = false;

Quarrellers::Quarrellers() :
    Unit("Quarrellers", 4, WOUNDS, 6, 5, false),
    m_duardinCrossbow(Weapon::Type::Missile, "Duardin Crossbow", 20, 1, 4, 4, 0, 1),
    m_duardinCrossbowVeteran(Weapon::Type::Missile, "Duardin Crossbow (Veteran)", 20, 1, 3, 4, 0, 1),
    m_rangersAxe(Weapon::Type::Melee, "Ranger's Axe", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, QUARRELLERS};
}

bool Quarrellers::configure(int numModels, bool duardinBucklers, StandardOptions standard, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standard = standard;
    m_drummer = drummer;
    m_duardinBucklers = duardinBucklers;

    Model veteran(BASESIZE, WOUNDS);
    veteran.addMissileWeapon(&m_duardinCrossbowVeteran);
    veteran.addMeleeWeapon(&m_rangersAxe);
    addModel(veteran);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_duardinCrossbow);
        model.addMeleeWeapon(&m_rangersAxe);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Quarrellers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_duardinCrossbow);
    visitor(&m_duardinCrossbowVeteran);
    visitor(&m_rangersAxe);
}

Unit *Quarrellers::Create(const ParameterList &parameters)
{
    auto unit = new Quarrellers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool duardinBucklers = GetBoolParam("duardinBucklers", parameters, false);
    auto standard = (StandardOptions)GetEnumParam("standard", parameters, None);
    bool drummer = GetBoolParam("drummer", parameters, false);

    bool ok = unit->configure(numModels, duardinBucklers, standard, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Quarrellers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Quarrellers", factoryMethod);
    }
}

int Quarrellers::extraAttacks(const Weapon *weapon) const
{
    if (weapon->name() == m_duardinCrossbow.name() && remainingModels() >= 20)
    {
        // Volley Fire
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(m_owningPlayer));
        if (unit && distanceTo(unit) > 3.0f)
        {
            return 1;
        }
    }
    return Unit::extraAttacks(weapon);
}

Rerolls Quarrellers::toSaveRerolls(const Weapon *weapon) const
{
    // Duardin Bucklers
    if (!m_ran && !m_charged)
    {
        if (!weapon->isMissile())
            return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

std::string Quarrellers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "standard")
    {
        if (parameter.m_intValue == None)
        {
            return "None";
        }
        else if (parameter.m_intValue == RunicIcon)
        {
            return "RunicIcon";
        }
        else if (parameter.m_intValue == ClanBanner)
        {
            return "ClanBanner";
        }
    }

    return ParameterValueToString(parameter);
}

int Quarrellers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "None")
    {
        return None;
    }
    else if (enumString == "RunicIcon")
    {
        return RunicIcon;
    }
    else if (enumString == "ClanBanner")
    {
        return ClanBanner;
    }
    return 0;
}

} // namespace Dispossessed
