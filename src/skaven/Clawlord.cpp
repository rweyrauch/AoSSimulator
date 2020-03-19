/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Clawlord.h>
#include <UnitFactory.h>

namespace Skaven
{
bool Clawlord::s_registered = false;

Unit *Clawlord::Create(const ParameterList &parameters)
{
    auto unit = new Clawlord();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Clawlord::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Clawlord", factoryMethod);
    }
}

Clawlord::Clawlord() :
    Skaventide("Clawlord", 6, WOUNDS, 6, 4, false),
    m_blade(Weapon::Type::Melee, "Warpforged Blade", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, HERO, CLAWLORD};
    m_weapons = {&m_blade};
}

bool Clawlord::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int Clawlord::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

    // Cornered Fury
    extra += (initialWounds() - remainingWounds());

    return extra;
}
} //namespace Skaven
