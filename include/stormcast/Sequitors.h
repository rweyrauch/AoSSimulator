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

namespace StormcastEternals {

    class Sequitors : public StormcastEternal {
    public:

        enum WeaponOption {
            Stormsmite_Maul,
            Tempest_Blade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Sequitors();

        ~Sequitors() override = default;

        bool
        configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        // Buff shields when not our combat phase.
        void onStartHero(PlayerId id) override { m_aethericChannellingWeapons = false; }

        // Buff weapons during our combat phase
        void onStartCombat(PlayerId id) override { m_aethericChannellingWeapons = true; }

        void onStartShooting(PlayerId player) override;

    private:

        WeaponOption m_weaponOption = Stormsmite_Maul;
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

//
// Abilities                    Implemented
// -------------------------------------------
// Greatmace Blast                  Yes
// Redemption Cache                 Yes
// Aetheric Channeling              Yes
// Soulshields                      Yes
//
} // namespace StormcastEternals

#endif //SEQUITORS_H