/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukArdboys.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    OrrukArdboys::Create,
    OrrukArdboys::ValueToString,
    OrrukArdboys::EnumStringToInt,
    {
        {ParamType::Integer, "Models", {.m_intValue = OrrukArdboys::MIN_UNIT_SIZE}, OrrukArdboys::MIN_UNIT_SIZE, OrrukArdboys::MAX_UNIT_SIZE, OrrukArdboys::MIN_UNIT_SIZE},
        {ParamType::Integer, "Choppas And Shields", {.m_intValue = 0}, 0, OrrukArdboys::MAX_UNIT_SIZE, 1},
        {ParamType::Integer, "Paired Choppas", {.m_intValue = 0}, 0, OrrukArdboys::MAX_UNIT_SIZE, 1},
        {ParamType::Integer, "Big Choppas", {.m_intValue = 0}, 0, OrrukArdboys::MAX_UNIT_SIZE, 1},
        {
            ParamType::Enum, "Boss Weapon", {.m_intValue = OrrukArdboys::ChoppaAndSmasha}, OrrukArdboys::ChoppaOrSmashaWithShield,
            OrrukArdboys::BigChoppa, 1
        },
        {ParamType::Boolean, "Drummer", {.m_boolValue = false}, false, false},
        {
            ParamType::Enum, "Standard", {.m_intValue = OrrukArdboys::None}, OrrukArdboys::None, OrrukArdboys::IconOfGork, 1
        }
    },
    DESTRUCTION,
    IRONJAWZ
};

bool OrrukArdboys::s_registered = false;

OrrukArdboys::OrrukArdboys() :
    Ironjawz("Orruk Ardboys", 4, WOUNDS, 6, 4, false),
    m_choppaOrSmasha(Weapon::Type::Melee, "Orruk-forged Choppa or Smasha", 1, 2, 4, 3, 0, 1),
    m_choppaAndSmasha(Weapon::Type::Melee, "Orruk-forged Choppas and Smashas", 1, 3, 4, 3, 0, 1),
    m_bigChoppa(Weapon::Type::Melee, "Orruk-forged Big Choppa", 1, 2, 4, 3, -1, 1),
    m_bossChoppaOrSmasha(Weapon::Type::Melee, "Orruk-forged Choppa or Smasha (Boss)", 1, 2, 3, 3, 0, 1),
    m_bossChoppaAndSmasha(Weapon::Type::Melee, "Orruk-forged Choppas and Smashas (Boss)", 1, 3, 3, 3, 0, 1),
    m_bossBigChoppa(Weapon::Type::Melee, "Orruk-forged Big Choppa (Boss)", 1, 2, 3, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, ARDBOYS};
}

bool OrrukArdboys::configure(int numModels, int numChoppasAndShield, int numPairedChoppas,
    int numBigChoppas, WeaponOption bossWeapon, bool drummer, StandardOption standard)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    if (numChoppasAndShield + numPairedChoppas + numBigChoppas > numModels)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_numChoppasAndShields = numChoppasAndShield;
    m_numPairedChoppas = numPairedChoppas;
    m_numBigChoppas = numBigChoppas;
    m_drummer = drummer;
    m_standardBearer = standard;
    m_bossWeapon = bossWeapon;

    // Add the Boss
    Model bossModel(BASESIZE, WOUNDS);
    if (m_bossWeapon == ChoppaOrSmashaWithShield)
    {
        bossModel.addMeleeWeapon(&m_bossChoppaOrSmasha);
    }
    else if (m_bossWeapon == ChoppaAndSmasha)
    {
        bossModel.addMeleeWeapon(&m_bossChoppaAndSmasha);
    }
    else if (m_bossWeapon == BigChoppa)
    {
        bossModel.addMeleeWeapon(&m_bossBigChoppa);
    }
    addModel(bossModel);

    for (auto i = 0; i < numChoppasAndShield; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_choppaOrSmasha);
    }
    for (auto i = 0; i < numPairedChoppas; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_choppaAndSmasha);
    }
    for (auto i = 0; i < numBigChoppas; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_bigChoppa);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void OrrukArdboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_choppaOrSmasha);
    visitor(&m_choppaAndSmasha);
    visitor(&m_bigChoppa);
    visitor(&m_bossChoppaOrSmasha);
    visitor(&m_bossChoppaAndSmasha);
    visitor(&m_bossBigChoppa);
}

