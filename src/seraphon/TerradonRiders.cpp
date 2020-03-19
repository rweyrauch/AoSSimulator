/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/TerradonRiders.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

bool TerradonRiders::s_registered = false;

TerradonRiders::TerradonRiders() :
    SeraphonBase("Terradon Riders", 16, WOUNDS, 5, 6, true),
    m_javelin(Weapon::Type::Missile, "Starstrike Javelin", 12, 2, 4, 3, 0, 1),
    m_javelinLeader(Weapon::Type::Missile, "Starstrike Javelin", 12, 3, 4, 3, 0, 1),
    m_bolas(Weapon::Type::Missile, "Sunleech Bolas", 6, RAND_D6, 4, 3, 0, 1),
    m_bolasLeader(Weapon::Type::Missile, "Sunleech Bolas", 6, RAND_D6, 4, 3, 0, 1),
    m_jaws(Weapon::Type::Melee, "Razor-sharp Jaws", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, TERRADON, TERRADON_RIDERS};
    m_weapons = {&m_javelin, &m_javelinLeader, &m_bolas, &m_bolasLeader, &m_jaws};
}

bool TerradonRiders::configure(int numModels, WeaponOption option)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Add the Alpha
    auto alpha = new Model(BASESIZE, WOUNDS);
    if (option == StarstrikeJavelins)
    {
        alpha->addMissileWeapon(&m_javelinLeader);
    }
    else if (option == SunleechBolas)
    {
        alpha->addMissileWeapon(&m_bolasLeader);
    }
    alpha->addMeleeWeapon(&m_jaws);
    addModel(alpha);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (option == StarstrikeJavelins)
        {
            model->addMissileWeapon(&m_javelin);
        }
        else if (option == SunleechBolas)
        {
            model->addMissileWeapon(&m_bolas);
        }
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *TerradonRiders::Create(const ParameterList &parameters)
{
    auto unit = new TerradonRiders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto option = (WeaponOption)GetEnumParam("Weapons", parameters, StarstrikeJavelins);

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure(numModels, option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TerradonRiders::Init()
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
                {ParamType::Enum, "Weapons", StarstrikeJavelins, StarstrikeJavelins, SunleechBolas, 1},
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Terradon Riders", factoryMethod);
    }
}

int TerradonRiders::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

std::string TerradonRiders::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == StarstrikeJavelins) return "Starstrike Javelins";
        else if (parameter.intValue == SunleechBolas) return "Sunleech Bolas";
    }
    return SeraphonBase::ValueToString(parameter);
}

int TerradonRiders::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Starstrike Javelins") return StarstrikeJavelins;
    else if (enumString == "Sunleech Bolas") return SunleechBolas;
    return SeraphonBase::EnumStringToInt(enumString);
}

} //namespace Seraphon
