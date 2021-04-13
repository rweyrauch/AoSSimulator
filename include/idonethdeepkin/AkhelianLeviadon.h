/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class AkhelianLeviadon : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AkhelianLeviadon();

        ~AkhelianLeviadon() override;

        void onWounded() override;

        void onRestore() override;

    private:

        void configure(MountTrait trait);

        size_t getDamageTableIndex() const;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int voidDrumSaveMod(const Unit *unit, const Weapon *weapon);
        int voidDrumToHitMod(const Unit *attacker, const Weapon *weapon, const Unit *target);

        void onCharged() override;

    private:

        lsignal::slot m_voidDrumSaveSlot,
            m_voidDrumHitSlot;

        Weapon m_harpoonLauncher,
                m_crushingJaws,
                m_scythedFins,
                m_spearAndHarpoons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crushing Charge                  Yes
// Jaws of Death                    Yes
// Void Drum                        Yes
//

} // namespace IdonethDeepkin

