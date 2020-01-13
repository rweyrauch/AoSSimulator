/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ossiarch/MorghastArchai.h>
#include <UnitFactory.h>

namespace OssiarchBonereapers
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
    { OSSIARCH_BONEREAPERS }
};

bool MorghastArchai::s_registered = false;

MorghastArchai::MorghastArchai() :
    OssiarchBonereaperBase("Morghast Archai", 9, WOUNDS, 10, 4, true),
    m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
    m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_ARCHAI};
    m_weapons = {&m_spiritHalberd, &m_spiritSwords};
}

bool MorghastArchai::configure(int numModels, WeaponOptions weapons)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weaponOption = weapons;

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == SpiritHalberd)
        {
            model->addMeleeWeapon(&m_spiritHalberd);
        }
        else if (weapons == SpiritSwords)
        {
            model->addMeleeWeapon(&m_spiritSwords);
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
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == SpiritHalberd)
        { return "Spirit Halberd"; }
        else if (parameter.intValue == SpiritSwords)
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

Wounds MorghastArchai::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Ebon-wrought Armour
    Dice::RollResult mortalSaves;
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} //namespace OssiarchBonereapers