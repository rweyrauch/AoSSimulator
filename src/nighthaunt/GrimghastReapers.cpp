/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GrimghastReapers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    GrimghastReapers::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = GrimghastReapers::MIN_UNIT_SIZE}, GrimghastReapers::MIN_UNIT_SIZE,
         GrimghastReapers::MAX_UNIT_SIZE},
    }
};

Weapon GrimghastReapers::s_slasherScythe(Weapon::Type::Melee, "Slasher Scythe", 2, 2, 4, 3, -1, 1);
Weapon GrimghastReapers::s_deathKnell(Weapon::Type::Melee, "Death Knell", 2, 1, 3, 3, -1, 2);

bool GrimghastReapers::s_registered = false;

GrimghastReapers::GrimghastReapers() :
    Unit("Grimghast Reapers", 8, WOUNDS, 10, 4, true)
{
    m_keywords = { DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GRIMGHAST_REAPERS };
}

bool GrimghastReapers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    Model extoller(BASESIZE, WOUNDS);
    // TODO: implement 'For Whom the Bell Tolls' (mortal wounds)
    extoller.addMeleeWeapon(&s_deathKnell);
    addModel(extoller);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_slasherScythe);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_slasherScythe.name() << ": " << s_slasherScythe.strength() << std::endl;
        std::cout << "\t" << s_deathKnell.name() << ": " << s_deathKnell.strength() << std::endl;
    }

    return true;
}

Unit *GrimghastReapers::Create(const ParameterList &parameters)
{
    auto unit = new GrimghastReapers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GrimghastReapers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Grimghast Reapers", factoryMethod);
    }
}

Rerolls GrimghastReapers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Reaped Like Corn
    if (weapon->name() == "Slasher Scythe" && unit->remainingModels() >= 5)
        return RerollFailed;

    return Unit::toHitRerolls(weapon, unit);
}

int GrimghastReapers::toSaveModifier(const Weapon *weapon) const
{
    // Etherial - no save modifiers allowed.
    int modifier = 0;

    // Etherial - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
        modifier = -weapon->rend();

    return modifier;
}

} // namespace Nighthaunt
