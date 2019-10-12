/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/ArchRevenant.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    ArchRevenant::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool ArchRevenant::s_registered = false;

ArchRevenant::ArchRevenant() :
    SylvanethBase("Arch-Revenant", 12, WOUNDS, 8, 4, true),
    m_glaive(Weapon::Type::Melee, "Revenant's Glaive", 2, 3, 3, 3, -2, 2),
    m_tailPincers(Weapon::Type::Melee, "Zephyrspite's Tail Pincers", 1, 1, 4, 3, 0, RAND_D3)
{
    m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, HERO, ARCH_REVENANT};
}

bool ArchRevenant::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_glaive);
    model.addMeleeWeapon(&m_tailPincers);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *ArchRevenant::Create(const ParameterList &parameters)
{
    auto unit = new ArchRevenant();

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ArchRevenant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arch-Revenant", factoryMethod);
    }
}

void ArchRevenant::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_glaive);
    visitor(&m_tailPincers);
}

Rerolls ArchRevenant::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Crescent Shield
    if (!m_crescentShieldProtection)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

Rerolls ArchRevenant::toSaveRerolls(const Weapon *weapon) const
{
    // Crescent Shield
    if (m_crescentShieldProtection)
    {
        return RerollOnes;
    }

    return Unit::toSaveRerolls(weapon);
}

} // namespace Sylvaneth