/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/LiegeKavalos.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    LiegeKavalos::Create,
    LiegeKavalos::ValueToString,
    LiegeKavalos::EnumStringToInt,
    {
    },
    DEATH,
    OSSIARCH_BONEREAPERS
};

bool LiegeKavalos::s_registered = false;

Unit *LiegeKavalos::Create(const ParameterList &parameters)
{
    auto unit = new LiegeKavalos();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string LiegeKavalos::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int LiegeKavalos::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void LiegeKavalos::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Liege-Kavalos", factoryMethod);
    }
}

LiegeKavalos::LiegeKavalos() :
    OssiarchBonereaperBase("Liege-Kavalos", 10, WOUNDS, 10, 3, false),
    m_blade(Weapon::Type::Melee, "Commander's Blade", 1, 3, 3, 3, -1, 2),
    m_shield(Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, LIEGE, HERO, LIEGE_KAVALOS};
    m_weapons = {&m_blade, &m_shield, &m_hoovesAndTeeth};
}

bool LiegeKavalos::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blade);
    model->addMeleeWeapon(&m_shield);
    model->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}
} // namespace OssiarchBonereapers
