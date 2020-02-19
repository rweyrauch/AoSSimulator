/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include <UnitFactory.h>
#include "nighthaunt/DreadbladeHarrow.h"

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    DreadbladeHarrow::Create,
    nullptr,
    nullptr,
    DreadbladeHarrow::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool DreadbladeHarrow::s_registered = false;

Unit *DreadbladeHarrow::Create(const ParameterList &parameters)
{
    auto unit = new DreadbladeHarrow();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DreadbladeHarrow::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadblade Harrow", factoryMethod);
    }
}

DreadbladeHarrow::DreadbladeHarrow() :
    Nighthaunt("Dreadblade Harrow", 12, WOUNDS, 10, 4, true),
    m_dreadblade(Weapon::Type::Melee, "Dreadblade", 1, 3, 3, 3, -1, 1),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, DREADBLADE_HARROW};
    m_weapons = {&m_dreadblade, &m_hoovesAndTeeth};
}

bool DreadbladeHarrow::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_dreadblade);
    model->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int DreadbladeHarrow::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Dreadblade
    auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
    if (!m_charged && (weapon->name() == m_dreadblade.name())) attacks++;

    return attacks;
}

Wounds DreadbladeHarrow::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Dreadblade
    auto wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    if (m_charged && (weapon->name() == m_dreadblade.name())) wounds.normal++;

    return wounds;
}

Rerolls DreadbladeHarrow::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), KNIGHT_OF_SHROUDS, 9.0f);
    if (unit && (weapon->name() == m_dreadblade.name()))
    {
        return RerollOnes;
    }
    return Unit::toWoundRerolls(weapon, target);
}
} // namespace Nighthaunt