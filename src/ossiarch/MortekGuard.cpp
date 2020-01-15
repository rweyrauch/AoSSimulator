/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/MortekGuard.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    MortekGuard::Create,
    MortekGuard::ValueToString,
    MortekGuard::EnumStringToInt,
    MortekGuard::ComputePoints,
    {
        {
            ParamType::Integer, "Models", MortekGuard::MIN_UNIT_SIZE, MortekGuard::MIN_UNIT_SIZE,
            MortekGuard::MAX_UNIT_SIZE, MortekGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Necrophoros", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Enum, "Weapons", MortekGuard::NadiriteBladeAndShield, MortekGuard::NadiriteBladeAndShield, MortekGuard::NadirateSpearAndShield, 1},
        {ParamType::Integer, "Soulcleaver Greatblade", 1, 0, MortekGuard::MAX_UNIT_SIZE/3, 1},
    },
    DEATH,
    { OSSIARCH_BONEREAPERS }
};

bool MortekGuard::s_registered = false;

Unit *MortekGuard::Create(const ParameterList &parameters)
{
    auto unit = new MortekGuard();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, NadiriteBladeAndShield);
    bool necrophoros = GetBoolParam("Necrophoros", parameters, true);
    int numGreatblades = GetIntParam("Soulcleaver Greatblade", parameters, 1);

    bool ok = unit->configure(numModels, weapons, numGreatblades, necrophoros);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MortekGuard::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == NadirateSpearAndShield)
        {
            return "Nadirate Spear and Shield";
        }
        else if (parameter.intValue == NadiriteBladeAndShield)
        {
            return "Nadirite Blade and Shield";
        }
    }
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int MortekGuard::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Nadirite Blade and Shield") return NadiriteBladeAndShield;
    else if (enumString == "Nadirate Spear and Shield") return NadirateSpearAndShield;

    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void MortekGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mortek Guard", factoryMethod);
    }
}

MortekGuard::MortekGuard() :
    OssiarchBonereaperBase("Mortek Guard", 4, WOUNDS, 10, 4, false),
    m_blade(Weapon::Type::Melee, "Nadirite Blade", 1, 2, 3, 4, -1, 1),
    m_spear(Weapon::Type::Melee, "Nadirite Spear", 2, 2, 3, 4, 0, 1),
    m_greatblade(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 2, 3, 3, -1, 1),
    m_bladeHekatos(Weapon::Type::Melee, "Nadirite Blade", 1, 3, 3, 4, -1, 1),
    m_spearHekatos(Weapon::Type::Melee, "Nadirite Spear", 2, 3, 3, 4, 0, 1),
    m_greatbladeHekatos(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTEK_GUARD};
    m_weapons = {&m_blade, &m_spear, &m_greatblade, &m_bladeHekatos, &m_spearHekatos, &m_greatbladeHekatos};
}

bool MortekGuard::configure(int numModels, WeaponOption option, int numGreatblades, bool necrophoros)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGreatblades = numModels / 10;
    if (numGreatblades > maxGreatblades)
    {
        // Invalid weapon configuration.
        return false;
    }

    auto hekatos = new Model(BASESIZE, WOUNDS);
    if (numGreatblades)
    {
        hekatos->addMeleeWeapon(&m_greatbladeHekatos);
        numGreatblades--;
    }
    else if (option == NadiriteBladeAndShield)
    {
        hekatos->addMeleeWeapon(&m_bladeHekatos);
    }
    else if (option == NadirateSpearAndShield)
    {
        hekatos->addMeleeWeapon(&m_spearHekatos);
    }
    addModel(hekatos);
    for (auto i = 0; i < numGreatblades; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_greatblade);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (option == NadiriteBladeAndShield)
        {
            model->addMeleeWeapon(&m_bladeHekatos);
        }
        else if (option == NadirateSpearAndShield)
        {
            model->addMeleeWeapon(&m_spearHekatos);
        }
        addModel(model);
    }

    m_necrophoros = necrophoros;

    m_points = ComputePoints(numModels);

    return true;
}

int MortekGuard::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    if ((unmodifiedHitRoll == 6) && ((weapon->name() == m_blade.name()) || (weapon->name() == m_spear.name()))) return 2;
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int MortekGuard::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_necrophoros) mod++;
    return mod;
}

int MortekGuard::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_necrophoros) mod++;
    return mod;
}

int MortekGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace OssiarchBonereapers
