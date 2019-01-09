/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SEQUITORS_H
#define SEQUITORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Sequitors : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    enum WeaponOption
    {
        StormsmiteMaul,
        TempestBlade
    };

    Sequitors();
    ~Sequitors() override = default;

    bool configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache);

protected:

    Rerolls toSaveRerolls() const override;

private:

    WeaponOption m_weaponOption = StormsmiteMaul;
    bool m_redemptionCache = false;
    bool m_aethericChannellingWeapons = false;

    static Weapon s_stormsmiteMaul,
        s_stormsmiteMaulPrime,
        s_tempestBlade,
        s_tempestBladePrime,
        s_stormsmiteGreatmace,
        s_stormsmiteGreatmacePrime;

};

} // namespace StormcastEternals

#endif //SEQUITORS_H