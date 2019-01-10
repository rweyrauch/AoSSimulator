/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHoppers.h>

namespace GloomspiteGitz
{

Weapon SquiqHoppers::s_fangFilledGob("Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon SquiqHoppers::s_slitta("Slitta", 1, 1, 5, 5, 0, 1);
Weapon SquiqHoppers::s_slittaBoss("Slitta", 1, 1, 4, 5, 0, 1);

SquiqHoppers::SquiqHoppers() :
    Unit("Squig Hoppers", RAND_3D6, WOUNDS, 4, 6, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HOPPERS };
}

bool SquiqHoppers::configure(int numModels)
{
    // validate inputs
    if (numModels < 5 || numModels > 20)
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

    return true;
}

} // namespace GloomspiteGitz
