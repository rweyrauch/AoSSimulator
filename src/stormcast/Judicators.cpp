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

Weapon Judicators::s_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1);
Weapon Judicators::s_skyboltPrime(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 2, 3, -1, 1);
Weapon Judicators::s_boltstormCrossbow(Weapon::Type::Missile, "Boltstorm Crossbow", 12, 3, 3, 4, 0, 1);
Weapon Judicators::s_shockboltBow(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 3, 3, -1, 1);
Weapon Judicators::s_shockboltPrime(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 2, 3, -1, 1);
Weapon Judicators::s_thunderboldCrossbow(Weapon::Type::Missile, "Thunderbolt Crossbow", 18, 0, 0, 0, 0, 0);
Weapon Judicators::s_stormGladius(Weapon::Type::Melee, "Storm Gladius", 1, 1, 3, 4, 0, 1);

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
        primeModel.addMissileWeapon(&s_skyboltPrime);
    }
    else if (m_weaponOption == BoltstormCrossbow)
    {
        primeModel.addMissileWeapon(&s_shockboltPrime);
    }
    primeModel.addMeleeWeapon(&s_stormGladius);
    addModel(primeModel);

    for (auto i = 0; i < numShockboltBows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&s_shockboltBow);
        model.addMeleeWeapon(&s_stormGladius);
        addModel(model);
    }
    for (auto i = 0; i < numThunderboltCrossbows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&s_thunderboldCrossbow);
        model.addMeleeWeapon(&s_stormGladius);
        addModel(model);
    }
    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == SkyboltBow)
            model.addMissileWeapon(&s_skyboltBow);
        else if (m_weaponOption == BoltstormCrossbow)
            model.addMissileWeapon(&s_boltstormCrossbow);
        model.addMeleeWeapon(&s_stormGladius);
        addModel(model);
    }

    return true;
}

Rerolls Judicators::toHitRerollsMissile(const Weapon* weapon, const Unit* unit) const
{
    // External Judgement
    if (unit->hasKeyword(CHAOS))
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerollsMissile(weapon, unit);
}

int Judicators::extraAttacksMissile(const Weapon* weapon) const
{
    // Rapid Fire
    if (!m_ran)
    {
        return 1;
    }
    return StormcastEternal::extraAttacksMissile(weapon);
}

} // namespace StormcastEternals
