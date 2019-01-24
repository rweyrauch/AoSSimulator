/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHoppers.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SquiqHoppers::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = SquiqHoppers::MIN_UNIT_SIZE}, SquiqHoppers::MIN_UNIT_SIZE,
         SquiqHoppers::MAX_UNIT_SIZE, SquiqHoppers::MIN_UNIT_SIZE}
    }
};

bool SquiqHoppers::s_registered = false;

Weapon SquiqHoppers::s_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon SquiqHoppers::s_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1);
Weapon SquiqHoppers::s_slittaBoss(Weapon::Type::Melee, "Slitta (Boss)", 1, 1, 4, 5, 0, 1);

SquiqHoppers::SquiqHoppers() :
    Unit("Squig Hoppers", RAND_3D6, WOUNDS, 4, 6, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HOPPERS };
}

bool SquiqHoppers::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&s_slittaBoss);
    boss.addMeleeWeapon(&s_fangFilledGob);
    addModel(boss);

    // and the rest
    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_slitta);
        model.addMeleeWeapon(&s_fangFilledGob);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_slitta.name() << ": " << s_slitta.strength() << std::endl;
        std::cout << "\t" << s_slittaBoss.name() << ": " << s_slittaBoss.strength() << std::endl;
        std::cout << "\t" << s_fangFilledGob.name() << ": " << s_fangFilledGob.strength() << std::endl;
    }

    return true;
}

Unit *SquiqHoppers::Create(const ParameterList &parameters)
{
    auto unit = new SquiqHoppers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SquiqHoppers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Squiq Hoppers", factoryMethod);
    }
}

} // namespace GloomspiteGitz
