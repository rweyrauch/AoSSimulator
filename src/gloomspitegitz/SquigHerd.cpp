/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHerd.h>

namespace GloomspiteGitz
{

Weapon SquiqHerd::s_fangFilledGob("Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon SquiqHerd::s_squigProdder("Squig Prodder", 1, 2, 5, 5, 0, 1);

SquiqHerd::SquiqHerd() :
    Unit("Squig Herd", 5, WOUNDS, 3, 6, false)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HERD };
}

bool SquiqHerd::configure(int numModels)
{
    // validate inputs
    if (numModels < 6 || numModels > 24)
    {
        // Invalid model count.
        return false;
    }

    const int numHerders = (numModels+5) / 6;

    // Add the herder
    for (auto i = 0; i < numHerders; i++)
    {
        Model herder(BASESIZE, WOUNDS);
        herder.addMeleeWeapon(s_squigProdder);
        addModel(herder);
    }

    // and the squigs
    for (auto i = numHerders; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(s_fangFilledGob);
        addModel(model);
    }

    return true;
}

} // namespace GloomspiteGitz
