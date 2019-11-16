/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/KavalosZandtos.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    ArchKavalosZandtos::Create,
    ArchKavalosZandtos::ValueToString,
    ArchKavalosZandtos::EnumStringToInt,
    {
    },
    DEATH,
    OSSIARCH_BONEREAPERS
};

bool ArchKavalosZandtos::s_registered = false;

Unit *ArchKavalosZandtos::Create(const ParameterList &parameters)
{
    auto unit = new ArchKavalosZandtos();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string ArchKavalosZandtos::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int ArchKavalosZandtos::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void ArchKavalosZandtos::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arch-Kavalos Zandtos", factoryMethod);
    }
}

ArchKavalosZandtos::ArchKavalosZandtos() :
    OssiarchBonereaperBase("Arch-Kavalos Zandtos", 12, WOUNDS, 10, 3, false),
    m_lance(Weapon::Type::Melee, "The Dark Lance", 2, 3, 3, 3, -1, 2),
    m_shield(Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, LIEGE, HERO, ARCH_KAVALOS_ZANDTOS};
}

bool ArchKavalosZandtos::configure()
{
    Model model(WOUNDS, BASESIZE);
    model.addMeleeWeapon(&m_lance);
    model.addMeleeWeapon(&m_shield);
    model.addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ArchKavalosZandtos::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_lance);
    visitor(m_shield);
    visitor(m_hoovesAndTeeth);
}

Wounds ArchKavalosZandtos::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // The Dark Lance
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return { 3, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls ArchKavalosZandtos::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Hatred of the Living
    if (target->hasKeyword(ORDER) || target->hasKeyword(DESTRUCTION))
    {
        return RerollOnes;
    }
    else if (target->hasKeyword(CHAOS))
    {
        return RerollFailed;
    }
    return Unit::toWoundRerolls(weapon, target);
}

} // namespace OssiarchBonereapers
