/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Evocators.h>

namespace StormcastEternals
{

Weapon Evocators::s_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1);
Weapon Evocators::s_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 5, 3, 3, -1, 1);
Weapon Evocators::s_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2);
Weapon Evocators::s_grandStavePrime(Weapon::Type::Melee, "Grandstave", 2, 4, 3, 3, 0, 2);

Evocators::Evocators() :
    StormcastEternal("Evocators", 5, WOUNDS, 8, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, RETRIBUTORS };
}

bool Evocators::configure(int numModels, int numGrandstaves, bool primeGrandstave)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    if (primeGrandstave && numGrandstaves <= 0)
    {
        // Must have one stave.
        return false;
    }

    const int maxGrandstaves = numModels / 5;
    if (numGrandstaves > maxGrandstaves)
    {
        // Invalid weapon configuration.
        return false;
    }

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (primeGrandstave)
    {
        primeModel.addMeleeWeapon(&s_grandStavePrime);
        numGrandstaves--;
    }
    else
        primeModel.addMeleeWeapon(&s_tempestBladeAndStavePrime);
    addModel(primeModel);

    for (auto i = 0; i < numGrandstaves; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_grandStave);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_tempestBladeAndStave);
        addModel(model);
    }

    return true;
}

Rerolls Evocators::toSaveRerolls(const Weapon* weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
        return RerollOnes;

    return StormcastEternal::toSaveRerolls(weapon);
}

int Evocators::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    int mortalWounds = Unit::generateMortalWounds(weapon, unit, hits);

    // Celestial Lightning Arc
    Dice dice;
    int roll = dice.rollD6();
    if (roll >= 4)
        mortalWounds++;
    roll = dice.rollD6();
    if (roll >= 4)
        mortalWounds++;

    return mortalWounds;
}

} // namespace StormcastEternals
