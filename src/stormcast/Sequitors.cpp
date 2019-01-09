/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Sequitors.h>

namespace StormcastEternals
{

Weapon Sequitors::s_stormsmiteMaul("Stormsmite Maul", 1, 2, 3, 3, 0, 1);
Weapon Sequitors::s_tempestBlade("Tempest Blade", 1, 3, 3, 4, 0, 1);
Weapon Sequitors::s_stormsmiteGreatmace("Stormsmite Greatmace", 1, 2, 3, 3, -1, 2);

Weapon Sequitors::s_stormsmiteMaulPrime("Stormsmite Maul", 1, 3, 3, 3, 0, 1);
Weapon Sequitors::s_tempestBladePrime("Tempest Blade", 1, 4, 3, 4, 0, 1);
Weapon Sequitors::s_stormsmiteGreatmacePrime("Stormsmite Greatmace", 1, 3, 3, 3, -1, 2);

Sequitors::Sequitors() :
    StormcastEternal("Sequitors", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, REDEEMER, SEQUITORS };
}

bool Sequitors::configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGrandmaces = (numModels / 5)*2;
    if (numGreatmaces > maxGrandmaces)
    {
        // Invalid weapon configuration.
        return false;
    }
    if (primeGreatmace && redemptionCache)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;
    m_redemptionCache = redemptionCache;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (primeGreatmace)
    {
        primeModel.addMeleeWeapon(s_stormsmiteGreatmacePrime);
    }
    else if (m_weaponOption == StormsmiteMaul)
    {
        primeModel.addMeleeWeapon(s_stormsmiteMaulPrime);
    }
    else if (m_weaponOption == TempestBlade)
    {
        primeModel.addMeleeWeapon(s_tempestBladePrime);
    }
    addModel(primeModel);

    for (auto i = 0; i < numGreatmaces; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(s_stormsmiteGreatmace);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == StormsmiteMaul)
            model.addMeleeWeapon(s_stormsmiteMaul);
        else if (m_weaponOption == TempestBlade)
            model.addMeleeWeapon(s_tempestBlade);
        addModel(model);
    }

    return true;
}

Rerolls Sequitors::toSaveRerolls() const
{
    // TODO: determine phase
    bool shootingPhase = false;

    // Soulshields Shields
    for (const auto &m : m_models)
    {
        // check if remaining models have a shield
        for (auto ip = m.meleeWeaponBegin(); ip != m.meleeWeaponEnd(); ++ip)
        {
            if (ip->name() == s_stormsmiteMaul.name() || ip->name() == s_tempestBlade.name())
            {
                if (m_aethericChannellingWeapons || shootingPhase)
                    return RerollOnes; // weapons empowered
                else
                    return RerollFailed; // shields empowered
            }
        }
    }

    return NoRerolls;
}

} // namespace StormcastEternals
