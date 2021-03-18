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

        static int ComputePoints(int numModels);

        static void Init();

        ValkiaTheBloody();

        ~ValkiaTheBloody() override = default;

        bool configure();

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_slaupnir;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Gaze of Khorne               TODO
// The Spear Slaupnir               Yes
// Daemonshield                     Yes
// On Bloodstained Wings            TODO
//

} // namespace Khorne
