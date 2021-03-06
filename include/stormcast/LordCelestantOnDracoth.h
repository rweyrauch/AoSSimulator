/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/MountedStormcast.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordCelestantOnDracoth : public MountedStormcastEternal {
    public:

        enum WeaponOption {
            Tempestos_Hammer,
            Thunderaxe,
            Lightning_Hammer,
            Stormstrike_Glaive,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordCelestantOnDracoth(Stormhost stormhost, WeaponOption weapons, bool sigmariteThundershield, CommandTrait trait, Artefact artefact, MountTrait mountTrait, bool isGeneral);

        ~LordCelestantOnDracoth() override = default;

    protected:

        WeaponOption m_weapon = Tempestos_Hammer;
        bool m_sigmariteThundershield = false;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_stormstrikeGlaive,
                m_lightningHammer,
                m_thunderaxe,
                m_tempestosHammer,
                m_clawsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lightning Hammer                 Partial/TODO
// Intolerable Damage               Yes
// Sigmarite Thundershield          Yes
// Stormstrike Glaive               Yes
// Storm Breath                     Yes
// Tempestos Hammer                 Yes
// Thunderaxe                       Yes
// Lord of the Host                 Yes
//

} // namespace StormcastEternals
