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
static FactoryMethod factoryMethod = {
    Hexwraiths::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Hexwraiths::MIN_UNIT_SIZE, Hexwraiths::MIN_UNIT_SIZE,
            Hexwraiths::MAX_UNIT_SIZE, Hexwraiths::MIN_UNIT_SIZE
        },
    },
    DEATH,
    NIGHTHAUNT
};

bool Hexwraiths::s_registered = false;

Hexwraiths::Hexwraiths() :
    Nighthaunt("Hexwraiths", 12, WOUNDS, 10, 4, true),
    m_spectralScythe(),
    m_hoovesAndTeeth(),
    m_spectralScytheHellwraith()
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, HEXWRAITHS};
}

bool Hexwraiths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&m_spectralScytheHellwraith);
    boss.addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(boss);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_spectralScythe);
        model.addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Hexwraiths::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spectralScythe);
    visitor(&m_hoovesAndTeeth);
    visitor(&m_spectralScytheHellwraith);
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

} //namespace Nighthaunt
