/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Judicators.h>

namespace StormcastEternals
{

Weapon Judicators::s_skyboltBow("Skybolt Bow", 24, 1, 3, 3, -1, 1);
Weapon Judicators::s_boltstormCrossbow("Boltstorm Crossbow", 12, 3, 3, 4, 0, 1);
Weapon Judicators::s_shockboltBow("Shockbolt Bow", 24, 1, 3, 3, -1, 1);
Weapon Judicators::s_thunderboldCrossbow("Thunderbolt Crossbow", 18, 0, 0, 0, 0, 0);
Weapon Judicators::s_stormGladius("Storm Gladius", 1, 1, 3, 4, 0, 1);

Judicators::Judicators() :
    StormcastEternal("Judicators", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, JUDICATORS };

    // Chained Lightning
    s_shockboltBow.setHitsPerAttack(RAND_D6);
}

bool Judicators::configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxSpecialWeapons = numModels / 5;
    if (numShockboltBows + numThunderboltCrossbows > maxSpecialWeapons)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (m_weaponOption == SkyboltBow)
    {
        Weapon skyboltPrime("Skybolt Bow", 24, 1, 2, 3, -1, 1);
        primeModel.addMissileWeapon(skyboltPrime);
    }
    else if (m_weaponOption == BoltstormCrossbow)
    {
        Weapon shockboltPrime("Shockbolt Bow", 24, 1, 2, 3, -1, 1);
        primeModel.addMissileWeapon(shockboltPrime);
    }
    primeModel.addMeleeWeapon(s_stormGladius);
    addModel(primeModel);

    for (auto i = 0; i < numShockboltBows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(s_shockboltBow);
        model.addMeleeWeapon(s_stormGladius);
        addModel(model);
    }
    for (auto i = 0; i < numThunderboltCrossbows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(s_thunderboldCrossbow);
        model.addMeleeWeapon(s_stormGladius);
        addModel(model);
    }
    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == SkyboltBow)
            model.addMissileWeapon(s_skyboltBow);
        else if (m_weaponOption == BoltstormCrossbow)
            model.addMissileWeapon(s_boltstormCrossbow);
        model.addMeleeWeapon(s_stormGladius);
        addModel(model);
    }

    return true;
}

Rerolls Judicators::toHitRerollsMissile(const Unit* unit) const
{
    // External Judgement
    if (unit->hasKeyword(CHAOS))
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerollsMissile(unit);
}

int Judicators::extraAttacksMissile() const
{
    // Rapid Fire
    if (!m_ran)
    {
        return 1;
    }
    return StormcastEternal::extraAttacksMissile();
}

} // namespace StormcastEternals
