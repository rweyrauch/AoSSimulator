/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/MountedStormcast.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LynusGhalmorianOnGryphcharger : public MountedStormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        LynusGhalmorianOnGryphcharger();

        ~LynusGhalmorianOnGryphcharger() override;

        bool configure(Lore lore, MountTrait trait);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

        Rerolls shieldOfThePaleKnight(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave,
                m_beakAndClaws;

        lsignal::slot m_shieldConnection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethereal Strike                 Yes
// Cycle of the Storm               TODO
// Ride the Winds Aetheric          TODO
// Spirit Flask                     Yes
// Shield of the Pale Knight        Yes
// Prime Electrids                  Yes
// Amethyst Gale                    Yes
// Sombre Exemplar                  Yes
//

} // namespace StormcastEternals
