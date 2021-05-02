/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class ValkiaTheBloody : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ValkiaTheBloody() = delete;

        ~ValkiaTheBloody() override;

    protected:

        ValkiaTheBloody(SlaughterHost host, bool isGeneral);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onRestore() override {
            m_usedGazeOfKhorneReroll = nullptr;
        }

    protected:

        Rerolls gazeOfKhorne(const Unit *unit);

        int gazeOfKhorneFlee(const Unit *unit, int roll);

    private:

        lsignal::slot m_gazeOfKhorne;
        lsignal::slot m_gazeOfKhorneFlee;
        const Unit* m_usedGazeOfKhorneReroll = nullptr;

        Weapon m_slaupnir{Weapon::Type::Melee, "Slaupnir", 2, 6, 3, 3, -2, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Gaze of Khorne               Yes
// The Spear Slaupnir               Yes
// Daemonshield                     Yes
// On Bloodstained Wings            TODO
//

} // namespace Khorne
