/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ossiarch/MorghastHarbingers.h>
#include <UnitFactory.h>

namespace OssiarchBonereapers
{

bool MorghastHarbingers::s_registered = false;

MorghastHarbingers::MorghastHarbingers() :
    OssiarchBonereaperBase("Morghast Harbingers", 9, WOUNDS, 10, 4, true),
    m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
    m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_HARBINGERS};
    m_weapons = {&m_spiritHalberd, &m_spiritSwords};
}

bool MorghastHarbingers::configure(int numModels, WeaponOptions weapons)
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

    m_points = ComputePoints(numModels);

    return true;
}

Unit *MorghastHarbingers::Create(const ParameterList &parameters)
{
    auto unit = new MorghastHarbingers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, SpiritHalberd);

    auto legion = (Legion)GetEnumParam("Legion", parameters, None);
    unit->setLegion(legion);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MorghastHarbingers::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == SpiritHalberd) { return "Spirit Halberd"; }
        else if (parameter.intValue == SpiritSwords) { return "Spirit Swords"; }
    }
    return ParameterValueToString(parameter);
}

int MorghastHarbingers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Spirit Halberd") { return SpiritHalberd; }
    else if (enumString == "Spirit Swords") { return SpiritSwords; }
    return 0;
}

void MorghastHarbingers::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            MorghastHarbingers::Create,
            MorghastHarbingers::ValueToString,
            MorghastHarbingers::EnumStringToInt,
            MorghastHarbingers::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", MorghastHarbingers::MIN_UNIT_SIZE, MorghastHarbingers::MIN_UNIT_SIZE, MorghastHarbingers::MAX_UNIT_SIZE,
                    MorghastHarbingers::MIN_UNIT_SIZE
                },
                {
                    ParamType::Enum, "Weapons", MorghastHarbingers::SpiritHalberd, MorghastHarbingers::SpiritHalberd, MorghastHarbingers::SpiritSwords, 1
                },
                {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None, OssiarchBonereaperBase::Crematorians, 1},
            },
            DEATH,
            { OSSIARCH_BONEREAPERS }
        };
        s_registered = UnitFactory::Register("Morghast Harbingers", factoryMethod);
    }
}

int MorghastHarbingers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace OssiarchBonereapers