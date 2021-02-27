/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <stormcast/StormcastEternals.h>

namespace StormcastEternals {

    class MountedStormcastEternal : public StormcastEternal {
    public:

        MountedStormcastEternal() = default;

        ~MountedStormcastEternal() override;

    protected:
        MountedStormcastEternal(const std::string &name, int move, int wounds, int bravery, int save, bool fly);

        int moveModifier() const override;

        int weaponRend(const Weapon *weapon, const Unit *target,
                       int hitRoll, int woundRoll) const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls runRerolls() const override;

        int prideLeader(const Unit *attacker, const Weapon *weapon, const Unit *target);

        void onEndCombat(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartCombat(PlayerId player) override;

    protected:

        lsignal::slot m_prideLeaderConnection;

        MountTrait m_mountTrait = MountTrait::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mount Traits
//    Lithe_Limbed                  Yes
//    Keen_Clawed                   Yes
//    Savage_Loyalty                Yes
//    Drake_Kin                     TODO
//    Thunder_Caller                TODO
//    Pack_Leader                   Yes
//    Storm_Winged                  TODO
//    Thunderlord                   Yes
//    Star_Branded                  Yes
//    Wind_Runner                   TODO
//    Aethereal_Stalker             TODO
//    Indefatigable                 Yes
//    Swiftwing                     Yes
//    Lashing_Tail                  Yes
//    Steel_Pinions                 Yes
//    Bounding_Leap                 TODO
//    Pride_Leader                  Yes
//    Ear_Bursting_Roar             Yes
//

} // namespace StormcastEternals
