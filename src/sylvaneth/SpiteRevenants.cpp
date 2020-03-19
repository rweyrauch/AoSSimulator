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

bool SpiteRevenants::s_registered = false;

SpiteRevenants::SpiteRevenants() :
    SylvanethBase("Spite Revenants", 5, WOUNDS, 6, 5, false),
    m_cruelTalonsAndFangs(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 3, 3, 3, 0, 1),
    m_cruelTalonsAndFangsShadestalker(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 4, 3, 3, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, OUTCASTS, SPITE_REVENANTS};
    m_weapons = {&m_cruelTalonsAndFangs, &m_cruelTalonsAndFangsShadestalker};

    s_globalBraveryMod.connect(this, &SpiteRevenants::unbridledMalice, &m_connection);
}

SpiteRevenants::~SpiteRevenants()
{
    m_connection.disconnect();
}

bool SpiteRevenants::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto shadestalker = new Model(BASESIZE, WOUNDS);
    shadestalker->addMeleeWeapon(&m_cruelTalonsAndFangsShadestalker);
    addModel(shadestalker);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_cruelTalonsAndFangs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

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
        static FactoryMethod factoryMethod = {
            SpiteRevenants::Create,
            SylvanethBase::ValueToString,
            SylvanethBase::EnumStringToInt,
            SpiteRevenants::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", SpiteRevenants::MIN_UNIT_SIZE, SpiteRevenants::MIN_UNIT_SIZE,
                    SpiteRevenants::MAX_UNIT_SIZE, SpiteRevenants::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
            },
            ORDER,
            { SYLVANETH }
        };
        s_registered = UnitFactory::Register("Spite-Revenants", factoryMethod);
    }
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

int SpiteRevenants::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int SpiteRevenants::unbridledMalice(const Unit *target)
{
    // Unbridled Malice
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }

    return 0;
}

} // namespace Sylvaneth