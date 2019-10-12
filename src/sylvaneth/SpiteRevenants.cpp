/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/SpiteRevenants.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{

static FactoryMethod factoryMethod = {
    SpiteRevenants::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", SpiteRevenants::MIN_UNIT_SIZE, SpiteRevenants::MIN_UNIT_SIZE,
            SpiteRevenants::MAX_UNIT_SIZE, SpiteRevenants::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool SpiteRevenants::s_registered = false;

SpiteRevenants::SpiteRevenants() :
    SylvanethBase("Spite Revenants", 5, WOUNDS, 6, 5, false),
    m_cruelTalonsAndFangs(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 3, 4, 4, 0, 1),
    m_cruelTalonsAndFangsShadestalker(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, SPITE_REVENANTS};
}

bool SpiteRevenants::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model shadestalker(BASESIZE, WOUNDS);
    shadestalker.addMeleeWeapon(&m_cruelTalonsAndFangsShadestalker);
    addModel(shadestalker);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_cruelTalonsAndFangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *SpiteRevenants::Create(const ParameterList &parameters)
{
    auto unit = new SpiteRevenants();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpiteRevenants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spite-Revenants", factoryMethod);
    }
}

void SpiteRevenants::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_cruelTalonsAndFangs);
    visitor(&m_cruelTalonsAndFangsShadestalker);
}

Rerolls SpiteRevenants::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Malicious Tormentors
    if (hasKeyword(DREADWOOD))
    {
        return RerollOnes;
    }
    return SylvanethBase::toHitRerolls(weapon, target);
}

} // namespace Sylvaneth