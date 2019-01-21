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

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static void Init();

    Sequitors();
    ~Sequitors() override = default;

    bool configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache);

    // Buff shields when not our combat phase.
    void hero(PlayerId id) override { m_aethericChannellingWeapons = false; StormcastEternal::hero(id);}

    // Buff weapons during our combat phase
    void combat(PlayerId id) override { m_aethericChannellingWeapons = true; StormcastEternal::combat(id); }

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Hits applyHitModifiers(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    WeaponOption m_weaponOption = StormsmiteMaul;
    bool m_redemptionCache = false;
    bool m_aethericChannellingWeapons = false;

    static Weapon s_stormsmiteMaul,
        s_stormsmiteMaulPrime,
        s_tempestBlade,
        s_tempestBladePrime,
        s_stormsmiteGreatmace,
        s_stormsmiteGreatmacePrime,
        s_redemptionCache;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //SEQUITORS_H