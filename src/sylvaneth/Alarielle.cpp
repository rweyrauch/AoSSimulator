/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Alarielle.h>

namespace Sylvaneth
{

Weapon Alarielle::s_spearOfKurnoth("Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6);
Weapon Alarielle::s_talonOfDwindling("Talon of Dwindling", 1, 4, 3, 4, 0, 1);
Weapon Alarielle::s_beetleGreatAntlers("Wardroth Beetle's Great Antlers", 2, 5, 4, 3, -2, 5);

Alarielle::Alarielle() :
    Unit("Alarielle", 16, WOUNDS, 10, 3, true)
{
    m_keywords = { ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN };
}

bool Alarielle::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(s_spearOfKurnoth);
    model.addMeleeWeapon(s_talonOfDwindling);
    model.addMeleeWeapon(s_beetleGreatAntlers);
    addModel(model);

    return true;
}

int Alarielle::toHitModifier(const Unit& unit) const
{
    // Sweeping Blows
    // TODO: need to limit this buff to the Antler's only.
    if (unit.remainingModels() >= 5)
    {
        return 1;
    }
    return Unit::toHitModifier(unit);
}

void Alarielle::hero()
{
    // Lifebloom
    if (remainingWounds() < WOUNDS && remainingWounds() > 0)
    {
        // heal D3
        Dice dice;
        int woundsHealed = dice.rollD3();
        for (auto& m : m_models)
        {
            m.woundsRemaining() += woundsHealed;
            if (m.woundsRemaining() > WOUNDS)
                m.woundsRemaining() = WOUNDS;
        }
    }
}

} // namespace Sylvaneth
