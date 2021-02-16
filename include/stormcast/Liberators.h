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
#include <UnitFactory.h>

namespace StormcastEternals {

    class Liberators : public StormcastEternal {
    public:

        enum WeaponOption {
            Warhammer = 0,
            Warblade,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Liberators();

        ~Liberators() override = default;

        bool
        configure(int numModels, WeaponOption weapons, bool pairedWeapons, int numGrandhammers, int numGrandblades);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        WeaponOption m_weaponOption = Warhammer;
        bool m_pairedWeapons = false;

        Weapon m_warhammer,
                m_warhammerPrime,
                m_warblade,
                m_warbladePrime,
                m_grandhammer,
                m_grandblade;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lay Low the Tyrants              Yes
// Paired Weapons                   Yes
// Sigmarite Shields                Yes
//

} // namespace StormcastEternals
