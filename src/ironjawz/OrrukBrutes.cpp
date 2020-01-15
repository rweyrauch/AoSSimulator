/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukBrutes.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    OrrukBrutes::Create,
    OrrukBrutes::ValueToString,
    OrrukBrutes::EnumStringToInt,
    OrrukBrutes::ComputePoints,
    {
        {ParamType::Integer, "Models", OrrukBrutes::MIN_UNIT_SIZE, OrrukBrutes::MIN_UNIT_SIZE, OrrukBrutes::MAX_UNIT_SIZE, OrrukBrutes::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", OrrukBrutes::TwoBruteChoppas, OrrukBrutes::TwoBruteChoppas,
            OrrukBrutes::JaggedGorehacka, 1
        },
        {ParamType::Integer, "Gore Choppas", 0, 0, OrrukBrutes::MAX_UNIT_SIZE / 5, 1},
        {
            ParamType::Enum, "Boss Weapon", OrrukBrutes::BossChoppa, OrrukBrutes::BossChoppa, OrrukBrutes::BossKlaw, 1
        },
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DESTRUCTION,
    { IRONJAWZ }
};

bool OrrukBrutes::s_registered = false;

OrrukBrutes::OrrukBrutes() :
    Ironjawz("Orruk Brutes", 4, WOUNDS, 6, 4, false),
    m_twoBruteChoppas(Weapon::Type::Melee, "Pair of Brute Choppas", 1, 4, 3, 3, -1, 1),
    m_gorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
    m_gorechoppa(Weapon::Type::Melee, "Gore-choppa", 2, 3, 4, 3, -1, 2),
    m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 3, 3, 3, -1, 2),
    m_bossKlawAndBruteSmasha(Weapon::Type::Melee, "Boss Klaw and Brute Smasha", 1, 4, 4, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, BRUTES};
    m_weapons = {&m_twoBruteChoppas, &m_gorehacka, &m_gorechoppa, &m_bossChoppa, &m_bossKlawAndBruteSmasha};
}

bool OrrukBrutes::configure(int numModels, WeaponOption weapons, int numGoreChoppas, BossWeaponOption bossWeapon)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGoreChoppas = numModels / 5;
    if (numGoreChoppas > maxGoreChoppas)
    {
        // Invalid weapon configuration.
        return false;
    }

    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (bossWeapon == BossChoppa)
    {
        bossModel->addMeleeWeapon(&m_bossChoppa);
    }
    else if (bossWeapon == BossKlaw)
    {
        bossModel->addMeleeWeapon(&m_bossKlawAndBruteSmasha);
    }
    addModel(bossModel);

    for (auto i = 0; i < numGoreChoppas; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_gorechoppa);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == TwoBruteChoppas)
        {
            model->addMeleeWeapon(&m_twoBruteChoppas);
        }
        else if (weapons == JaggedGorehacka)
        {
            model->addMeleeWeapon(&m_gorehacka);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *OrrukBrutes::Create(const ParameterList &parameters)
{
    auto unit = new OrrukBrutes();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, TwoBruteChoppas);
    int numGoreChoppas = GetIntParam("Gore Choppas", parameters, 0);
    BossWeaponOption bossWeapon = (BossWeaponOption) GetEnumParam("Boss Weapon", parameters, BossChoppa);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(numModels, weapons, numGoreChoppas, bossWeapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukBrutes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Brutes", factoryMethod);
    }
}

std::string OrrukBrutes::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == TwoBruteChoppas)
        {
            return "Two Brute Choppas";
        }
        else if (parameter.intValue == JaggedGorehacka)
        {
            return "Jagged Gore-hacka";
        }
    }

    if (std::string(parameter.name) == "Boss Weapon")
    {
        if (parameter.intValue == BossChoppa)
        {
            return "Boss Choppa";
        }
        else if (parameter.intValue == BossKlaw)
        {
            return "Boss Klaw";
        }
    }

    return Ironjawz::ValueToString(parameter);
}

int OrrukBrutes::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Two Brute Choppas")
    {
        return TwoBruteChoppas;
    }
    else if (enumString == "Jagged Gore-hacka")
    {
        return JaggedGorehacka;
    }
    else if (enumString == "Boss Choppa")
    {
        return BossChoppa;
    }
    else if (enumString == "Boss Klaw")
    {
        return BossKlaw;
    }

    return Ironjawz::EnumStringToInt(enumString);
}

Rerolls OrrukBrutes::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Duff Up da Big Thing
    if (target->wounds() >= 4)
    {
        return RerollFailed;
    }
    return Ironjawz::toHitRerolls(weapon, target);
}

int OrrukBrutes::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Ironjawz
