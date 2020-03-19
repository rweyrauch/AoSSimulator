/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Hexwraiths.h>
#include <UnitFactory.h>

namespace Nighthaunt
{
bool Hexwraiths::s_registered = false;

Hexwraiths::Hexwraiths() :
    Nighthaunt("Hexwraiths", 12, WOUNDS, 10, 4, true),
    m_spectralScythe(Weapon::Type::Melee, "Spectral Scythe", 1, 2, 4, 3, -1, 1),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 5, 0, 1),
    m_spectralScytheHellwraith(Weapon::Type::Melee, "Spectral Scythe", 1, 3, 4, 3, -1, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, HEXWRAITHS};
    m_weapons = {&m_spectralScythe, &m_spectralScytheHellwraith, &m_hoovesAndTeeth};
}

bool Hexwraiths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto boss = new Model(BASESIZE, WOUNDS);
    boss->addMeleeWeapon(&m_spectralScytheHellwraith);
    boss->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(boss);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spectralScythe);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Hexwraiths::Create(const ParameterList &parameters)
{
    auto unit = new Hexwraiths();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Hexwraiths::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Hexwraiths::Create,
            nullptr,
            nullptr,
            Hexwraiths::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", Hexwraiths::MIN_UNIT_SIZE, Hexwraiths::MIN_UNIT_SIZE,
                    Hexwraiths::MAX_UNIT_SIZE, Hexwraiths::MIN_UNIT_SIZE
                },
            },
            DEATH,
            { NIGHTHAUNT }
        };
        s_registered = UnitFactory::Register("Hexwraiths", factoryMethod);
    }
}

Wounds Hexwraiths::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Frightful Touch
    if ((woundRoll == 6) && (weapon->name() == m_spectralScythe.name()))
    {
        return {0, 1};
    }
    return Nighthaunt::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int Hexwraiths::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Nighthaunt
