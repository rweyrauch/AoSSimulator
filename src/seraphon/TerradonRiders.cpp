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
static FactoryMethod factoryMethod = {
    TerradonRiders::Create,
    TerradonRiders::ValueToString,
    TerradonRiders::EnumStringToInt,
    TerradonRiders::ComputePoints,
    {
        {
            ParamType::Integer, "Models", TerradonRiders::MIN_UNIT_SIZE, TerradonRiders::MIN_UNIT_SIZE,
            TerradonRiders::MAX_UNIT_SIZE, TerradonRiders::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", TerradonRiders::StarstrikeJavelins, TerradonRiders::StarstrikeJavelins, TerradonRiders::SunleechBolas, 1},
        {ParamType::Boolean, "Master of the Skies", SIM_FALSE, SIM_FALSE, SIM_TRUE, 0}
    },
    ORDER,
    { SERAPHON }
};

bool TerradonRiders::s_registered = false;

TerradonRiders::TerradonRiders() :
    SeraphonBase("Terradon Riders", 14, WOUNDS, 10, 5, true),
    m_javelin(Weapon::Type::Missile, "Starstrike Javelin", 10, 2, 4, 3, 0, 1),
    m_javelinLeader(Weapon::Type::Missile, "Starstrike Javelin", 10, 2, 3, 3, 0, 1),
    m_bolas(Weapon::Type::Missile, "Sunleech Bolas", 5, 1, 4, 4, 0, 1),
    m_bolasLeader(Weapon::Type::Missile, "Sunleech Bolas", 5, 1, 3, 4, 0, 1),
    m_beak(Weapon::Type::Melee, "Terradon's Razor-sharp Beak", 1, 4, 4, 4, 0, 1),
    m_skyblade(Weapon::Type::Melee, "Skyblade", 1, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINK, TERRADON_RIDERS};
    m_weapons = {&m_javelin, &m_javelinLeader, &m_bolas, &m_bolasLeader, &m_beak, &m_skyblade};
}

bool TerradonRiders::configure(int numModels, WeaponOption option, bool masterOfTheSkies)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    if (masterOfTheSkies)
    {
        auto master = new Model(BASESIZE, WOUNDS);
        master->addMeleeWeapon(&m_skyblade);
        master->addMeleeWeapon(&m_beak);
        addModel(master);
    }
    else
    {
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
        alpha->addMeleeWeapon(&m_beak);
        addModel(alpha);
    }

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
        model->addMeleeWeapon(&m_beak);
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
    bool master = GetBoolParam("Master of the Skies", parameters, false);

    bool ok = unit->configure(numModels, option, master);
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
