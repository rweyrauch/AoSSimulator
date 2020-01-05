/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Soulreaper.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    MortisanSoulreaper::Create,
    MortisanSoulreaper::ValueToString,
    MortisanSoulreaper::EnumStringToInt,
    {},
    DEATH,
    OSSIARCH_BONEREAPERS
};

bool MortisanSoulreaper::s_registered = false;

Unit *MortisanSoulreaper::Create(const ParameterList &parameters)
{
    auto unit = new MortisanSoulreaper();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MortisanSoulreaper::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int MortisanSoulreaper::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void MortisanSoulreaper::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mortisan Soulreaper", factoryMethod);
    }
}

MortisanSoulreaper::MortisanSoulreaper() :
    OssiarchBonereaperBase("Mortisan Soulreaper", 5, WOUNDS, 10, 4, false),
    m_scythe(Weapon::Type::Melee, "Soulreaper Scythe", 2, 3, 3, 3, -1, 2)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_SOULREAPER};
    m_weapons = {&m_scythe};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool MortisanSoulreaper::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_scythe);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds MortisanSoulreaper::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deathly Touch
    if ((hitRoll == 6) && (weapon->name() == m_scythe.name()))
    {
        return {0, 2};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls MortisanSoulreaper::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Soulreaper
    if ((target->remainingModels() >= 5) && (weapon->name() == m_scythe.name()))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace OssiarchBonereapers
