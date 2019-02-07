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
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    enum WeaponOption
    {
        StormsmiteMaul,
        TempestBlade
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    Sequitors();
    ~Sequitors() override = default;

    bool configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache);

    // Buff shields when not our combat phase.
    void hero(PlayerId id) override { m_aethericChannellingWeapons = false; StormcastEternal::hero(id);}

    // Buff weapons during our combat phase
    void combat(PlayerId id) override { m_aethericChannellingWeapons = true; StormcastEternal::combat(id); }

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    WeaponOption m_weaponOption = StormsmiteMaul;
    bool m_haveRedemptionCache = false;
    bool m_aethericChannellingWeapons = false;

    Weapon m_stormsmiteMaul,
        m_stormsmiteMaulPrime,
        m_tempestBlade,
        m_tempestBladePrime,
        m_stormsmiteGreatmace,
        m_stormsmiteGreatmacePrime,
        m_redemptionCache;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //SEQUITORS_H