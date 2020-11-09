/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AKHELIANLEVIADON_H
#define AKHELIANLEVIADON_H

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

        bool configure();

        void onWounded() override;

        void onRestore() override;

    private:

        int getDamageTableIndex() const;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int voidDrumSaveMod(const Unit *unit, const Weapon *weapon);
        int voidDrumToHitMod(const Unit *attacker, const Weapon *weapon, const Unit *target);

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
// Crushing Charge                  TODO
// Jaws of Death                    Yes
// Void Drum                        Yes
//

} // namespace IdonethDeepkin

#endif // AKHELIANLEVIADON_H
