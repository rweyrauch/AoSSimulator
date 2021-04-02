/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Judicators : public StormcastEternal {
    public:

        enum WeaponOption {
            Skybolt_Bow,
            Boltstorm_Crossbow,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Judicators(Stormhost stormhost, int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows);

        ~Judicators() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onStartShooting(PlayerId player) override;

    private:

        WeaponOption m_weaponOption = Skybolt_Bow;

        Weapon m_skyboltBow,
                m_skyboltPrime,
                m_boltstormCrossbow,
                m_shockboltBow,
                m_shockboltPrime,
                m_thunderboldCrossbow,
                m_stormGladius;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chained Lightning                Yes
// Eternal Judgement                Yes
// Rapid Fire                       Yes
// Thunderbolt Crossbow             Yes
//

} // namespace StormcastEternals
