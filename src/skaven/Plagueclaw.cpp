/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Plagueclaw.h>
#include <UnitFactory.h>

namespace Skaven
{
bool Plagueclaw::s_registered = false;

Unit *Plagueclaw::Create(const ParameterList &parameters)
{
    auto unit = new Plagueclaw();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Plagueclaw::Init()
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

        s_registered = UnitFactory::Register("Plagueclaw", factoryMethod);
    }
}

Plagueclaw::Plagueclaw() :
    Skaventide("Plagueclaw", 3, WOUNDS, 4, 5, false),
    m_catapult(Weapon::Type::Missile, "Plagueclaw Catapult", 31, 1, 3, 3, -2, RAND_D6),
    m_knives(Weapon::Type::Melee, "Rusty Knives", 1, RAND_D6, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, PLAGUECLAW};
    m_weapons = {&m_catapult, &m_knives};
}

bool Plagueclaw::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_catapult);
    model->addMeleeWeapon(&m_knives);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int Plagueclaw::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Skaventide::toHitModifier(weapon, target);

    // Barrage of Disease
    if (target->remainingModels() >= 10) mod++;

    return mod;
}

Wounds Plagueclaw::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto wounds = Skaventide::weaponDamage(weapon, target, hitRoll, woundRoll);

    // Barrage of Disease
    if (target->remainingModels() >= 10)
    {
        wounds.normal += Dice::roll2D6();
    }
    return wounds;
}

} //namespace Skaven
