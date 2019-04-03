/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <death/MorghastArchai.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    MorghastArchai::Create,
    MorghastArchai::ValueToString,
    MorghastArchai::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", MorghastArchai::MIN_UNIT_SIZE, MorghastArchai::MIN_UNIT_SIZE, MorghastArchai::MAX_UNIT_SIZE,
            MorghastArchai::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", MorghastArchai::SpiritHalberd, MorghastArchai::SpiritHalberd, MorghastArchai::SpiritSwords, 1
        },
    },
    DEATH,
    DEATHLORDS
};

bool MorghastArchai::s_registered = false;

MorghastArchai::MorghastArchai() :
    Unit("Morghast Archai", 9, WOUNDS, 10, 4, true),
    m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
    m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {DEATH, REANIMANT, MORGHAST, DEATHLORDS, MORGHAST_ARCHAI};
}

bool MorghastArchai::configure(int numModels, WeaponOptions weapons)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weapons = weapons;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == SpiritHalberd)
        {
            model.addMeleeWeapon(&m_spiritHalberd);
        }
        else if (weapons == SpiritSwords)
        {
            model.addMeleeWeapon(&m_spiritSwords);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void MorghastArchai::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spiritHalberd);
    visitor(&m_spiritSwords);
}

Unit *MorghastArchai::Create(const ParameterList &parameters)
{
    auto unit = new MorghastArchai();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, SpiritHalberd);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MorghastArchai::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == SpiritHalberd)
        { return "Spirit Halberd"; }
        else if (parameter.m_intValue == SpiritSwords)
        { return "Spirit Swords"; }
    }
    return ParameterValueToString(parameter);
}

int MorghastArchai::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Spirit Halberd") { return SpiritHalberd; }
    else if (enumString == "Spirit Swords") { return SpiritSwords; }
    return 0;
}

void MorghastArchai::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Morghast Archai", factoryMethod);
    }
}

} //namespace Death