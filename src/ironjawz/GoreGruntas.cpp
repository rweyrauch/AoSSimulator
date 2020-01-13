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
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DESTRUCTION,
    { IRONJAWZ }
};

bool OrrukGoreGruntas::s_registered = false;

OrrukGoreGruntas::OrrukGoreGruntas() :
    Ironjawz("Orruk Gore-gruntas", 9, WOUNDS, 7, 4, false),
    m_pigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 4, 3, 3, -1, 1),
    m_jaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 4, 0, 1),
    m_bossPigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 5, 3, 3, -1, 1),
    m_bossJaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 5, 3, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, GORE_GRUNTAS};
    m_weapons = {&m_pigironChoppa, &m_jaggedGorehacka, &m_tusksAndHooves, &m_bossPigironChoppa, &m_bossJaggedGorehacka};
}

bool OrrukGoreGruntas::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Boss
    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (weapons == PigIronChoppa)
    {
        bossModel->addMeleeWeapon(&m_bossPigironChoppa);
    }
    else if (weapons == JaggedGorehacka)
    {
        bossModel->addMeleeWeapon(&m_bossJaggedGorehacka);
    }
    bossModel->addMeleeWeapon(&m_tusksAndHooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == PigIronChoppa)
        {
            model->addMeleeWeapon(&m_pigironChoppa);
        }
        else if (weapons == JaggedGorehacka)
        {
            model->addMeleeWeapon(&m_jaggedGorehacka);
        }
        model->addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *OrrukGoreGruntas::Create(const ParameterList &parameters)
{
    auto unit = new OrrukGoreGruntas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, PigIronChoppa);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

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
    if (std::string(parameter.name) == "weapons")
    {
        if (parameter.intValue == PigIronChoppa)
        {
            return "Pig-iron Choppa";
        }
        else if (parameter.intValue == JaggedGorehacka)
        {
            return "Jagged Gore-hacka";
        }
    }
    return Ironjawz::ValueToString(parameter);
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
    return Ironjawz::EnumStringToInt(enumString);
}

void OrrukGoreGruntas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Gore-gruntas", factoryMethod);
    }
}

} // namespace Ironjawz