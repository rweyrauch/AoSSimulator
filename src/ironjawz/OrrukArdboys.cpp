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
        {ParamType::Integer, "Models", OrrukArdboys::MIN_UNIT_SIZE, OrrukArdboys::MIN_UNIT_SIZE, OrrukArdboys::MAX_UNIT_SIZE, OrrukArdboys::MIN_UNIT_SIZE},
        {ParamType::Integer, "Shields", 0, 0, OrrukArdboys::MAX_UNIT_SIZE / 5 * 2, 1},
        {ParamType::Boolean, "Drummer", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {
            ParamType::Enum, "Standard", OrrukArdboys::None, OrrukArdboys::None, OrrukArdboys::GlyphBearer, 1
        },
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DESTRUCTION,
    { IRONJAWZ }
};

bool OrrukArdboys::s_registered = false;

OrrukArdboys::OrrukArdboys() :
    Ironjawz("Orruk Ardboys", 4, WOUNDS, 6, 4, false),
    m_choppa(Weapon::Type::Melee, "Ardboy Choppa", 1, 2, 3, 3, -1, 1),
    m_bossChoppa(Weapon::Type::Melee, "Ardboy Choppa", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, ARDBOYS};
    m_weapons = {&m_choppa, &m_bossChoppa};
}

bool OrrukArdboys::configure(int numModels, int numShields, bool drummer, StandardOption standard)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxShields = (numModels / 5) * 2;
    if (numShields > maxShields)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_numShields = numShields;
    m_drummer = drummer;
    m_standardBearer = standard;

    // Add the Boss
    auto bossModel = new Model(BASESIZE, WOUNDS);
    bossModel->addMeleeWeapon(&m_bossChoppa);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_choppa);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *OrrukArdboys::Create(const ParameterList &parameters)
{
    auto unit = new OrrukArdboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numShields = GetIntParam("Shields", parameters, 0);
    bool drummer = GetBoolParam("Drummer", parameters, false);
    StandardOption standard = (StandardOption) GetEnumParam("Standard", parameters, None);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(numModels, numShields, drummer, standard);
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
    if (m_standardBearer == BannerBearer)
    {
        modBrav += 2;
    }
    return modBrav;
}

void OrrukArdboys::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    Ironjawz::computeBattleshockEffect(roll, numFled, numAdded);
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
    if (m_numShields > 0)
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
    if (std::string(parameter.name) == "Standard")
    {
        if (parameter.intValue == None)
        {
            return "None";
        }
        else if (parameter.intValue == BannerBearer)
        {
            return "Banner Bearer";
        }
        else if (parameter.intValue == GlyphBearer)
        {
            return "Glyph Bearer";
        }
    }

    return Ironjawz::ValueToString(parameter);
}

int OrrukArdboys::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Banner Bearer")
    {
        return BannerBearer;
    }
    else if (enumString == "Glyph Bearer")
    {
        return GlyphBearer;
    }
    return Ironjawz::EnumStringToInt(enumString);
}

} // namespace Ironjawz
