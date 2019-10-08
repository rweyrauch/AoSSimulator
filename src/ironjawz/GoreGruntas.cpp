/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/GoreGruntas.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    OrrukGoreGruntas::Create,
    OrrukGoreGruntas::ValueToString,
    OrrukGoreGruntas::EnumStringToInt,
    {
        {ParamType::Integer, "Models", OrrukGoreGruntas::MIN_UNIT_SIZE, OrrukGoreGruntas::MIN_UNIT_SIZE, OrrukGoreGruntas::MAX_UNIT_SIZE, OrrukGoreGruntas::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", OrrukGoreGruntas::PigIronChoppa, OrrukGoreGruntas::PigIronChoppa,
            OrrukGoreGruntas::JaggedGorehacka, 1
        },
    },
    DESTRUCTION,
    IRONJAWZ
};

bool OrrukGoreGruntas::s_registered = false;

OrrukGoreGruntas::OrrukGoreGruntas() :
    Ironjawz("Orruk Gore-gruntas", 9, WOUNDS, 7, 4, false),
    m_pigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 4, 3, 3, -1, 1),
    m_jaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 4, 0, 1),
    m_bossPigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa (Boss)", 1, 5, 3, 3, -1, 1),
    m_bossJaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka (Boss)", 2, 5, 3, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, GORE_GRUNTAS};
}

bool OrrukGoreGruntas::configure(int numModels, OrrukGoreGruntas::WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Boss
    Model bossModel(BASESIZE, WOUNDS);
    if (weapons == PigIronChoppa)
    {
        bossModel.addMeleeWeapon(&m_bossPigironChoppa);
    }
    else if (weapons == JaggedGorehacka)
    {
        bossModel.addMeleeWeapon(&m_bossJaggedGorehacka);
    }
    bossModel.addMeleeWeapon(&m_tusksAndHooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == PigIronChoppa)
        {
            model.addMeleeWeapon(&m_pigironChoppa);
        }
        else if (weapons == JaggedGorehacka)
        {
            model.addMeleeWeapon(&m_jaggedGorehacka);
        }
        model.addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void OrrukGoreGruntas::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_pigironChoppa);
    visitor(&m_jaggedGorehacka);
    visitor(&m_tusksAndHooves);
    visitor(&m_bossPigironChoppa);
    visitor(&m_bossJaggedGorehacka);
}

Unit *OrrukGoreGruntas::Create(const ParameterList &parameters)
{
    auto unit = new OrrukGoreGruntas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, PigIronChoppa);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string OrrukGoreGruntas::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == PigIronChoppa)
        {
            return "Pig-iron Choppa";
        }
        else if (parameter.m_intValue == JaggedGorehacka)
        {
            return "Jagged Gore-hacka";
        }
    }
    return ParameterValueToString(parameter);
}

int OrrukGoreGruntas::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Pig-iron Choppa")
    {
        return PigIronChoppa;
    }
    else if (enumString == "Jagged Gore-hacka")
    {
        return JaggedGorehacka;
    }
    return 0;
}

void OrrukGoreGruntas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Gore-gruntas", factoryMethod);
    }
}

} // namespace Ironjawz