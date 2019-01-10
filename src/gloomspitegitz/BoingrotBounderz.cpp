/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/BoingrotBounderz.h>

namespace GloomspiteGitz
{

Weapon BoingrotBounderz::s_fangFilledGob("Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon BoingrotBounderz::s_pokinLance("Pokin' Lance", 2, 2, 4, 4, -1, 1);
Weapon BoingrotBounderz::s_pokinLanceBoss("Pokin' Lance", 2, 2, 3, 4, -1, 1);

BoingrotBounderz::BoingrotBounderz() :
    Unit("Boingrot Bounderz", RAND_2D6, WOUNDS, 5, 4, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, BOINGROT_BOUNDERZ };
}

bool BoingrotBounderz::configure(int numModels)
{
    // validate inputs
    if (numModels < 5 || numModels > 20)
    {
        // Invalid model count.
        return false;
    }

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&s_pokinLanceBoss);
    boss.addMeleeWeapon(&s_fangFilledGob);
    addModel(boss);

    // and the rest
    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_pokinLance);
        model.addMeleeWeapon(&s_fangFilledGob);
        addModel(model);
    }

    return true;
}

} // namespace GloomspiteGitz
