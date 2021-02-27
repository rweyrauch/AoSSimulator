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

    class VandusHammerhand : public MountedStormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VandusHammerhand();

        ~VandusHammerhand() override;

        bool configure(MountTrait trait);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

        int lordOfTheHammerhandsBraveryMod(const Unit *unit);

    private:

        Weapon m_heldensen,
                m_clawsAndFangs;

        lsignal::slot m_lordSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Heldensen                        Yes
// Intolerable Damage               Yes
// Storm Breath                     Yes
// Lord of the Hammerhands          Yes
// Vengeful Determination           Yes
//

} // namespace StormcastEternals
