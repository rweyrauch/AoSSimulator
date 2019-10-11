/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBoarboys.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBoarboys::Create,
    SavageBoarboys::ValueToString,
    SavageBoarboys::EnumStringToInt,
    {
        {ParamType::Integer, "Models", SavageBoarboys::MIN_UNIT_SIZE, SavageBoarboys::MIN_UNIT_SIZE, SavageBoarboys::MAX_UNIT_SIZE, SavageBoarboys::MIN_UNIT_SIZE},
        {ParamType::Enum, "Weapons", SavageBoarboys::Chompa, SavageBoarboys::Chompa, SavageBoarboys::SavageStikka, 1},
        {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBoarboys::s_registered = false;

Unit *SavageBoarboys::Create(const ParameterList &parameters)
{
    auto unit = new SavageBoarboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, Chompa);
    bool thumper = GetBoolParam("Skull Thumper", parameters, true);
    bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

    bool ok = unit->configure(numModels, weapons, thumper, totem);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageBoarboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Boarboys", factoryMethod);
    }
}

SavageBoarboys::SavageBoarboys() :
    Unit("Savage Boarboys", 12, WOUNDS, 5, 6, false),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1),
    m_stikka(Weapon::Type::Melee, "Savage Stikka", 2, 3, 4, 4, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
    m_chompaBoss(Weapon::Type::Melee, "Chompa", 1, 4, 4, 3, 0, 1),
    m_stikkaBoss(Weapon::Type::Melee, "Savage Stikka", 2, 4, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOYS};
}

bool SavageBoarboys::configure(int numModels, WeaponOption weapons, bool skullThumper, bool totemBearer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_thumper = skullThumper;
    m_totemBearer = totemBearer;

    // Add the Boss
    Model bossModel(BASESIZE, WOUNDS);
    if (weapons == Chompa)
    {
        bossModel.addMeleeWeapon(&m_chompaBoss);
    }
    else if (weapons == SavageStikka)
    {
        bossModel.addMeleeWeapon(&m_stikkaBoss);
    }
    bossModel.addMeleeWeapon(&m_tusksAndHooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == Chompa)
        {
            model.addMeleeWeapon(&m_chompa);
        }
        else if (weapons == SavageStikka)
        {
            model.addMeleeWeapon(&m_stikka);
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

void SavageBoarboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompa);
    visitor(&m_chompaBoss);
    visitor(&m_stikka);
    visitor(&m_stikkaBoss);
    visitor(&m_tusksAndHooves);
}

std::string SavageBoarboys::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Chompa) { return "Chompa"; }
        else if (parameter.m_intValue == SavageStikka) { return "Savage Stikka"; }
    }
    return ValueToString(parameter);
}

int SavageBoarboys::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Chompa") { return Chompa; }
    else if (enumString == "Savage Stikka") { return SavageStikka; }
    return EnumStringToInt(enumString);
}

} // namespace Bonesplitterz
