/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GlaivewraithStalkers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    GlaivewraithStalkers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", GlaivewraithStalkers::MIN_UNIT_SIZE, GlaivewraithStalkers::MIN_UNIT_SIZE,
            GlaivewraithStalkers::MAX_UNIT_SIZE, GlaivewraithStalkers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Drummer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    DEATH,
    NIGHTHAUNT
};

bool GlaivewraithStalkers::s_registered = false;

GlaivewraithStalkers::GlaivewraithStalkers() :
    Nighthaunt("Glaivewraith Stalkers", 6, WOUNDS, 10, 4, true),
    m_huntersGlaive(Weapon::Type::Melee, "Hunter's Glaive", 2, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GLAIVEWRAITH_STALKERS};
}

bool GlaivewraithStalkers::configure(int numModels, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_drummer = drummer;
    m_runAndCharge = m_drummer;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_huntersGlaive);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *GlaivewraithStalkers::Create(const ParameterList &parameters)
{
    auto unit = new GlaivewraithStalkers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool drummer = GetBoolParam("Drummer", parameters, true);

    bool ok = unit->configure(numModels, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GlaivewraithStalkers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Glaivewraith Stalkers", factoryMethod);
    }
}

Rerolls GlaivewraithStalkers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // The Point of Death
    if (m_charged || unit->charged())
    {
        return RerollFailed;
    }

    return Nighthaunt::toHitRerolls(weapon, unit);
}

void GlaivewraithStalkers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_huntersGlaive);
}

} // namespace Nighthaunt
