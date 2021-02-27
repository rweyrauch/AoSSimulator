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

    class LordCelestantOnStardrake : public StormcastEternal {
    public:

        enum WeaponOption {
            Celestine_Hammer,
            Stormbound_Blade,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        LordCelestantOnStardrake();

        ~LordCelestantOnStardrake() override;

        bool configure(WeaponOption weapons, MountTrait trait);

    protected:

        void onWounded() override;

        int getDamageTableIndex() const;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onStartShooting(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        void onRestore() override;

        int arcaneLineage(const Unit *target);

    protected:

        WeaponOption m_weaponOption = Celestine_Hammer;

    private:

        Weapon m_celestineHammer,
                m_stormboundBlade,
                m_greatClaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   Yes
// Cavernous Jaws                   Yes
// Inescapable Vengeance            Yes
// Lord of the Heavens              Yes
// Sigmarite Thundershield          Yes
// Stormbound Blade                 Yes
// Sweeping Tail                    Yes
// Lord of the Celestial Host       Yes
//

} // namespace StormcastEternals
