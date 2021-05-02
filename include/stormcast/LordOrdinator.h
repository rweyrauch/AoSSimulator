/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <vector>
#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordOrdinator : public StormcastEternal {
    public:

        enum WeaponOption {
            Astral_Hammers,
            Astral_Grandhammer
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordOrdinator(Stormhost stormhost, WeaponOption weaponOption, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordOrdinator() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        int arcaneEngineer(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        WeaponOption m_weaponOption = Astral_Hammers;

        Weapon m_astralHammers,
                m_astralGrandhammer;

        mutable std::vector<const Unit *> m_meteoricSlam;  // Modified in weaponDamage method

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Engineer                  Yes
// Comet Strike                     Yes
// Meteoric Slam                    Yes
// Solemn Duty                      Yes
//

} // namespace StormcastEternals