Unit *OrrukArdboys::Create(const ParameterList &parameters)
{
    auto unit = new OrrukArdboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numChoppasAndShield = GetIntParam("Choppas And Shields", parameters, 0);
    int numPairedChoppas = GetIntParam("Paired Choppas", parameters, 0);
    int numBigChoppas = GetIntParam("Big Choppas", parameters, 0);
    WeaponOption bossWeapon = (WeaponOption) GetEnumParam("Boss Weapon", parameters, ChoppaAndSmasha);
    bool drummer = GetBoolParam("Drummer", parameters, false);
    StandardOption standard = (StandardOption) GetEnumParam("Standard", parameters, None);

    bool ok = unit->configure(numModels, numChoppasAndShield, numPairedChoppas, numBigChoppas, bossWeapon, drummer, standard);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukArdboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Ardboys", factoryMethod);
    }
}

int OrrukArdboys::bravery() const
{
    int modBrav = m_bravery;
    if (m_standardBearer == OrrukBanner)
    {
        modBrav += 2;
    }
    return modBrav;
}

void OrrukArdboys::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    if (m_standardBearer == IconOfGork)
    {
        Dice dice;
        Dice::RollResult result;
        dice.rollD6(numFled, result);
        numFled -= result.numUnmodified6s();
    }
    else
    {
        Ironjawz::computeBattleshockEffect(roll, numFled, numAdded);
    }
}

int OrrukArdboys::chargeModifier() const
{
    int modifier = Ironjawz::chargeModifier();

    if (m_drummer)
        modifier += 2;

    return modifier;
}

Wounds OrrukArdboys::applyWoundSave(const Wounds &wounds)
{
    // Orruk-forged Shields
    if (m_numChoppasAndShields > 0)
    {
        Dice dice;
        Dice::RollResult result;
        dice.rollD6(wounds.normal, result);
        Wounds modWounds = wounds;
        modWounds.normal -= result.numUnmodified6s();
        return modWounds;
    }
    return Ironjawz::applyWoundSave(wounds);
}

std::string OrrukArdboys::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Boss Weapon")
    {
        if (parameter.m_intValue == ChoppaOrSmashaWithShield)
        {
            return "Choppa Or Smasha With Shield";
        }
        else if (parameter.m_intValue == ChoppaAndSmasha)
        {
            return "Choppa And Smasha";
        }
        else if (parameter.m_intValue == BigChoppa)
        {
            return "Big Choppa";
        }
    }
    else if (parameter.m_name == "Standard")
    {
        if (parameter.m_intValue == None)
        {
            return "None";
        }
        else if (parameter.m_intValue == OrrukBanner)
        {
            return "Orruk Banner";
        }
        else if (parameter.m_intValue == IconOfGork)
        {
            return "Icon Of Gork";
        }
    }

    return ParameterValueToString(parameter);
}

int OrrukArdboys::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Choppa Or Smasha With Shield")
    {
        return ChoppaOrSmashaWithShield;
    }
    else if (enumString == "Choppa And Smasha")
    {
        return ChoppaAndSmasha;
    }
    else if (enumString == "Big Choppa")
    {
        return BigChoppa;
    }
    else if (enumString == "None")
    {
        return None;
    }
    else if (enumString == "Orruk Banner")
    {
        return OrrukBanner;
    }
    else if (enumString == "Icon Of Gork")
    {
        return IconOfGork;
    }
    return 0;
}

} // namespace Ironjawz
