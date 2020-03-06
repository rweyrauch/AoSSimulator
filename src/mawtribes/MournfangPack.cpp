/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/MournfangPack.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    MournfangPack::Create,
    MournfangPack::ValueToString,
    MournfangPack::EnumStringToInt,
    MournfangPack::ComputePoints,
    {
        {
            ParamType::Integer, "Models", MournfangPack::MIN_UNIT_SIZE, MournfangPack::MIN_UNIT_SIZE,
            MournfangPack::MAX_UNIT_SIZE, MournfangPack::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", MournfangPack::CullingClubOrPreyHackerAndIronfist, MournfangPack::CullingClubOrPreyHackerAndIronfist, MournfangPack::GargantHacker, 1},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool MournfangPack::s_registered = false;

Unit *MournfangPack::Create(const ParameterList &parameters)
{
    auto unit = new MournfangPack();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, CullingClubOrPreyHackerAndIronfist);
    bool banner = GetBoolParam("Banner Bearer", parameters, true);
    bool hornblower = GetBoolParam("Hornblower", parameters, true);

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure(numModels, weapons, hornblower, banner);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MournfangPack::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mournfang Pack", factoryMethod);
    }
}

MournfangPack::MournfangPack() :
    MawtribesBase("Mournfang Pack", 9, WOUNDS, 6, 4, false),
    m_pistol(Weapon::Type::Missile, "Ironlock Pistol", 12, 1, 4, 3, -1, RAND_D3),
    m_clubOrHacker(Weapon::Type::Melee, "Culling Club or Prey Hacker", 1, 3, 3, 3, 0, 2),
    m_gargantHacker(Weapon::Type::Melee, "Gargant Hacker", 2, 2, 4, 3, -1, 3),
    m_tusks(Weapon::Type::Melee, "Tusks", 1, 4, 4, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MOURNFANG_PACK};
    m_weapons = {&m_pistol, &m_clubOrHacker, &m_gargantHacker, &m_tusks};
}

bool MournfangPack::configure(int numModels, WeaponOption weaponOption, bool hornblower, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_option = weaponOption;
    m_hornblower = hornblower;
    m_bannerBearer = bannerBearer;

    auto skalg = new Model(BASESIZE, WOUNDS);
    skalg->addMissileWeapon(&m_pistol);
    if (weaponOption == CullingClubOrPreyHackerAndIronfist)
        skalg->addMeleeWeapon(&m_clubOrHacker);
    else if (weaponOption == GargantHacker)
        skalg->addMeleeWeapon(&m_gargantHacker);
    skalg->addMeleeWeapon(&m_tusks);
    addModel(skalg);

    if (m_hornblower)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Hornblower");
        if (weaponOption == CullingClubOrPreyHackerAndIronfist)
            model->addMeleeWeapon(&m_clubOrHacker);
        else if (weaponOption == GargantHacker)
            model->addMeleeWeapon(&m_gargantHacker);
        model->addMeleeWeapon(&m_tusks);
        addModel(model);
    }
    if (m_bannerBearer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Banner Bearer");
        if (weaponOption == CullingClubOrPreyHackerAndIronfist)
            model->addMeleeWeapon(&m_clubOrHacker);
        else if (weaponOption == GargantHacker)
            model->addMeleeWeapon(&m_gargantHacker);
        model->addMeleeWeapon(&m_tusks);
        addModel(model);
    }
    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weaponOption == CullingClubOrPreyHackerAndIronfist)
            model->addMeleeWeapon(&m_clubOrHacker);
        else if (weaponOption == GargantHacker)
            model->addMeleeWeapon(&m_gargantHacker);
        model->addMeleeWeapon(&m_tusks);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int MournfangPack::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

std::string MournfangPack::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == CullingClubOrPreyHackerAndIronfist) return "Culling Club or Prey Hacker and Ironfist";
        else if (parameter.intValue == GargantHacker) return "Gargant Hacker";
    }
    return MawtribesBase::ValueToString(parameter);
}

int MournfangPack::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Culling Club or Prey Hacker and Ironfist") return CullingClubOrPreyHackerAndIronfist;
    else if (enumString == "Gargant Hacker") return GargantHacker;

    return MawtribesBase::EnumStringToInt(enumString);
}

Wounds MournfangPack::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Mournfang Charge
    if (m_charged && (weapon->name() == m_tusks.name()))
    {
        return {weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds MournfangPack::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    // Ironfist
    if ((saveRoll == 6) && (m_option == CullingClubOrPreyHackerAndIronfist))
    {
        return {0, 1};
    }
    return Unit::computeReturnedDamage(weapon, saveRoll);
}

int MournfangPack::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

void MournfangPack::onWounded()
{
    MawtribesBase::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Hornblower"))
        {
            m_hornblower = false;
        }
        if (ip->slain() && (ip->getName() == "Banner Bearer"))
        {
            m_bannerBearer = false;
        }
    }
}

void MournfangPack::onRestore()
{
    MawtribesBase::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->getName() == "Hornblower")
        {
            m_hornblower = true;
        }
        if (ip->getName() == "Banner Bearer")
        {
            m_bannerBearer = true;
        }
    }
}

int MournfangPack::braveryModifier() const
{
    auto mod = MawtribesBase::braveryModifier();

    if (m_bannerBearer) mod++;

    return mod;
}

} // namespace OgorMawtribes
