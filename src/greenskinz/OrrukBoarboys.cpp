/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukBoarboys.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz
{
static FactoryMethod factoryMethod = {
    OrrukBoarboys::Create,
    OrrukBoarboys::ValueToString,
    OrrukBoarboys::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", OrrukBoarboys::MIN_UNIT_SIZE,
            OrrukBoarboys::MIN_UNIT_SIZE, OrrukBoarboys::MAX_UNIT_SIZE, OrrukBoarboys::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", OrrukBoarboys::Choppa, OrrukBoarboys::Choppa, OrrukBoarboys::PigstikkaSpear, 1},
        {ParamType::Boolean, "Glyph Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Waaagh! Horns", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    DESTRUCTION,
    GREENSKINZ
};

bool OrrukBoarboys::s_registered = false;

OrrukBoarboys::OrrukBoarboys() :
    Unit("Orruk Boarboys", 9, WOUNDS, 5, 5, false),
    m_choppa(Weapon::Type::Melee, "Choppa", 1, 1, 4, 4, -1, 1),
    m_pigstikkaSpear(Weapon::Type::Melee, "Pigstikka Spear", 2, 1, 4, 4, 0, 1),
    m_warBoarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1),
    m_choppaBoss(Weapon::Type::Melee, "Choppa", 1, 2, 4, 4, -1, 1),
    m_pigstikkaSpearBoss(Weapon::Type::Melee, "Pigstikka Spear", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUK_BOARBOYS};
    m_weapons = {&m_choppa, &m_pigstikkaSpear, &m_warBoarsTusks, &m_choppaBoss, &m_pigstikkaSpearBoss};
}

bool OrrukBoarboys::configure(int numModels, WeaponOption weapons, bool glyphBearer, bool horns)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_waaaghHorns = horns;
    m_glyphBearer = glyphBearer;
    m_weaponOption = weapons;

    // Add the boss
    auto boss = new Model(BASESIZE, WOUNDS);
    switch (weapons)
    {
        case Choppa:
            boss->addMeleeWeapon(&m_choppaBoss);
            break;
        case PigstikkaSpear:
            boss->addMeleeWeapon(&m_pigstikkaSpearBoss);
            break;
    }
    boss->addMeleeWeapon(&m_warBoarsTusks);
    addModel(boss);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        switch (weapons)
        {
            case Choppa:
                model->addMeleeWeapon(&m_choppa);
                break;
            case PigstikkaSpear:
                model->addMeleeWeapon(&m_pigstikkaSpear);
                break;
        }
        model->addMeleeWeapon(&m_warBoarsTusks);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *OrrukBoarboys::Create(const ParameterList &parameters)
{
    auto unit = new OrrukBoarboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Choppa);
    bool glyphBearer = GetBoolParam("Glyph Bearer", parameters, false);
    bool horns = GetBoolParam("Waaagh! Horns", parameters, false);

    bool ok = unit->configure(numModels, weapons, glyphBearer, horns);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukBoarboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Boarboys", factoryMethod);
    }
}

std::string OrrukBoarboys::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Choppa)
        {
            return "Choppa";
        }
        else if (parameter.m_intValue == PigstikkaSpear)
        {
            return "Pigstikka Spear";
        }
    }
    return ParameterValueToString(parameter);
}

int OrrukBoarboys::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Choppa")
    {
        return Choppa;
    }
    else if (enumString == "Pigstikka Spear")
    {
        return PigstikkaSpear;
    }
    return 0;
}

int OrrukBoarboys::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_waaaghHorns)
        modifier += 2;
    return modifier;
}

int OrrukBoarboys::braveryModifier() const
{
    auto modifier = Unit::braveryModifier();
    if (m_glyphBearer)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
        if (!units.empty())
        {
            modifier += 2;
        }
    }
    return modifier;
}

int OrrukBoarboys::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    // Tusker Charge
    if (m_charged && weapon->name() == m_warBoarsTusks.name())
    {
        return RerollFailed;
    }
    return Unit::toWoundModifier(weapon, target);
}

Rerolls OrrukBoarboys::toSaveRerolls(const Weapon *weapon) const
{
    // Tusker Shield
    if (!weapon->isMissile())
    {
        return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

} // namespace Greenskinz
